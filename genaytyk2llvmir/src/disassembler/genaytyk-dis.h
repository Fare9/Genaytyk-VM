#ifndef GENAYTYK_DIS_H
#define GENAYTYK_DIS_H

#include <iostream>
#include <vector>
#include "genaytyk_impl.h"

namespace genaytyk
{
    namespace disassembler
    {
        /// definition of the instructions supported by
        /// Genaytyk VM, each enumator correspond to the
        /// instruction opcode.
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

        /// Genaytyk type of operands
        typedef enum _opcode_types
        {
            IMMEDIATE = 0x49,
            SERIAL_HASH = 0x4F,
            ADDRESS = 0x51,
            REGISTER = 0x52
        } opcode_types;

        /// States for the disassember
        /// extracted from the first version
        /// of the disassembler.
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
            Genaytyk_Disassembler(uint8_t *p_to_code, size_t size_of_code, llvm::Module* module, llvm::IRBuilder<>& irbuilder);


            void disassemble(llvm::IRBuilder<>& irbuilder);
            //
            //==============================================================================
            // Initializer methods
            //==============================================================================
            //

            void initialize();
            void initializeOperandStruct();
            void initializeRegisterIndexOffsetsize();
            void initializeInstruction2Str();

            void initializeGlobalData(llvm::IRBuilder<>& irbuilder);

            //
            //==============================================================================
            // Getters methods
            //==============================================================================
            //

            /// Get the string with the operation
            /// from its opcode
            std::string getOperationString(uint8_t opcode);

            /// Get the struct of operands given
            /// the opcode (used as index)-
            std::vector<uint8_t> getOperandStruct(uint8_t opcode);

            /// Once obtained the operandStruct that
            /// we're interested in, this return
            /// a vector of maps with the size of the
            /// operand in opcodes and the type
            std::vector<std::map<std::string,uint8_t>> getMapsFromOperandStruct(std::vector<uint8_t> operandStruct);

            /// Get immediate value given a size of immediate
            /// value, we always return an uint32_t
            uint32_t getImmediateValue(uint8_t sizeOfImmediate);

            /// Get all the offsets where to jump
            /// this is the same pass than the disassembler
            /// but will take only offsets of jumps to create
            /// later basic blocks.
            void get_jump_address(llvm::IRBuilder<> &irbuilder);
        private:
            llvm::Module* module;
            /// pointer to buffer with code
            uint8_t *p_to_code;
            size_t size_of_code;

            /// FSM variables
            uint32_t index;
            bool call_jmp;
            bool jmp_op;
            bool is_ret;
            bool is_in_call;
            uint32_t ret;
            std::string function_name;

            states state;

            llvm::GlobalVariable* hardcodedString;
            // Map for the functions, this will correspond
            // to the list of offsets from the previous
            // disassembler
            std::map<uint32_t, llvm::Function *> addr2llvmfunc;

            /// Map for the basic blocks, for the moment
            /// it will be generated one per instruction
            /// in order to be able to make jumps backward
            /// also it will allow to check if a jumps
            /// forward basic block is already created
            std::map<uint32_t, llvm::BasicBlock *> addr2llvmbb;

            std::vector<uint32_t> basic_block_addresses;

            /// Necessary for instructions, structs with
            /// the operand types and registers
            std::vector<std::vector<uint8_t>> operandStruct;

            std::vector<std::vector<uint8_t>> registersIndexOffsetSize;

            // Map for instructions names
            std::map<uint32_t, std::string> instr2str;

            // unique pointer to a translation library
            std::unique_ptr<GenaytykLlvmIrTranslatorGenaytyk_impl> genaytyk_translator;
        };

    } // namespace disassembler
} // namespace genaytyk

#endif