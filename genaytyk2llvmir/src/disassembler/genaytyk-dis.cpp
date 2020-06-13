#include "genaytyk-dis.h"

namespace genaytyk
{
    namespace disassembler
    {
        Genaytyk_Disassembler::Genaytyk_Disassembler(uint8_t *p_to_code, size_t size_of_code) : index(0),
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
            this->initialize();
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
                {ERROR, "ERROR"}
            };

            this->instr2str = std::move(i2s);
        }
        //
        //==============================================================================
        // Getters methods
        //==============================================================================
        //
        std::string Genaytyk_Disassembler::getOperationString(uint8_t opcode)
        {
        }

    } // namespace disassembler
} // namespace genaytyk