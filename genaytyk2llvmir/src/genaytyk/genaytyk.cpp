#include "genaytyk_impl.h"

#include "genaytyk_exception.h"

namespace genaytyk
{

    //
    //==============================================================================
    // Create variables implementation.
    //==============================================================================
    //
    /**
    *   Linkage determines if multiple declarations of same
    *   object refer to same object or separate ones.
    *
    *   - ExternalLinkage: externally visible function
    *   - AvailableExternallyLinkage: available for inspection, not emission.
    *   - LinkOnceAnyLinkage: keep one copy of function when linking (inline).
    *   - LinkOnceODRLinkage: Same, but only replaced by something equivalent.
    *   - WeakAnyLinkage: Keep one copy of named function when linking.
    *   - WeakODRLinkage: Same, but only replaced by something equivalent.
    *   - AppendingLinkage: Special purpose, only applies to global arrays.
    *   - InternalLinkage: Rename collisions when linking (static functions).
    *   - PrivateLinkage: Like internal, but omit from symbol table.
    *   - ExternalWeakLinkage: ExternalWeak linkage description.
    *   - CommonLinkage: Tentative definitions.
    */
    llvm::GlobalVariable *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateGlobal(llvm::Type *type, std::string name)
    {
        llvm::GlobalVariable *gVar = nullptr;

        this->module->getOrInsertGlobal(name, type);
        gVar = this->module->getNamedGlobal(name);
        gVar->setLinkage(llvm::GlobalValue::PrivateLinkage);
        gVar->setAlignment(4);

        return gVar;
    }

    llvm::Function *GenaytykLlvmIrTranslatorGenaytyk_impl::createFunc(llvm::IRBuilder<> &irbuilder, std::string name, llvm::ArrayRef<llvm::Type *> params, llvm::Type *return_type)
    {
        llvm::FunctionType *funcType = llvm::FunctionType::get(
            return_type,
            params,
            false);

        llvm::Function *function = llvm::Function::Create(
            funcType,
            llvm::Function::ExternalLinkage,
            name,
            this->module);

        return function;
    }

    llvm::BasicBlock *GenaytykLlvmIrTranslatorGenaytyk_impl::createBB(llvm::Function *parentFunction, std::string name)
    {
        return llvm::BasicBlock::Create(
            this->module->getContext(),
            name,
            parentFunction);
    }

