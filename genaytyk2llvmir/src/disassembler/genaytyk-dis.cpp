#include "genaytyk-dis.h"
#include "disassembler-exception.h"

namespace genaytyk
{
    namespace disassembler
    {
        Genaytyk_Disassembler::Genaytyk_Disassembler(uint8_t *p_to_code, size_t size_of_code, llvm::Module *module, llvm::IRBuilder<> &irbuilder) : index(0),
                                                                                                                                                    call_jmp(false),
                                                                                                                                                    jmp_op(false),
                                                                                                                                                    is_ret(false),
                                                                                                                                                    is_in_call(false),
                                                                                                                                                    ret(0),
                                                                                                                                                    function_name(""),
                                                                                                                                                    state(SETUP)
        {
            this->p_to_code = p_to_code;
            this->size_of_code = size_of_code;
            this->module = module;
            // as we miss std::make_unique we use the one from llvm
            this->genaytyk_translator = llvm::make_unique<GenaytykLlvmIrTranslatorGenaytyk_impl>(module, irbuilder);
            this->initialize();
        }

        void Genaytyk_Disassembler::get_jump_address(llvm::IRBuilder<> &irbuilder)
        {
            std::vector<uint8_t> operandStruct;
            std::vector<std::map<std::string, uint8_t>> mapsOfOperands;
            this->state = SETUP;
            while (true)
            {
                if (this->state == SETUP)
                {
                    this->index = 0;
                    this->state = GET_OPERATION;
                    continue;
                }
                else if (this->state == INIT)
                {
                    if (this->index >= this->size_of_code) // || (this->index >= until))
                    {
                        break;
                    }

                    this->state = GET_OPERATION;

                    continue;
                }
                else if (this->state == GET_OPERATION)
                {
                    this->call_jmp = false;
                    this->is_ret = false;
                    this->jmp_op = false;

                    switch (this->p_to_code[index])
                    {
                    case RET:
                        index++;
                        this->is_ret = true;
                        this->state = END_NORMAL;
                        continue;
                    case NOP:
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case PUSHAD:
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case POPAD:
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case ERROR:
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case CALL:
                        this->call_jmp = true;
                        break;
                    case JMP:
                        this->jmp_op = true;
                        break;
                    default:
                        break;
                    }

                    std::vector<uint8_t> two_opcodes = {
                        this->p_to_code[index],
                        this->p_to_code[index + 1]};

                    index += 2;

                    operandStruct = this->getOperandStruct(two_opcodes[1]);
                    mapsOfOperands = this->getMapsFromOperandStruct(operandStruct);

                    if ((two_opcodes[0] >= JZ) && (two_opcodes[0] <= JBE))
                    {
                        this->state = GET_JC_OPERANDS;
                    }
                    else
                    {
                        this->state = GET_OPERANDS;
                    }
                }
                else if (this->state == GET_JC_OPERANDS)
                {
                    uint32_t offsets_to_jump;

                    // prepare destination to jump
                    if (mapsOfOperands[0]["type"] == IMMEDIATE)
                    {
                        offsets_to_jump = this->getImmediateValue(mapsOfOperands[0]["size"]);
                    }

                    for (size_t i = 1; i < mapsOfOperands.size(); i++)
                    {
                        switch (mapsOfOperands[i]["type"]) // check operand types
                        {
                        case IMMEDIATE:
                            getImmediateValue(mapsOfOperands[i]["size"]);
                            break;
                        case REGISTER:
                            // check and get register from the opcode
                            index++;
                            break;
                        case ADDRESS:
                            index++;
                            break;
                        case SERIAL_HASH:
                            getImmediateValue(mapsOfOperands[i]["size"]);
                            break;
                        default:
                            break;
                        }
                    }

                    // get addresses where to jump
                    basic_block_addresses.push_back(index);
                    basic_block_addresses.push_back(offsets_to_jump);

                    this->state = END_NORMAL;
                }
                else if (this->state == GET_OPERANDS)
                {
                    for (size_t i = 0; i < mapsOfOperands.size(); i++)
                    {
                        if (mapsOfOperands[i]["type"] == IMMEDIATE)
                        {
                            uint32_t offset = getImmediateValue(mapsOfOperands[i]["size"]);
                            if (jmp_op)
                            {
                                basic_block_addresses.push_back(offset);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == REGISTER)
                        {
                            index++;
                        }
                        else if (mapsOfOperands[i]["type"] == ADDRESS)
                        {
                            index++;
                        }
                        else if (mapsOfOperands[i]["type"] == SERIAL_HASH)
                        {
                            getImmediateValue(mapsOfOperands[i]["size"]);
                        }
                        else
                        {
                            throw Disassembler_Exception("Error type not supported\n");
                        }
                    }

                    this->state = END_NORMAL;
                    continue;
                }
                else if (state == END_NORMAL)
                {
                    if (is_ret)
                    {
                        is_in_call = false;
                    }

                    state = INIT;

                    continue;
                }
            }
        }

        void Genaytyk_Disassembler::disassemble(llvm::IRBuilder<> &irbuilder)
        {
            llvm::Function *current_function;
            llvm::BasicBlock *current_basic_block;
            std::vector<uint8_t> operandStruct;
            std::vector<std::map<std::string, uint8_t>> mapsOfOperands;
            instructions instruction;

            llvm::Value *l = nullptr;
            llvm::Value *r = nullptr;
            bool l_is_register = false;
            uint8_t l_register = 0;

            this->state = SETUP;

            // declare types that will use later
            llvm::Type *hardcodedType = llvm::ArrayType::get(irbuilder.getInt8Ty(), strlen("aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w") + 1);

            llvm::PointerType *i32_ptr = llvm::PointerType::get(irbuilder.getInt32Ty(), 0);
            llvm::PointerType *i8_ptr = llvm::PointerType::get(irbuilder.getInt8Ty(), 0);
            llvm::PointerType *i16_ptr = llvm::PointerType::get(irbuilder.getInt16Ty(), 0);

            while (true)
            {
                if (this->state == SETUP)
                {
                    this->initializeGlobalData(irbuilder);
                    get_jump_address(irbuilder);
                    this->index = 0;
                    // create the main function
                    current_function = this->genaytyk_translator->createFunc(irbuilder, "main", {}, irbuilder.getVoidTy());
                    this->addr2llvmfunc[index] = current_function;

                    this->state = GET_OPERATION;
                    continue;
                }
                else if (this->state == INIT)
                {
                    if (this->index >= this->size_of_code)
                    {
                        break;
                    }

                    l = nullptr;
                    r = nullptr;
                    l_is_register = false;
                    l_register = 0;

                    this->state = GET_OPERATION;

                    continue;
                }
                else if (this->state == GET_OPERATION)
                {
                    llvm::BasicBlock *basic_block;
                    auto it = this->addr2llvmfunc.find(index);

                    printf("Disassembling the instruction: %x\n", index);

                    if (it != this->addr2llvmfunc.end())
                    {
                        // function is going to start, so create a basic block for it
                        this->is_in_call = true;
                        current_function = it->second;

                        auto *bb_entry = this->genaytyk_translator->createBB(current_function, "entry");
                        irbuilder.SetInsertPoint(bb_entry);
                        current_basic_block = bb_entry;
                    }
                    // now create basic block for new instruction
                    // and write instruction in there

                    if (std::find(this->basic_block_addresses.begin(), this->basic_block_addresses.end(), index) != this->basic_block_addresses.end())
                    {
                        // if address is one of the previous obtained offsets

                        if (this->addr2llvmbb.find(index) == this->addr2llvmbb.end())
                        {
                            basic_block = this->genaytyk_translator->createBB(current_function, std::to_string(index));
                            this->addr2llvmbb[index] = basic_block;
                        }
                        else
                        {
                            basic_block = this->addr2llvmbb[index];

                            // if basic block exists, probably it was created for a jump,
                            // move it to current position
                            if (current_basic_block != nullptr)
                                basic_block->moveAfter(current_basic_block); // current basic block is always the previous one
                        }

                        /*
                        *   Due to the meaning of what is a basic block
                        *   as a block of code that finish with a control
                        *   flow modification instruction, it's not correct
                        *   to create a basic block that does not finish
                        *   as this, but it's possible to jump to the middle
                        *   of a basic block in Assembly, so just create
                        *   a fake end of basic block with a jump to the next
                        *   instructions.
                        */
                        if (instruction < JMP || instruction > CALL)
                        {
                            this->genaytyk_translator->translateCreateJMP(basic_block, irbuilder);
                        }

                        irbuilder.SetInsertPoint(basic_block);
                        current_basic_block = basic_block; // to set correctly the basic blocks on jumps
                    }

                    this->call_jmp = false;
                    this->is_ret = false;
                    this->jmp_op = false;

                    switch (this->p_to_code[index])
                    {
                    case RET:
                        irbuilder.CreateRet(nullptr);
                        index++;
                        this->is_ret = true;
                        this->state = END_NORMAL;
                        continue;
                    case NOP:
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case PUSHAD:
                        this->genaytyk_translator->translatePushad(irbuilder);
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case POPAD:
                        this->genaytyk_translator->translatePopad(irbuilder);
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case ERROR:
                        irbuilder.CreateRet(nullptr);
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case CALL:
                        this->call_jmp = true;
                        break;
                    case JMP:
                        this->jmp_op = true;
                        break;
                    default:
                        break;
                    }

                    std::vector<uint8_t> two_opcodes = {
                        this->p_to_code[index],
                        this->p_to_code[index + 1]};

                    index += 2;

                    instruction = static_cast<instructions>(two_opcodes[0]);

                    operandStruct = this->getOperandStruct(two_opcodes[1]);
                    mapsOfOperands = this->getMapsFromOperandStruct(operandStruct);

                    if ((two_opcodes[0] >= JZ) && (two_opcodes[0] <= JBE))
                    {
                        this->state = GET_JC_OPERANDS;
                    }
                    else
                    {
                        this->state = GET_OPERANDS;
                    }
                }
                else if (this->state == GET_JC_OPERANDS)
                {
                    uint32_t offsets_to_jump;
                    llvm::BasicBlock *next_addr = nullptr;
                    llvm::BasicBlock *jump_addr = nullptr;

                    // prepare destination to jump
                    if (mapsOfOperands[0]["type"] == IMMEDIATE)
                    {
                        offsets_to_jump = this->getImmediateValue(mapsOfOperands[0]["size"]);
                    }

                    for (size_t i = 1; i < mapsOfOperands.size(); i++)
                    {
                        if (mapsOfOperands[i]["type"] == IMMEDIATE)
                        {
                            auto size = mapsOfOperands[i]["size"];
                            uint32_t offset = getImmediateValue(size);
                            if (l == nullptr)
                            {
                                if (size == 1)
                                    l = irbuilder.getInt8(offset);
                                else if (size == 2)
                                    l = irbuilder.getInt16(offset);
                                else
                                    l = irbuilder.getInt32(offset);
                            }

                            else if (r == nullptr)
                            {
                                if (size == 1)
                                    r = irbuilder.getInt8(offset);
                                else if (size == 2)
                                    r = irbuilder.getInt16(offset);
                                else
                                    r = irbuilder.getInt32(offset);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == REGISTER)
                        {
                            // check and get register from the opcode
                            llvm::Type *reg_type = this->genaytyk_translator->getRegisterType(p_to_code[index]);
                            uint8_t reg = p_to_code[index];
                            index++;

                            if (l == nullptr)
                            {
                                l_is_register = true;
                                l_register = p_to_code[index];
                                l = this->genaytyk_translator->loadRegister(reg, irbuilder, reg_type);
                            }
                            else if (r == nullptr)
                            {
                                r = this->genaytyk_translator->loadRegister(reg, irbuilder, reg_type);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == ADDRESS)
                        {
                            auto *reg_type = this->genaytyk_translator->getRegisterType(p_to_code[index]);
                            llvm::PointerType *ptr_type;
                            llvm::Type *int_type;
                            auto *reg_value = this->genaytyk_translator->loadRegister(p_to_code[index], irbuilder, reg_type);
                            index++;

                            if (l == nullptr)
                            {
                                if (i + 1 >= mapsOfOperands.size()) // if it's just one operand
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 1) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(8)))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 2) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(16)))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     reg_value,
                                                                                     ptr_type);

                                l = this->genaytyk_translator->load(l, irbuilder, int_type);
                            }
                            else if (r == nullptr)
                            {
                                if (l->getType()->isIntegerTy(8))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if (l->getType()->isIntegerTy(16))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     reg_value,
                                                                                     ptr_type);
                                r = this->genaytyk_translator->load(r, irbuilder, int_type);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == SERIAL_HASH)
                        {
                            auto *immediate_value = irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"]));
                            llvm::PointerType *ptr_type;
                            llvm::Type *int_type;

                            if (l == nullptr)
                            {
                                if (i + 1 >= mapsOfOperands.size()) // if it's just one operand
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 1) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(8)))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 2) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(16)))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     immediate_value,
                                                                                     ptr_type);
                                l = this->genaytyk_translator->load(l, irbuilder, int_type);
                            }
                            else if (r == nullptr)
                            {
                                if (l->getType()->isIntegerTy(8))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if (l->getType()->isIntegerTy(16))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     immediate_value,
                                                                                     ptr_type);
                                r = this->genaytyk_translator->load(r, irbuilder, int_type);
                            }
                        }
                    }

                    // now create the basic block for the next address
                    if (this->addr2llvmbb.find(index) == this->addr2llvmbb.end())
                    {
                        next_addr = this->genaytyk_translator->createBB(current_function, std::to_string(index));
                        this->addr2llvmbb[index] = next_addr;
                    }
                    else
                    {
                        next_addr = this->addr2llvmbb[index];
                    }

                    // create a basic block for the offset where to jump
                    if (this->addr2llvmbb.find(offsets_to_jump) == this->addr2llvmbb.end())
                    {
                        jump_addr = this->genaytyk_translator->createBB(current_function, std::to_string(offsets_to_jump));
                        this->addr2llvmbb[offsets_to_jump] = jump_addr;
                    }
                    else
                    {
                        jump_addr = this->addr2llvmbb[offsets_to_jump];
                    }

                    // now create the jump
                    switch (instruction)
                    {
                    case JZ:
                        this->genaytyk_translator->translateCreateJZ(l, r, jump_addr, next_addr, irbuilder);
                        break;
                    case JNZ:
                        this->genaytyk_translator->translateCreateJNZ(l, r, jump_addr, next_addr, irbuilder);
                        break;
                    case JA:
                        this->genaytyk_translator->translateCreateJA(l, r, jump_addr, next_addr, irbuilder);
                        break;
                    case JB:
                        this->genaytyk_translator->translateCreateJB(l, r, jump_addr, next_addr, irbuilder);
                        break;
                    case JNB:
                        this->genaytyk_translator->translateCreateJNB(l, r, jump_addr, next_addr, irbuilder);
                        break;
                    case JBE:
                        this->genaytyk_translator->translateCreateJBE(l, r, jump_addr, next_addr, irbuilder);
                        break;
                    default:
                        break;
                    }

                    this->state = END_NORMAL;
                }
                else if (this->state == GET_OPERANDS)
                {
                    llvm::Value *store_addr;

                    for (size_t i = 0; i < mapsOfOperands.size(); i++)
                    {
                        if (mapsOfOperands[i]["type"] == IMMEDIATE)
                        {
                            auto size = mapsOfOperands[i]["size"];
                            uint32_t offset = getImmediateValue(size);

                            if (call_jmp)
                            {
                                // it is a call check if function exists or create it
                                llvm::Function *callee;
                                if (this->addr2llvmfunc.find(offset) == this->addr2llvmfunc.end())
                                {
                                    callee = this->genaytyk_translator->createFunc(irbuilder, std::to_string(offset), {}, irbuilder.getVoidTy());
                                    this->addr2llvmfunc[offset] = callee;
                                }
                                else
                                {
                                    callee = this->addr2llvmfunc[offset];
                                }
                                // create a call to a function
                                this->genaytyk_translator->translateCreateCall(callee, irbuilder);
                            }
                            else if (jmp_op)
                            {
                                llvm::BasicBlock *jumped;
                                if (this->addr2llvmbb.find(offset) == this->addr2llvmbb.end())
                                {
                                    jumped = this->genaytyk_translator->createBB(current_function, std::to_string(offset));
                                    this->addr2llvmbb[offset] = jumped;
                                }
                                else
                                {
                                    jumped = this->addr2llvmbb[offset];
                                }
                                // create a jump to the address
                                this->genaytyk_translator->translateCreateJMP(jumped, irbuilder);
                            }
                            else
                            {
                                if (l == nullptr)
                                {
                                    if (size == 1)
                                        l = irbuilder.getInt8(offset);
                                    else if (size == 2)
                                        l = irbuilder.getInt16(offset);
                                    else
                                        l = irbuilder.getInt32(offset);
                                }

                                else if (r == nullptr)
                                {
                                    if (size == 1)
                                        r = irbuilder.getInt8(offset);
                                    else if (size == 2)
                                        r = irbuilder.getInt16(offset);
                                    else
                                        r = irbuilder.getInt32(offset);
                                }
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == REGISTER)
                        {
                            uint8_t opcodeRegister = this->p_to_code[index];
                            index++;
                            llvm::Type *typereg = this->genaytyk_translator->getRegisterType(opcodeRegister);
                            if (l == nullptr)
                            {
                                l_is_register = true;
                                l_register = opcodeRegister;
                                if (instruction == MOV)
                                    l = this->genaytyk_translator->getRegister(opcodeRegister);
                                else
                                    l = this->genaytyk_translator->loadRegister(opcodeRegister, irbuilder, typereg);
                                    
                            }
                            else if (r == nullptr)
                            {
                                r = this->genaytyk_translator->loadRegister(opcodeRegister, irbuilder, typereg);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == ADDRESS)
                        {
                            llvm::Type *reg_type = this->genaytyk_translator->getRegisterType(p_to_code[index]);
                            auto *reg_value = this->genaytyk_translator->loadRegister(p_to_code[index], irbuilder, reg_type);
                            llvm::PointerType *ptr_type;
                            llvm::Type *int_type;
                            index++;

                            if (l == nullptr)
                            {
                                if (i + 1 >= mapsOfOperands.size()) // if it's just one operand
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 1) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(8)))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 2) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(16)))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     reg_value,
                                                                                     ptr_type);

                                store_addr = l;
                                if (instruction != MOV)
                                    l = this->genaytyk_translator->load(l, irbuilder, int_type);
                            }
                            else if (r == nullptr)
                            {
                                if (l->getType()->isIntegerTy(8))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if (l->getType()->isIntegerTy(16))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     reg_value,
                                                                                     ptr_type);
                                r = this->genaytyk_translator->load(r, irbuilder, int_type);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == SERIAL_HASH)
                        {
                            auto *immediate_value = irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"]));
                            llvm::PointerType *ptr_type;
                            llvm::Type *int_type;

                            if (l == nullptr)
                            {
                                if (i + 1 >= mapsOfOperands.size()) // if it's just one operand
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 1) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(8)))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if ((mapsOfOperands[i + 1]["type"] == IMMEDIATE && mapsOfOperands[i + 1]["size"] == 2) ||
                                         (mapsOfOperands[i + 1]["type"] == REGISTER && this->genaytyk_translator->getRegisterType(this->p_to_code[index])->isIntegerTy(16)))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }
                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     immediate_value,
                                                                                     ptr_type);

                                store_addr = l;
                                if (instruction != MOV)
                                    l = this->genaytyk_translator->load(l, irbuilder, int_type);
                            }
                            else if (r == nullptr)
                            {
                                if (l->getType()->isIntegerTy(8))
                                {
                                    ptr_type = i8_ptr;
                                    int_type = irbuilder.getInt8Ty();
                                }
                                else if (l->getType()->isIntegerTy(16))
                                {
                                    ptr_type = i16_ptr;
                                    int_type = irbuilder.getInt16Ty();
                                }
                                else
                                {
                                    ptr_type = i32_ptr;
                                    int_type = irbuilder.getInt32Ty();
                                }

                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder,
                                                                                     hardcodedType,
                                                                                     llvm::dyn_cast<llvm::Value>(this->hardcodedString),
                                                                                     immediate_value,
                                                                                     ptr_type);
                                r = this->genaytyk_translator->load(r, irbuilder, int_type);
                            }
                        }
                        else
                        {
                            throw Disassembler_Exception("Error type not supported\n");
                        }
                    }

                    llvm::Value *result;
                    switch (instruction)
                    {
                    case MOV:
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, r);
                        }
                        break;
                    case ADD:
                        result = this->genaytyk_translator->translateAdd(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case SUB:
                        result = this->genaytyk_translator->translateSub(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case IMUL:
                        result = this->genaytyk_translator->translateImul(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case IDIV:
                        result = this->genaytyk_translator->translateIdiv(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case OR:
                        result = this->genaytyk_translator->translateOr(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case XOR:
                        result = this->genaytyk_translator->translateXor(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case AND:
                        result = this->genaytyk_translator->translateAnd(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case INC:
                        result = this->genaytyk_translator->translateInc(l, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case DEC:
                        result = this->genaytyk_translator->translateDec(l, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case NOT:
                        result = this->genaytyk_translator->translateNot(l, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case SHR:
                        if (!l->getType()->isIntegerTy(32))
                            l = irbuilder.CreateZExt(l, irbuilder.getInt32Ty());
                        if (!r->getType()->isIntegerTy(32))
                            r = irbuilder.CreateZExt(r, irbuilder.getInt32Ty());
                        result = this->genaytyk_translator->translateShr(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case SHL:
                        if (!l->getType()->isIntegerTy(32))
                            l = irbuilder.CreateZExt(l, irbuilder.getInt32Ty());
                        if (!r->getType()->isIntegerTy(32))
                            r = irbuilder.CreateZExt(r, irbuilder.getInt32Ty());
                        result = this->genaytyk_translator->translateShl(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case ROR:
                        if (!l->getType()->isIntegerTy(32))
                            l = irbuilder.CreateZExt(l, irbuilder.getInt32Ty());
                        if (!r->getType()->isIntegerTy(32))
                            r = irbuilder.CreateZExt(r, irbuilder.getInt32Ty());
                        result = this->genaytyk_translator->translateRor(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case ROL:
                        if (!l->getType()->isIntegerTy(32))
                            l = irbuilder.CreateZExt(l, irbuilder.getInt32Ty());
                        if (!r->getType()->isIntegerTy(32))
                            r = irbuilder.CreateZExt(r, irbuilder.getInt32Ty());
                        result = this->genaytyk_translator->translateRol(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    case PUSH:
                        this->genaytyk_translator->translatePush(l, irbuilder);
                        break;
                    case POP:
                        result = this->genaytyk_translator->translatePop(irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, result);
                        }
                        else
                        {
                            this->genaytyk_translator->store(store_addr, irbuilder, result);
                        }
                        break;
                    default:
                        break;
                    }

                    this->state = END_NORMAL;
                    continue;
                }
                else if (state == END_NORMAL)
                {
                    if (is_ret)
                    {
                        is_in_call = false;
                    }

                    state = INIT;

                    continue;
                }
            }
        } // namespace disassembler

        //
        //==============================================================================
        // Initializer methods
        //==============================================================================
        //
        void Genaytyk_Disassembler::initialize()
        {
            this->initializeOperandStruct();
            this->initializeRegisterIndexOffsetsize();
            this->initializeInstruction2Str();
        }

        void Genaytyk_Disassembler::initializeOperandStruct()
        {
            std::vector<std::vector<uint8_t>> os = {
                {1, 1, 0x52},
                {1, 3, 0x4F},
                {1, 1, 0x51},
                {1, 3, 0x49},
                {1, 4, 0x49},
                {2, 1, 0x52, 1, 0x52},
                {2, 1, 0x52, 3, 0x4F},
                {2, 1, 0x52, 1, 0x51},
                {2, 1, 0x52, 1, 0x49},
                {2, 1, 0x52, 2, 0x49},
                {2, 1, 0x52, 4, 0x49},
                {2, 3, 0x4F, 1, 0x52},
                {2, 3, 0x4F, 1, 0x49},
                {2, 3, 0x4F, 2, 0x49},
                {2, 3, 0x4F, 4, 0x49},
                {2, 1, 0x51, 1, 0x52},
                {2, 1, 0x51, 1, 0x49},
                {2, 1, 0x51, 2, 0x49},
                {2, 1, 0x51, 4, 0x49},
                {3, 3, 0x49, 1, 0x52, 1, 0x52},
                {3, 3, 0x49, 1, 0x52, 3, 0x4F},
                {3, 3, 0x49, 1, 0x52, 1, 0x51},
                {3, 3, 0x49, 1, 0x52, 1, 0x49},
                {3, 3, 0x49, 1, 0x52, 2, 0x49},
                {3, 3, 0x49, 1, 0x52, 4, 0x49},
                {3, 3, 0x49, 3, 0x4F, 1, 0x52},
                {3, 3, 0x49, 3, 0x4F, 1, 0x49},
                {3, 3, 0x49, 3, 0x4F, 2, 0x49},
                {3, 3, 0x49, 3, 0x4F, 4, 0x49},
                {3, 3, 0x49, 1, 0x51, 1, 0x52},
                {3, 3, 0x49, 1, 0x51, 1, 0x49},
                {3, 3, 0x49, 1, 0x51, 2, 0x49},
                {3, 3, 0x49, 1, 0x51, 4, 0x49}};

            this->operandStruct = std::move(os);
        }

        void Genaytyk_Disassembler::initializeRegisterIndexOffsetsize()
        {
            std::vector<std::vector<uint8_t>> riso = {
                {1, 4, 4},       // "EAX"
                {2, 8, 4},       // "EBX"
                {3, 0xC, 4},     // "ECX"
                {4, 0x10, 4},    // "EDX"
                {5, 0x14, 4},    // "ESP"
                {6, 0x18, 4},    // "EBP"
                {7, 0x1C, 4},    // "EDI"
                {8, 0x20, 4},    // "ESI"
                {9, 0x24, 4},    // "REG0x24"
                {0xA, 0x28, 4},  // "REG0x28"
                {0xB, 0x2C, 4},  // "REG0x2C"
                {0xC, 0x30, 4},  // "REG0x30"
                {0xD, 0x34, 4},  // "REG0x34"
                {0xE, 0x38, 4},  // "REG0x38"
                {0xF, 4, 1},     // "AL"
                {0x10, 5, 1},    // "AH"
                {0x11, 8, 1},    // "BL"
                {0x12, 9, 1},    // "BH"
                {0x13, 0xC, 1},  // "CL"
                {0x14, 0xD, 1},  // "CH"
                {0x15, 0x10, 1}, // "DL"
                {0x16, 0x11, 1}, // "DH"
                {0x17, 4, 2},    // "AX"
                {0x18, 8, 2},    // "BX"
                {0x19, 0xC, 2},  // "CX"
                {0x1A, 0x10, 2}, // "DX"
                {0x1B, 0x14, 2}, // "SP"
                {0x1C, 0x18, 2}, // "BP"
                {0x1D, 0x1C, 2}, // "DI"
                {0x1E, 0x20, 2}  // "SI"
            };

            this->registersIndexOffsetSize = std::move(riso);
        }

        void Genaytyk_Disassembler::initializeInstruction2Str()
        {
            std::map<uint32_t, std::string> i2s =
                {
                    {MOV, "MOV"},
                    {ADD, "ADD"},
                    {SUB, "SUB"},
                    {IMUL, "IMUL"},
                    {IDIV, "IDIV"},
                    {OR, "OR"},
                    {XOR, "XOR"},
                    {AND, "AND"},
                    {INC, "INC"},
                    {DEC, "DEC"},
                    {NOT, "NOT"},
                    {SHR, "SHR"},
                    {SHL, "SHL"},
                    {ROR, "ROR"},
                    {ROL, "ROL"},
                    {JMP, "JMP"},
                    {JZ, "JZ"},
                    {JNZ, "JNZ"},
                    {JA, "JA"},
                    {JB, "JB"},
                    {JNB, "JNB"},
                    {JBE, "JBE"},
                    {CALL, "CALL"},
                    {PUSH, "PUSH"},
                    {POP, "POP"},
                    {RET, "RET"},
                    {NOP, "NOP"},
                    {PUSHAD, "PUSHAD"},
                    {POPAD, "POPAD"},
                    {ERROR, "ERROR"}};

            this->instr2str = std::move(i2s);
        }

        void Genaytyk_Disassembler::initializeGlobalData(llvm::IRBuilder<> &irbuilder)
        {
            llvm::Type *charTy = irbuilder.getInt8Ty();
            llvm::Type *i32Ty = irbuilder.getInt32Ty();

            // create hardcoded var
            //llvm::Type *hardcodedType = llvm::VectorType::get(charTy, strlen("aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w") + 1);

            llvm::Type *hardcodedType = llvm::ArrayType::get(charTy, strlen("aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w") + 1);
            auto *hardcodedString = genaytyk_translator->translateCreateGlobal(hardcodedType, "HardcodedString");

            llvm::Constant *hardcodedString_value = llvm::ConstantDataArray::getString(this->module->getContext(), "aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w", true);
            hardcodedString->setAlignment(1);
            hardcodedString->setInitializer(hardcodedString_value);

            this->hardcodedString = hardcodedString;

            // aOkay_guy string
            llvm::Type *okay_guy_type = llvm::ArrayType::get(charTy, strlen("OKAY_GUY") + 1);
            auto *okay_guy = genaytyk_translator->translateCreateGlobal(okay_guy_type, "OKAY_GUY");
            llvm::Constant *okay_guy_value = llvm::ConstantDataArray::getString(this->module->getContext(), "OKAY_GUY", true);
            okay_guy->setAlignment(1);
            okay_guy->setInitializer(okay_guy_value);

            llvm::ConstantAggregateZero *zero_initializer;

            // hashed name
            llvm::Type *hashName_type = llvm::VectorType::get(charTy, 0x5f);
            zero_initializer = llvm::ConstantAggregateZero::get(hashName_type);
            auto *hash_name = genaytyk_translator->translateCreateGlobal(hashName_type, "hash_name");
            hash_name->setAlignment(1);
            hash_name->setInitializer(zero_initializer);

            // vector of 9 bytes empty
            llvm::Type *nine_size_vector = llvm::VectorType::get(charTy, 9);
            zero_initializer = llvm::ConstantAggregateZero::get(nine_size_vector);
            auto *nine_size = genaytyk_translator->translateCreateGlobal(nine_size_vector, "global1");
            nine_size->setAlignment(1);
            nine_size->setInitializer(zero_initializer);

            llvm::ConstantInt *zero_int_value = llvm::ConstantInt::get(this->module->getContext(), llvm::APInt(32, 0));
            // MySerialHash
            auto *myserialhash = genaytyk_translator->translateCreateGlobal(i32Ty, "MySerialHash");
            myserialhash->setAlignment(4);
            myserialhash->setInitializer(zero_int_value);
            // two dd unused
            auto *global2 = genaytyk_translator->translateCreateGlobal(i32Ty, "global2");
            global2->setAlignment(4);
            global2->setInitializer(zero_int_value);

            auto *global3 = genaytyk_translator->translateCreateGlobal(i32Ty, "global3");
            global3->setAlignment(4);
            global3->setInitializer(zero_int_value);

            // vector of 0x14 bytes empty
            llvm::Type *twenty_size_vector = llvm::VectorType::get(charTy, 0x14);
            zero_initializer = llvm::ConstantAggregateZero::get(twenty_size_vector);
            auto *global4 = genaytyk_translator->translateCreateGlobal(twenty_size_vector, "global4");
            global4->setAlignment(1);
            global4->setInitializer(zero_initializer);

            // MyName & MySerial field
            llvm::Type *name_serial_vector = llvm::VectorType::get(charTy, 0x25);
            zero_initializer = llvm::ConstantAggregateZero::get(name_serial_vector);
            auto *myname = genaytyk_translator->translateCreateGlobal(name_serial_vector, "MyName");
            myname->setAlignment(1);
            myname->setInitializer(zero_initializer);
            auto *myserial = genaytyk_translator->translateCreateGlobal(name_serial_vector, "MySerial");
            myserial->setAlignment(1);
            myserial->setInitializer(zero_initializer);

            // nameLength
            auto *namelength = genaytyk_translator->translateCreateGlobal(i32Ty, "nameLength");
            namelength->setAlignment(4);
            namelength->setInitializer(zero_int_value);
            // finalComparison
            auto *finalComparison = genaytyk_translator->translateCreateGlobal(i32Ty, "finalComparison");
            finalComparison->setAlignment(4);
            finalComparison->setInitializer(zero_int_value);
        }

        //
        //==============================================================================
        // Getters methods
        //==============================================================================
        //
        std::string Genaytyk_Disassembler::getOperationString(uint8_t opcode)
        {
            if (opcode > ERROR)
            {
                return "";
            }

            return this->instr2str[opcode];
        }

        std::vector<uint8_t> Genaytyk_Disassembler::getOperandStruct(uint8_t opcode)
        {
            if ((opcode < 1) || (opcode > this->operandStruct.size()))
            {
                throw Disassembler_Exception("Opcode not in operand struct");
            }

            opcode -= 1;
            return this->operandStruct[opcode];
        }

        std::vector<std::map<std::string, uint8_t>> Genaytyk_Disassembler::getMapsFromOperandStruct(std::vector<uint8_t> operandStruct)
        {
            std::vector<std::map<std::string, uint8_t>> operands;

            if ((operandStruct[0] <= 3) && (operandStruct[0] >= 1))
            {
                operands.push_back(
                    {{"size", operandStruct[1]},
                     {"type", operandStruct[2]}});
            }

            if ((operandStruct[0] <= 3) && (operandStruct[0] >= 2))
            {
                operands.push_back(
                    {{"size", operandStruct[3]},
                     {"type", operandStruct[4]}});
            }

            if (operandStruct[0] == 3)
            {
                operands.push_back(
                    {{"size", operandStruct[5]},
                     {"type", operandStruct[6]}});
            }

            return operands;
        }

        uint32_t Genaytyk_Disassembler::getImmediateValue(uint8_t sizeOfImmediate)
        {
            uint32_t value = 0;
            uint8_t aux = 0;

            while (sizeOfImmediate != 0)
            {
                aux = this->p_to_code[this->index];
                this->index++;
                value |= (aux << (8 * (sizeOfImmediate - 1)));
                sizeOfImmediate--;
            };

            return value;
        }

    } // namespace disassembler
} // namespace genaytyk