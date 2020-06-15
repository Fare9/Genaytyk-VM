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

        void Genaytyk_Disassembler::disassemble(llvm::IRBuilder<> &irbuilder)
        {
            llvm::Function *current_function;
            std::vector<uint8_t> operandStruct;
            std::vector<std::map<std::string, uint8_t>> mapsOfOperands;
            instructions instruction;

            while (true)
            {
                if (this->state == SETUP)
                {
                    this->initializeGlobalData(irbuilder);
                    // create the main function
                    current_function = this->genaytyk_translator->createFunc(irbuilder, "main", irbuilder.getVoidTy(), irbuilder.getVoidTy());
                    auto *bb_entry = this->genaytyk_translator->createBB(current_function, "entry");
                    irbuilder.SetInsertPoint(bb_entry);

                    this->state = GET_OPERATION;
                    continue;
                }
                else if (this->state == INIT)
                {
                    if (this->index >= this->size_of_code)
                    {
                        break;
                    }
                    this->state = GET_OPERATION;

                    llvm::outs() << "\nDo you want to continue disassembling?\n";

                    if (getchar() == 'n')
                        return;

                    while ((getchar()) != '\n')
                        ;

                    continue;
                }
                else if (this->state == GET_OPERATION)
                {
                    llvm::BasicBlock *basic_block;
                    auto it = this->addr2llvmfunc.find(index);

                    if (it != this->addr2llvmfunc.end())
                    {
                        // function is going to start, so create a basic block for it
                        this->is_in_call = true;
                        current_function = it->second;
                    }
                    // now create basic block for new instruction
                    // and write instruction in there
                    if (this->addr2llvmbb.find(index) == this->addr2llvmbb.end())
                    {
                        basic_block = this->genaytyk_translator->createBB(current_function, std::to_string(index));
                        this->addr2llvmbb[index] = basic_block;
                    }
                    else
                    {
                        basic_block = this->addr2llvmbb[index];
                    }
                    irbuilder.SetInsertPoint(basic_block);

                    this->call_jmp = false;
                    this->is_ret = false;
                    this->jmp_op = false;

                    switch (this->p_to_code[index])
                    {
                    case RET:
                        irbuilder.CreateRet(this->genaytyk_translator->loadRegister(REG_EAX, irbuilder, irbuilder.getInt32Ty()));
                        index++;
                        this->is_ret = true;
                        this->state = END_NORMAL;
                        continue;
                    case NOP:
                        index++;
                        this->state = END_NORMAL;
                        continue;
                    case PUSHAD:
                        //TODO
                        continue;
                    case POPAD:
                        //TODO
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
                    llvm::Value *l = nullptr;
                    llvm::Value *r = nullptr;
                    llvm::BasicBlock *next_addr = nullptr;
                    llvm::BasicBlock *jump_addr = nullptr;

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
                            if (l == nullptr)
                                l = irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"]));
                            else if (r == nullptr)
                                r = irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"]));
                            break;
                        case REGISTER:
                            // check and get register from the opcode
                            if (l == nullptr)
                                l = this->genaytyk_translator->getRegister(p_to_code[index]);
                            else if (r == nullptr)
                                r = this->genaytyk_translator->getRegister(p_to_code[index]);
                            index++;
                            break;
                        case ADDRESS:
                            if (l == nullptr)
                            {
                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, this->genaytyk_translator->getRegister(p_to_code[index]));
                                l = this->genaytyk_translator->load(l, irbuilder, irbuilder.getInt32Ty());
                            }
                            else if (r == nullptr)
                            {
                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, this->genaytyk_translator->getRegister(p_to_code[index]));
                                r = this->genaytyk_translator->load(l, irbuilder, irbuilder.getInt32Ty());
                            }
                            index++;
                            break;
                        case SERIAL_HASH:
                            if (l == nullptr)
                            {
                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"])));
                                l = this->genaytyk_translator->load(l, irbuilder, irbuilder.getInt32Ty());
                            }
                            else if (r == nullptr)
                            {
                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"])));
                                r = this->genaytyk_translator->load(r, irbuilder, irbuilder.getInt32Ty());
                            }
                        default:
                            break;
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
                    llvm::Value *l = nullptr;
                    llvm::Value *r = nullptr;
                    bool l_is_register = false;
                    bool r_is_register = false;
                    uint8_t l_register = 0;
                    uint8_t r_register = 0;

                    for (size_t i = 0; i < mapsOfOperands.size(); i++)
                    {
                        if (mapsOfOperands[i]["type"] == IMMEDIATE)
                        {
                            uint32_t offset = getImmediateValue(mapsOfOperands[i]["size"]);

                            if (call_jmp)
                            {
                                // it is a call check if function exists or create it
                                llvm::Function *callee;
                                if (this->addr2llvmfunc.find(offset) == this->addr2llvmfunc.end())
                                {
                                    callee = this->genaytyk_translator->createFunc(irbuilder, std::to_string(offset), irbuilder.getVoidTy(), irbuilder.getVoidTy());
                                    this->addr2llvmfunc[offset] = callee;
                                }
                                else
                                {
                                    callee = this->addr2llvmfunc[offset];
                                }
                                // create a call to a function
                                auto* return_value = this->genaytyk_translator->translateCreateCall(callee, irbuilder);
                                this->genaytyk_translator->storeRegister(REG_EAX, irbuilder, return_value);
                            }
                            else
                            {
                                if (l == nullptr)
                                    l = irbuilder.getInt32(offset);
                                else if (r == nullptr)
                                    r = irbuilder.getInt32(offset);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == REGISTER)
                        {
                            uint8_t opcodeRegister = this->p_to_code[index];
                            index++;
                            if (l == nullptr)
                            {
                                l_is_register = true;
                                l_register = opcodeRegister;
                                l = this->genaytyk_translator->getRegister(opcodeRegister);
                            }
                            else if (r == nullptr)
                            {
                                r_is_register = true;
                                r_register = opcodeRegister;
                                r = this->genaytyk_translator->getRegister(opcodeRegister);
                            }
                        }
                        else if (mapsOfOperands[i]["type"] == ADDRESS)
                        {
                            if (l == nullptr)
                            {
                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, this->genaytyk_translator->getRegister(p_to_code[index]));
                                l = this->genaytyk_translator->load(l, irbuilder, irbuilder.getInt32Ty());
                            }
                            else if (r == nullptr)
                            {
                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, this->genaytyk_translator->getRegister(p_to_code[index]));
                                r = this->genaytyk_translator->load(l, irbuilder, irbuilder.getInt32Ty());
                            }
                            index++;
                            break;
                        }
                        else if (mapsOfOperands[i]["type"] == SERIAL_HASH)
                        {
                            if (l == nullptr)
                            {
                                l = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"])));
                                l = this->genaytyk_translator->load(l, irbuilder, irbuilder.getInt32Ty());
                            }
                            else if (r == nullptr)
                            {
                                r = this->genaytyk_translator->getPointerFromAddress(irbuilder, irbuilder.getInt1Ty(), this->hardcodedString, irbuilder.getInt32(getImmediateValue(mapsOfOperands[i]["size"])));
                                r = this->genaytyk_translator->load(r, irbuilder, irbuilder.getInt32Ty());
                            }
                        }
                        else
                        {
                            throw Disassembler_Exception("Error type not supported\n");
                        }
                    }

                    switch (instruction)
                    {
                    case MOV:
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case ADD:
                        this->genaytyk_translator->translateAdd(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case SUB:
                        this->genaytyk_translator->translateSub(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case IMUL:
                        this->genaytyk_translator->translateImul(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case IDIV:
                        this->genaytyk_translator->translateIdiv(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case OR:
                        this->genaytyk_translator->translateOr(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case XOR:
                        this->genaytyk_translator->translateXor(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case AND:
                        this->genaytyk_translator->translateAnd(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case INC:
                        this->genaytyk_translator->translateInc(l, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case DEC:
                        this->genaytyk_translator->translateDec(l, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case NOT:
                        this->genaytyk_translator->translateNot(l, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case SHR:
                        this->genaytyk_translator->translateShr(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case SHL:
                        this->genaytyk_translator->translateShl(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case ROR:
                        this->genaytyk_translator->translateRor(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
                        }
                        break;
                    case ROL:
                        this->genaytyk_translator->translateRol(l, r, irbuilder);
                        if (l_is_register)
                        {
                            this->genaytyk_translator->storeRegister(l_register, irbuilder, r);
                        }
                        else
                        {
                            this->genaytyk_translator->store(l, irbuilder, r);
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
        }

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
            llvm::Type *charTy = irbuilder.getInt1Ty();
            llvm::Type *i32Ty = irbuilder.getInt32Ty();

            // create hardcoded var
            llvm::Type *hardcodedType = llvm::VectorType::get(charTy, strlen("aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w") + 1);
            auto *hardcodedString = genaytyk_translator->translateCreateGlobal(hardcodedType, "HardcodedString");
            llvm::Constant *hardcodedString_value = llvm::ConstantDataArray::getString(this->module->getContext(), "aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w", true);
            hardcodedString->setAlignment(1);
            hardcodedString->setInitializer(hardcodedString_value);

            this->hardcodedString = hardcodedString;

            // aOkay_guy string
            llvm::Type *okay_guy_type = llvm::VectorType::get(charTy, strlen("OKAY_GUY") + 1);
            auto *okay_guy = genaytyk_translator->translateCreateGlobal(okay_guy_type, "OKAY_GUY");
            llvm::Constant *okay_guy_value = llvm::ConstantDataArray::getString(this->module->getContext(), "OKAY_GUY", true);
            okay_guy->setAlignment(1);
            okay_guy->setInitializer(okay_guy_value);

            // hashed name
            llvm::Type *hashName_type = llvm::VectorType::get(charTy, 0x5f);
            genaytyk_translator->translateCreateGlobal(hashName_type, "hash_name");

            // vector of 9 bytes empty
            llvm::Type *nine_size_vector = llvm::VectorType::get(charTy, 9);
            genaytyk_translator->translateCreateGlobal(nine_size_vector, "global1");

            // MySerialHash
            genaytyk_translator->translateCreateGlobal(i32Ty, "MySerialHash");

            // two dd unused
            genaytyk_translator->translateCreateGlobal(i32Ty, "global2");
            genaytyk_translator->translateCreateGlobal(i32Ty, "global3");

            // vector of 0x14 bytes empty
            llvm::Type *twenty_size_vector = llvm::VectorType::get(charTy, 0x14);
            genaytyk_translator->translateCreateGlobal(twenty_size_vector, "global4");

            // MyName & MySerial field
            llvm::Type *name_serial_vector = llvm::VectorType::get(charTy, 0x25);
            genaytyk_translator->translateCreateGlobal(name_serial_vector, "MyName");
            genaytyk_translator->translateCreateGlobal(name_serial_vector, "MySerial");

            // nameLength
            genaytyk_translator->translateCreateGlobal(i32Ty, "nameLength");

            // finalComparison
            genaytyk_translator->translateCreateGlobal(i32Ty, "finalComparison");
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