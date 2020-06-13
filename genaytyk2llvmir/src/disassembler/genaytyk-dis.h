#ifndef GENAYTYK_DIS_H
#define GENAYTYK_DIS_H

#include <iostream>
#include <vector>
#include "genaytyk_impl.h"

namespace genaytyk
{
    namespace disassembler
    {

        typedef enum _instructions
        {
            MOV = 1,
            ADD,
            SUB,
            IMUL,
            IDIV,
            OR,
            XOR,
            AND,
            INC,
            DEC,
            NOT,
            SHR,
            SHL,
            ROR,
            ROL,
            JMP,
            JZ,
            JNZ,
            JA,
            JB,
            JNB,
            JBE,
            CALL,
            PUSH,
            POP,
            RET,
            NOP,
            PUSHAD,
            POPAD,
            ERROR
        } instructions;

        typedef enum _opcode_types
        {
            IMMEDIATE = 0x49,
            SERIAL_HASH = 0x4F,
            ADDRESS = 0x51,
            REGISTER = 0x52
        } opcode_types;

        typedef enum _states
        {
            SETUP = 0,
            INIT = 98,
            GET_OPERATION = 1,
            GET_OPERANDS = 2,
            GET_JC_OPERANDS = 3,
            END_CALL = 4,
            END_RET = 5,
            END_NORMAL = 6,
            ERROR_STATE = 99
        } states;

        class Genaytyk_Disassembler
        {
        public:

            Genaytyk_Disassembler(uint8_t* p_to_code, size_t size_of_code);

            //
            //==============================================================================
            // Initializer methods
            //==============================================================================
            //

            void initialize();
            void initializeOperandStruct();
            void initializeRegisterIndexOffsetsize();
            void initializeInstruction2Str();


            //
            //==============================================================================
            // Getters methods
            //==============================================================================
            //
            std::string getOperationString(uint8_t opcode);

        private:
            /// pointer to buffer with code
            uint8_t*    p_to_code;
            size_t      size_of_code;

            /// FSM variables
            uint32_t index;
            bool call_jmp;
            bool jmp_op;
            bool is_ret;
            bool is_in_call;
            uint32_t ret;
            std::string function_name;

            states state;
            
            // Map for the functions, this will correspond
            // to the list of offsets from the previous
            // disassembler
            std::map<uint32_t, llvm::Function*> addr2llvmfunc;

            /// Map for the basic blocks, for the moment
            /// it will be generated one per instruction
            /// in order to be able to make jumps backward
            /// also it will allow to check if a jumps
            /// forward basic block is already created
            std::map<uint32_t, llvm::BasicBlock *> addr2llvmbb;

            /// Necessary for instructions, structs with
            /// the operand types and registers
            std::vector<std::vector<uint8_t>> operandStruct;

            std::vector<std::vector<uint8_t>> registersIndexOffsetSize;

            // Map for instructions names
            std::map<uint32_t,std::string> instr2str;

    } // namespace disassembler
} // namespace genaytyk

#endif