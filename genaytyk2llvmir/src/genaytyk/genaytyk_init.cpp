#include "genaytyk_impl.h"

#include "genaytyk_exception.h"

namespace genaytyk
{
    GenaytykLlvmIrTranslatorGenaytyk_impl::GenaytykLlvmIrTranslatorGenaytyk_impl(llvm::Module *module, llvm::IRBuilder<> &irbuilder) : module(module),
                                                                                                                                       reg2parentMap(REG_ERROR, REG_EIP)
    {
        this->initialize(irbuilder);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initialize(llvm::IRBuilder<> &irbuilder)
    {
        this->initializeRegNameMap();
        this->initializeRegTypeMap();
        this->initializeRegLlvmMap(); // this must go first because of pointers
        this->initializeLlvmRegMap();

        this->initializeRegistersParentMap32(); // Map registers

        this->initializeFunctions(irbuilder);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRegNameMap()
    {
        std::map<uint32_t, std::string> r2n =
            {
                {REG_EIP, "eip"},
                {REG_EAX, "eax"},
                {REG_EBX, "ebx"},
                {REG_ECX, "ecx"},
                {REG_EDX, "edx"},
                {REG_ESP, "esp"},
                {REG_EBP, "ebp"},
                {REG_0x24, "reg_0x24"},
                {REG_0x28, "reg_0x28"},
                {REG_0x2C, "reg_0x2c"},
                {REG_0x30, "reg_0x30"},
                {REG_0x34, "reg_0x34"},
                {REG_0x38, "reg_0x38"},
                {REG_AL, "al"},
                {REG_AH, "ah"},
                {REG_BL, "bl"},
                {REG_BH, "bh"},
                {REG_CL, "cl"},
                {REG_CH, "ch"},
                {REG_DL, "dl"},
                {REG_DH, "dh"},
                {REG_AX, "ax"},
                {REG_BX, "bx"},
                {REG_CX, "cx"},
                {REG_DX, "dx"},
                {REG_SP, "sp"},
                {REG_BP, "bp"},
                {REG_DI, "di"},
                {REG_SI, "si"}};

        this->reg2name = std::move(r2n);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRegTypeMap()
    {
        auto *i8 = llvm::IntegerType::getInt8Ty(this->module->getContext());
        auto *i16 = llvm::IntegerType::getInt16Ty(this->module->getContext());
        auto *i32 = llvm::IntegerType::getInt32Ty(this->module->getContext());

        std::map<uint32_t, llvm::Type *> r2t =
            {
                {REG_EIP, i32},
                {REG_EAX, i32},
                {REG_EBX, i32},
                {REG_ECX, i32},
                {REG_EDX, i32},
                {REG_ESP, i32},
                {REG_EBP, i32},
                {REG_0x24, i32},
                {REG_0x28, i32},
                {REG_0x2C, i32},
                {REG_0x30, i32},
                {REG_0x34, i32},
                {REG_0x38, i32},
                {REG_AL, i8},
                {REG_AH, i8},
                {REG_BL, i8},
                {REG_BH, i8},
                {REG_CL, i8},
                {REG_CH, i8},
                {REG_DL, i8},
                {REG_DH, i8},
                {REG_AX, i16},
                {REG_BX, i16},
                {REG_CX, i16},
                {REG_DX, i16},
                {REG_SP, i16},
                {REG_BP, i16},
                {REG_DI, i16},
                {REG_SI, i16}};

        this->reg2type = std::move(r2t);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRegLlvmMap()
    {
        std::map<uint32_t, llvm::GlobalVariable *> r2l =
            {
                {REG_EIP, this->translateCreateGlobal(this->reg2type[REG_EIP], this->reg2name[REG_EIP])},
                {REG_EAX, this->translateCreateGlobal(this->reg2type[REG_EAX], this->reg2name[REG_EAX])},
                {REG_EBX, this->translateCreateGlobal(this->reg2type[REG_EBX], this->reg2name[REG_EBX])},
                {REG_ECX, this->translateCreateGlobal(this->reg2type[REG_ECX], this->reg2name[REG_ECX])},
                {REG_EDX, this->translateCreateGlobal(this->reg2type[REG_EDX], this->reg2name[REG_EDX])},
                {REG_ESP, this->translateCreateGlobal(this->reg2type[REG_ESP], this->reg2name[REG_ESP])},
                {REG_EBP, this->translateCreateGlobal(this->reg2type[REG_EBP], this->reg2name[REG_EBP])},
                {REG_0x24, this->translateCreateGlobal(this->reg2type[REG_0x24], this->reg2name[REG_0x24])},
                {REG_0x28, this->translateCreateGlobal(this->reg2type[REG_0x28], this->reg2name[REG_0x28])},
                {REG_0x2C, this->translateCreateGlobal(this->reg2type[REG_0x2C], this->reg2name[REG_0x2C])},
                {REG_0x30, this->translateCreateGlobal(this->reg2type[REG_0x30], this->reg2name[REG_0x30])},
                {REG_0x34, this->translateCreateGlobal(this->reg2type[REG_0x34], this->reg2name[REG_0x34])},
                {REG_0x38, this->translateCreateGlobal(this->reg2type[REG_0x38], this->reg2name[REG_0x38])},
                {REG_AL, this->translateCreateGlobal(this->reg2type[REG_AL], this->reg2name[REG_AL])},
                {REG_AH, this->translateCreateGlobal(this->reg2type[REG_AH], this->reg2name[REG_AH])},
                {REG_BL, this->translateCreateGlobal(this->reg2type[REG_BL], this->reg2name[REG_BL])},
                {REG_BH, this->translateCreateGlobal(this->reg2type[REG_BH], this->reg2name[REG_BH])},
                {REG_CL, this->translateCreateGlobal(this->reg2type[REG_CL], this->reg2name[REG_CL])},
                {REG_CH, this->translateCreateGlobal(this->reg2type[REG_CH], this->reg2name[REG_CH])},
                {REG_DL, this->translateCreateGlobal(this->reg2type[REG_DL], this->reg2name[REG_DL])},
                {REG_DH, this->translateCreateGlobal(this->reg2type[REG_DH], this->reg2name[REG_DH])},
                {REG_AX, this->translateCreateGlobal(this->reg2type[REG_AX], this->reg2name[REG_AX])},
                {REG_BX, this->translateCreateGlobal(this->reg2type[REG_BX], this->reg2name[REG_BX])},
                {REG_CX, this->translateCreateGlobal(this->reg2type[REG_CX], this->reg2name[REG_CX])},
                {REG_DX, this->translateCreateGlobal(this->reg2type[REG_DX], this->reg2name[REG_DX])},
                {REG_SP, this->translateCreateGlobal(this->reg2type[REG_SP], this->reg2name[REG_SP])},
                {REG_BP, this->translateCreateGlobal(this->reg2type[REG_BP], this->reg2name[REG_BP])},
                {REG_DI, this->translateCreateGlobal(this->reg2type[REG_DI], this->reg2name[REG_DI])},
                {REG_SI, this->translateCreateGlobal(this->reg2type[REG_SI], this->reg2name[REG_SI])}};

        this->GenaytykRegs2Llvm = std::move(r2l);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeLlvmRegMap()
    {
        std::map<llvm::GlobalVariable *, uint32_t> l2r =
            {
                {this->GenaytykRegs2Llvm[REG_EIP], REG_EIP},
                {this->GenaytykRegs2Llvm[REG_EAX], REG_EAX},
                {this->GenaytykRegs2Llvm[REG_EBX], REG_EBX},
                {this->GenaytykRegs2Llvm[REG_ECX], REG_ECX},
                {this->GenaytykRegs2Llvm[REG_EDX], REG_EDX},
                {this->GenaytykRegs2Llvm[REG_ESP], REG_ESP},
                {this->GenaytykRegs2Llvm[REG_EBP], REG_EBP},
                {this->GenaytykRegs2Llvm[REG_0x24], REG_0x24},
                {this->GenaytykRegs2Llvm[REG_0x28], REG_0x28},
                {this->GenaytykRegs2Llvm[REG_0x2C], REG_0x2C},
                {this->GenaytykRegs2Llvm[REG_0x30], REG_0x30},
                {this->GenaytykRegs2Llvm[REG_0x34], REG_0x34},
                {this->GenaytykRegs2Llvm[REG_0x38], REG_0x38},
                {this->GenaytykRegs2Llvm[REG_AL], REG_AL},
                {this->GenaytykRegs2Llvm[REG_AH], REG_AH},
                {this->GenaytykRegs2Llvm[REG_BL], REG_BL},
                {this->GenaytykRegs2Llvm[REG_BH], REG_BH},
                {this->GenaytykRegs2Llvm[REG_CL], REG_CL},
                {this->GenaytykRegs2Llvm[REG_CH], REG_CH},
                {this->GenaytykRegs2Llvm[REG_DL], REG_DL},
                {this->GenaytykRegs2Llvm[REG_DH], REG_DH},
                {this->GenaytykRegs2Llvm[REG_AX], REG_AX},
                {this->GenaytykRegs2Llvm[REG_BX], REG_BX},
                {this->GenaytykRegs2Llvm[REG_CX], REG_CX},
                {this->GenaytykRegs2Llvm[REG_DX], REG_DX},
                {this->GenaytykRegs2Llvm[REG_SP], REG_SP},
                {this->GenaytykRegs2Llvm[REG_BP], REG_BP},
                {this->GenaytykRegs2Llvm[REG_DI], REG_DI},
                {this->GenaytykRegs2Llvm[REG_SI], REG_SI}};

        this->llvm2GenaytykRegs = std::move(l2r);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRegistersParentMapToOther(
        const std::vector<genaytyk_reg> &rs,
        genaytyk_reg other)
    {
        for (auto r : rs)
        {
            if (r >= reg2parentMap.size())
            {
                throw Genaytyk_Exception("Register out of range.");
            }
            reg2parentMap[r] = other;
        }
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRegistersParentMap32()
    {
        std::vector<std::vector<genaytyk_reg>> rss =
            {
                {REG_AH, REG_AL, REG_AX, REG_EAX},
                {REG_CH, REG_CL, REG_CX, REG_ECX},
                {REG_DH, REG_DL, REG_DX, REG_EDX},
                {REG_BH, REG_BL, REG_BX, REG_EBX},
                {REG_SP, REG_ESP},
                {REG_BP, REG_EBP},
                {REG_SI, REG_ESI},
                {REG_DI, REG_EDI},
                {REG_0x24},
                {REG_0x28},
                {REG_0x2C},
                {REG_0x30},
                {REG_0x34},
                {REG_0x38},
                {REG_EIP},
            };

        for (std::vector<genaytyk_reg> &rs : rss)
        {
            this->initializeRegistersParentMapToOther(rs, rs.back());
        }
    }

    static std::vector<std::string> funArgs = {"l", "r"};

    void setFuncArgs(llvm::Function *func, std::vector<std::string> funArgs)
    {
        unsigned idx = 0;
        llvm::Function::arg_iterator ai, ae;

        if (func->arg_size() != funArgs.size())
            return;

        for (ai = func->arg_begin(), ae = func->arg_end();
             ai != ae;
             ++ai, ++idx)
        {
            ai->setName(funArgs[idx]);
        }
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeFunctions(llvm::IRBuilder<> &irbuilder)
    {
        this->initializeRor(irbuilder);
        this->initializeRol(irbuilder);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRor(llvm::IRBuilder<> &irbuilder)
    {
        // create vector for arguments
        // this will be 2 integers
        std::vector<llvm::Type *> integers(funArgs.size(), llvm::Type::getInt32Ty(this->module->getContext()));

        llvm::FunctionType *funcType = llvm::FunctionType::get(
            /*Type=*/llvm::IntegerType::getInt32Ty(this->module->getContext()), // return type (int 32)
            /*Params=*/integers,
            /*isVarArg=*/false);

        auto *ror_function = llvm::Function::Create(
            /*FunctionType=*/funcType,
            /*LinkageTypes=*/llvm::Function::ExternalLinkage, /* function can be accessed from external file */
            /*Twine=*/"ror",
            /*Module=*/this->module);

        setFuncArgs(ror_function, funArgs);

        auto *entry = llvm::BasicBlock::Create(
            this->module->getContext(),
            "entry_ror",
            ror_function);

        irbuilder.SetInsertPoint(entry);

        // get arguments
        llvm::Function::arg_iterator AI = ror_function->arg_begin();
        llvm::Value *l = llvm::dyn_cast<llvm::Value>(AI++);
        llvm::Value *r = llvm::dyn_cast<llvm::Value>(AI);

        unsigned l_bitwidth = llvm::cast<llvm::IntegerType>(l)->getBitWidth();

        auto *v1 = irbuilder.CreateMul(irbuilder.getInt32(32), irbuilder.getInt32(l_bitwidth));
        auto *mask = irbuilder.CreateSub(v1, irbuilder.getInt32(1));

        auto *count = irbuilder.CreateAnd(r, mask);

        auto *minus_count = irbuilder.CreateNeg(count);
        auto *minus_count_and_mask = irbuilder.CreateAnd(minus_count, mask);

        auto *value_shr = irbuilder.CreateLShr(l, count);
        auto *value_shl = irbuilder.CreateShl(l, minus_count_and_mask);

        auto *ror = irbuilder.CreateOr(value_shr, value_shl);

        irbuilder.CreateRet(ror);

        this->ror_function = ror_function;
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::initializeRol(llvm::IRBuilder<> &irbuilder)
    {
        // create vector for arguments
        // this will be 2 integers
        std::vector<llvm::Type *> integers(funArgs.size(), llvm::Type::getInt32Ty(this->module->getContext()));

        llvm::FunctionType *funcType = llvm::FunctionType::get(
            /*Type=*/llvm::IntegerType::getInt32Ty(this->module->getContext()), // return type (int 32)
            /*Params=*/integers,
            /*isVarArg=*/false);

        auto *rol_function = llvm::Function::Create(
            /*FunctionType=*/funcType,
            /*LinkageTypes=*/llvm::Function::ExternalLinkage, /* function can be accessed from external file */
            /*Twine=*/"rol",
            /*Module=*/this->module);

        setFuncArgs(rol_function, funArgs);

        auto *entry = llvm::BasicBlock::Create(
            this->module->getContext(),
            "entry_rol",
            rol_function);

        irbuilder.SetInsertPoint(entry);

        // get arguments
        llvm::Function::arg_iterator AI = rol_function->arg_begin();
        llvm::Value *l = llvm::dyn_cast<llvm::Value>(AI++);
        llvm::Value *r = llvm::dyn_cast<llvm::Value>(AI);

        unsigned l_bitwidth = llvm::cast<llvm::IntegerType>(l)->getBitWidth();

        auto *v1 = irbuilder.CreateMul(irbuilder.getInt32(32), irbuilder.getInt32(l_bitwidth));
        auto *mask = irbuilder.CreateSub(v1, irbuilder.getInt32(1));

        auto *count = irbuilder.CreateAnd(r, mask);

        auto *minus_count = irbuilder.CreateNeg(count);
        auto *minus_count_and_mask = irbuilder.CreateAnd(minus_count, mask);

        auto *value_shl = irbuilder.CreateShl(l, count);
        auto *value_shr = irbuilder.CreateLShr(l, minus_count_and_mask);

        auto *rol = irbuilder.CreateOr(value_shl, value_shr);

        irbuilder.CreateRet(rol);

        this->rol_function = rol_function;
    }
} // namespace genaytyk