    //
    //==============================================================================
    // Load/store methods.
    //==============================================================================
    //

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::loadRegister(uint32_t reg_, llvm::IRBuilder<> &irbuilder, llvm::Type *dstType)
    {
        if (reg_ >= REG_ERROR)
        {
            return nullptr;
        }

        auto *reg_type = getRegisterType(reg_);
        auto parent_reg = getParentRegister(reg_);
        auto *reg = getRegister(parent_reg);
        if (reg == nullptr)
        {
            throw Genaytyk_Exception("GenaytykLlvmIrTranslatorGenaytyk_impl::loadRegister unhandled register");
        }

        llvm::Value *ret;
        ret = irbuilder.CreateLoad(reg);

        if (reg_ != parent_reg) // if no parent, we have to fix
        {
            // if the register is one of the high nibble of
            // byte, shift to the right the register
            if (reg_ == REG_AH ||
                reg_ == REG_BH ||
                reg_ == REG_CH ||
                reg_ == REG_DH)
            {
                ret = irbuilder.CreateLShr(ret, 8);
            }

            ret = irbuilder.CreateTrunc(ret, reg_type);
        }

        ret = generateTypeConversion(irbuilder, ret, dstType);

        return ret;
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::load(llvm::Value *addr, llvm::IRBuilder<> &irbuilder, llvm::Type *dstType)
    {
        llvm::Value *ret;

        ret = irbuilder.CreateLoad(addr);

        ret = irbuilder.CreateTrunc(ret, dstType);

        return ret;
    }

    llvm::StoreInst *GenaytykLlvmIrTranslatorGenaytyk_impl::storeRegister(uint32_t reg_, llvm::IRBuilder<> &irbuilder, llvm::Value *val)
    {
        if (reg_ >= REG_ERROR)
        {
            return nullptr;
        }

        auto *reg_type = getRegisterType(reg_);
        auto parent_reg = getParentRegister(reg_);
        auto *reg = getRegister(parent_reg);

        if (reg == nullptr)
        {
            throw Genaytyk_Exception("GenaytykLlvmIrTranslatorGenaytyk_impl::storeRegister unhandled register");
        }

        llvm::StoreInst *ret;

        if (reg_type->isIntegerTy() && val->getType()->isIntegerTy())
        {
            auto *valT = llvm::cast<llvm::IntegerType>(val->getType());

            // our registers are always integer types
            // in opposite to x86
            auto *it = llvm::dyn_cast<llvm::IntegerType>(getRegisterType(reg_));
            if (valT->getBitWidth() > it->getBitWidth())
            {
                val = irbuilder.CreateTrunc(val, reg->getValueType());
            }
        }

        val = generateTypeConversion(irbuilder, val, reg->getValueType());

        // now check in which register is going to store
        if (reg_ == parent_reg) // if it's the parent reg, store is simple
        {
            ret = irbuilder.CreateStore(val, reg);
        }
        else
        {
            // in other case, it's necessary to preserve
            // values from other parts
            llvm::Value *l = irbuilder.CreateLoad(reg);

            if (!(l->getType()->isIntegerTy(16) || l->getType()->isIntegerTy(32) || l->getType()->isIntegerTy(64)))
            {
                throw Genaytyk_Exception("Unexpected parent type.");
            }

            llvm::Value *andC = nullptr;
            if (reg_ == REG_AH ||
                reg_ == REG_BH ||
                reg_ == REG_CH ||
                reg_ == REG_DH)
            {
                // if it's the high nibble
                // of the byte, needs to store
                // low part
                if (l->getType()->isIntegerTy(16))
                {
                    andC = irbuilder.getInt16(0x00ff);
                }
                else if (l->getType()->isIntegerTy(32))
                {
                    andC = irbuilder.getInt32(0xffff00ff);
                }
                else if (l->getType()->isIntegerTy(64))
                {
                    andC = irbuilder.getInt64(0xffffffffffff00ff);
                }
                // val must be applied to high part of nibble
                val = irbuilder.CreateShl(val, 8);
            }
            else if (reg_type->isIntegerTy(8))
            {
                if (l->getType()->isIntegerTy(16))
                {
                    andC = irbuilder.getInt16(0xff00);
                }
                else if (l->getType()->isIntegerTy(32))
                {
                    andC = irbuilder.getInt32(0xffffff00);
                }
                else if (l->getType()->isIntegerTy(64))
                {
                    andC = irbuilder.getInt64(0xffffffffffffff00);
                }
            }
            else if (reg_type->isIntegerTy(16))
            {
                if (l->getType()->isIntegerTy(32))
                {
                    andC = irbuilder.getInt32(0xffff0000);
                }
                else if (l->getType()->isIntegerTy(64))
                {
                    andC = irbuilder.getInt64(0xffffffffffff0000);
                }
            }
            else if (reg_type->isIntegerTy(32))
            {
                if (l->getType()->isIntegerTy(64))
                {
                    andC = irbuilder.getInt64(0xffffffff00000000);
                }
            }

            if (andC == nullptr)
            {
                throw Genaytyk_Exception("Mask not initialized in storeRegister().");
            }

            l = irbuilder.CreateAnd(l, andC);

            auto *o = irbuilder.CreateOr(l, val);
            ret = irbuilder.CreateStore(o, reg);
        }

        return ret;
    }

    llvm::StoreInst *GenaytykLlvmIrTranslatorGenaytyk_impl::store(llvm::Value *addr, llvm::IRBuilder<> &irbuilder, llvm::Value *val)
    {
        return irbuilder.CreateStore(val, addr);
    }

    //
    //==============================================================================
    // Genaytyk instruction translation methods.
    //==============================================================================
    //
    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateAdd(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateAdd(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateSub(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateSub(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateImul(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateMul(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateIdiv(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateUDiv(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateOr(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateOr(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateXor(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateXor(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateAnd(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateAnd(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateInc(llvm::Value *v, llvm::IRBuilder<> &irbuilder)
    {

        if (v->getType()->isIntegerTy(8))
            return irbuilder.CreateAdd(v, irbuilder.getInt8(1));
        else if (v->getType()->isIntegerTy(16))
            return irbuilder.CreateAdd(v, irbuilder.getInt16(1));
        else
            return irbuilder.CreateAdd(v, irbuilder.getInt32(1));
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateDec(llvm::Value *v, llvm::IRBuilder<> &irbuilder)
    {
        if (v->getType()->isIntegerTy(8))
            return irbuilder.CreateSub(v, irbuilder.getInt8(1));
        else if (v->getType()->isIntegerTy(16))
            return irbuilder.CreateSub(v, irbuilder.getInt16(1));
        else
            return irbuilder.CreateSub(v, irbuilder.getInt32(1));
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateNot(llvm::Value *v, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateNot(v);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateShr(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateLShr(l, r);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateShl(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateShl(l, r);
    }

    /// Not the best implementation
    /// but maybe the enough for this
    /// translator
    /// taken from: https://en.wikipedia.org/wiki/Circular_shift#Implementing_circular_shifts
    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateRor(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        auto *ror = irbuilder.CreateCall(ror_function, {l, r});
        return ror;
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translateRol(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder)
    {
        auto *rol = irbuilder.CreateCall(rol_function, {l, r});
        return rol;
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::translatePushad(llvm::IRBuilder<> &irbuilder)
    {
        irbuilder.CreateCall(pushad);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::translatePopad(llvm::IRBuilder<> &irbuilder)
    {
        irbuilder.CreateCall(popad);
    }

    void GenaytykLlvmIrTranslatorGenaytyk_impl::translatePush(llvm::Value *val, llvm::IRBuilder<> &irbuilder)
    {
        auto *pt = llvm::PointerType::get(irbuilder.getInt32Ty(), 0);
        auto *sp = this->loadRegister(REG_ESP, irbuilder, this->getRegisterType(REG_ESP));
        llvm::Value *c = irbuilder.getInt32(-4);

        auto *a0 = sp;
        auto *a1 = irbuilder.CreateAdd(a0, c);

        irbuilder.CreateStore(val, irbuilder.CreateIntToPtr(a1, pt));
        this->storeRegister(REG_ESP, irbuilder, a1);
    }

    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::translatePop(llvm::IRBuilder<> &irbuilder)
    {
        auto *pt = llvm::PointerType::get(irbuilder.getInt32Ty(), 0);
        auto *sp = this->loadRegister(REG_ESP, irbuilder, this->getRegisterType(REG_ESP));
        llvm::Value *c = irbuilder.getInt32(4);

        auto *a1 = sp;
        auto *a2 = irbuilder.CreateAdd(a1, c);

        llvm::Value *ret = irbuilder.CreateLoad(irbuilder.CreateIntToPtr(a1, pt));
        this->storeRegister(REG_ESP, irbuilder, a2);

        return ret;
    }
    //
    //==============================================================================
    // Genaytyk branching instruction translation methods.
    //==============================================================================
    //
    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJMP(llvm::BasicBlock *destination, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateBr(destination);
    }

    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJZ(llvm::Value *l, llvm::Value *r, llvm::BasicBlock *destination, llvm::BasicBlock *next_addr, llvm::IRBuilder<> &irbuilder)
    {
        llvm::Value *jz_condition = irbuilder.Insert(new llvm::ICmpInst(llvm::ICmpInst::ICMP_EQ, l, r), "ifcond");

        return irbuilder.CreateCondBr(jz_condition, destination, next_addr);
    }

    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJNZ(llvm::Value *l, llvm::Value *r, llvm::BasicBlock *destination, llvm::BasicBlock *next_addr, llvm::IRBuilder<> &irbuilder)
    {
        llvm::Value *jnz_condition = irbuilder.Insert(new llvm::ICmpInst(llvm::ICmpInst::ICMP_NE, l, r), "ifcond");

        return irbuilder.CreateCondBr(jnz_condition, destination, next_addr);
    }

    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJA(llvm::Value *l, llvm::Value *r, llvm::BasicBlock *destination, llvm::BasicBlock *next_addr, llvm::IRBuilder<> &irbuilder)
    {
        llvm::Value *ja_condition = irbuilder.Insert(new llvm::ICmpInst(llvm::ICmpInst::ICMP_UGT, l, r), "ifcond");

        return irbuilder.CreateCondBr(ja_condition, destination, next_addr);
    }

    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJB(llvm::Value *l, llvm::Value *r, llvm::BasicBlock *destination, llvm::BasicBlock *next_addr, llvm::IRBuilder<> &irbuilder)
    {
        llvm::Value *jb_condition = irbuilder.Insert(new llvm::ICmpInst(llvm::ICmpInst::ICMP_ULT, l, r), "ifcond");

        return irbuilder.CreateCondBr(jb_condition, destination, next_addr);
    }

    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJNB(llvm::Value *l, llvm::Value *r, llvm::BasicBlock *destination, llvm::BasicBlock *next_addr, llvm::IRBuilder<> &irbuilder)
    {
        llvm::Value *jnb_condition = irbuilder.Insert(new llvm::ICmpInst(llvm::ICmpInst::ICMP_UGE, l, r), "ifcond"); // jnb == jae

        return irbuilder.CreateCondBr(jnb_condition, destination, next_addr);
    }

    llvm::BranchInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateJBE(llvm::Value *l, llvm::Value *r, llvm::BasicBlock *destination, llvm::BasicBlock *next_addr, llvm::IRBuilder<> &irbuilder)
    {
        llvm::Value *jbe_condition = irbuilder.Insert(new llvm::ICmpInst(llvm::ICmpInst::ICMP_ULE, l, r), "ifcond");

        return irbuilder.CreateCondBr(jbe_condition, destination, next_addr);
    }

    llvm::CallInst *GenaytykLlvmIrTranslatorGenaytyk_impl::translateCreateCall(llvm::Function *callee, llvm::IRBuilder<> &irbuilder)
    {
        return irbuilder.CreateCall(callee);
    }
    //
    //==============================================================================
    // getters
    //==============================================================================
    //
    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::getPointerFromAddress(llvm::IRBuilder<> irbuilder, llvm::Type *array_type, llvm::Value *base, llvm::Value *offset, llvm::Type* dest_type)
    {
        llvm::Value *gep = irbuilder.CreateGEP(
            array_type,
            base,
            offset);

        return irbuilder.CreateBitCast(gep, dest_type);
    }

    llvm::GlobalVariable *GenaytykLlvmIrTranslatorGenaytyk_impl::getRegister(uint32_t r)
    {
        auto fIt = GenaytykRegs2Llvm.find(r);
        return fIt != GenaytykRegs2Llvm.end() ? fIt->second : nullptr;
    }

    llvm::Type *GenaytykLlvmIrTranslatorGenaytyk_impl::getRegisterType(uint32_t r)
    {
        auto fIt = reg2type.find(r);
        if (fIt == reg2type.end())
        {
            throw Genaytyk_Exception(
                "Missing type for register number: " + std::to_string(r));
        }
        return fIt->second;
    }

    uint32_t GenaytykLlvmIrTranslatorGenaytyk_impl::getParentRegister(uint32_t r)
    {
        return r < reg2parentMap.size()
                   ? (reg2parentMap[r] != REG_ERROR ? reg2parentMap[r] : r)
                   : r;
    }

    //
    //==============================================================================
    // Utilities
    //==============================================================================
    //
    llvm::Value *GenaytykLlvmIrTranslatorGenaytyk_impl::generateTypeConversion(llvm::IRBuilder<> &irb,
                                                                               llvm::Value *from,
                                                                               llvm::Type *to)
    {
        if (to == nullptr || from->getType() == to)
        {
            return from;
        }

        llvm::Value *ret = nullptr;

        if (!to->isIntegerTy())
        {
            throw Genaytyk_Exception("Invalid combination of conversion method and destination type");
        }

        if (from->getType()->isIntegerTy())
        {
            ret = irb.CreateZExtOrTrunc(from, to);
        }
        else
        {
            auto size = this->module->getDataLayout().getTypeStoreSizeInBits(from->getType());
            auto intTy = irb.getIntNTy(size);
            ret = irb.CreateBitCast(from, intTy);
            ret = irb.CreateZExtOrTrunc(ret, to);
        }

        return ret;
    }
} // namespace genaytyk