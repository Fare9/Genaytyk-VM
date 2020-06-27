/*
*   Header implementation of genaytyk VM,
*   code is based mainly in capstone2llvmir
*   from retdec!
*/
#ifndef GENAYTYK_IMPL_H
#define GENAYTYK_IMPL_H

// libraries for creating a module
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
// for calling verifyFunction
#include "llvm/IR/Verifier.h"
// builder to create IR code
#include "llvm/IR/IRBuilder.h"
// vector from C++
#include <vector>

// for output
#include "llvm/Support/raw_ostream.h"

#include <iostream>

namespace genaytyk
{

    /// enum type for genaytyk registers
    /// names are only same to x86
    /// registers, but are not x86 registers
    typedef enum registers
    {
        REG_EIP,
        REG_EAX,
        REG_EBX,
        REG_ECX,
        REG_EDX,
        REG_ESP,
        REG_EBP,
        REG_EDI,
        REG_ESI,
        REG_0x24,
        REG_0x28,
        REG_0x2C,
        REG_0x30,
        REG_0x34,
        REG_0x38,
        REG_AL,
        REG_AH,
        REG_BL,
        REG_BH,
        REG_CL,
        REG_CH,
        REG_DL,
        REG_DH,
        REG_AX,
        REG_BX,
        REG_CX,
        REG_DX,
        REG_SP,
        REG_BP,
        REG_DI,
        REG_SI,
        REG_ERROR
    } genaytyk_reg;

    class GenaytykLlvmIrTranslatorGenaytyk_impl
    {
    public:
        GenaytykLlvmIrTranslatorGenaytyk_impl(llvm::Module *module, llvm::IRBuilder<>& irbuilder);

        //
        //==============================================================================
        // Initializer methods
        //==============================================================================
        //

        /**
         * Initializer method. It calls different initializer methods that
         * need an IRBuilder
         */
        void initialize(llvm::IRBuilder<>& irbuilder);

        /**
		 * Initialize @c _reg2name. See comment for @c _reg2name to know what
		 * must be initialized, and what may or may not be initialized.
		 */
        void initializeRegNameMap();

        /**
		 * Initialize @c _reg2type. See comment for @c _reg2type to know what
		 * must be initialized, and what may or may not be initialized.
		 */
        void initializeRegTypeMap();

        /**
         * Initialize @ llvm2GenaytykRegs. See comment for @c llvm2GenaytykRegs to know what
         * must be initialized, and what may or may not be initialized.
         */
        void initializeLlvmRegMap();

        /**
         * Initialize @ llvm2GenaytykRegs. See comment for @c llvm2GenaytykRegs to know what
         * must be initialized, and what may or may not be initialized.
         */
        void initializeRegLlvmMap();

        /**
         * Initialize @ reg2parentMap. See comment for @c reg2parentMap to know what
         * must be initialized, and what may or may not be initialized.
         */
        void initializeRegistersParentMapToOther(
            const std::vector<genaytyk_reg>& rs,
		    genaytyk_reg other);

        void initializeRegistersParentMap32();

        /**
         * Initializer of different methods that are used as assembly instructions
         * for not being implemented in LLVM IR
         */
        void initializeFunctions(llvm::IRBuilder<>& irbuilder);

        /**
         * Initialize function of ror, rol, pushad and popad 
         * to avoid masive code when this instructions are used
         */
        void initializeRor(llvm::IRBuilder<>& irbuilder);
        void initializeRol(llvm::IRBuilder<>& irbuilder);
        void initializePushad(llvm::IRBuilder<>& irbuilder);
        void initializePopad(llvm::IRBuilder<>& irbuilder);

        //
        //==============================================================================
        // Load/store methods.
        //==============================================================================
        //

        /**
        * Methods to create load and store instructions in LLVM IR
        * depending on source type
        */
        llvm::Value *loadRegister(
            uint32_t reg_,                // register to load
            llvm::IRBuilder<> &irbuilder, // builder to generate instruction
            llvm::Type *dstType           // destination type
        );
        llvm::Value *load(
            llvm::Value *addr,            // address to load
            llvm::IRBuilder<> &irbuilder, // builder to generate instruction
            llvm::Type *dstType           // destination type
        );

        llvm::StoreInst* storeRegister(
            uint32_t reg_,                // destination register
            llvm::IRBuilder<> &irbuilder, // builder to generate instruction
            llvm::Value *val              // source value
        );

        llvm::StoreInst* store(
            llvm::Value *addr,            // destination address
            llvm::IRBuilder<> &irbuilder, // builder to generate instruction
            llvm::Value *val              // source value
        );

        

        //
        //==============================================================================
        // Create statements implementation.
        //==============================================================================
        //

        /**
         * Method to generate a global variable,
         * given a name and a type.
         */
        llvm::GlobalVariable *translateCreateGlobal(
            llvm::Type *type, // global variable type
            std::string name  // global variable name
        );

        /**
         * Method to generate a function given
         * a name, a set of params and a return
         * type
         */
        llvm::Function* createFunc(
            llvm::IRBuilder<>& irbuilder,       // builder
            std::string name,                   // function name
            llvm::ArrayRef<llvm::Type*> params, // parameters as vector type
            llvm::Type* return_type             // return type
        );

        /**
         * Method to generate a basic block
         * given a name.
         */
        llvm::BasicBlock* createBB(
            llvm::Function* parentFunction,     // parent function
            std::string name                    // basic block name
        );

        //
        //==============================================================================
        // Genaytyk instruction translation methods.
        //==============================================================================
        //
        llvm::Value *translateAdd(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateSub(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateImul(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateIdiv(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateOr(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateXor(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateAnd(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateInc(llvm::Value *v, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateDec(llvm::Value *v, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateNot(llvm::Value *v, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateShr(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateShl(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateRor(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translateRol(llvm::Value *l, llvm::Value *r, llvm::IRBuilder<> &irbuilder);

        void translatePushad(llvm::IRBuilder<> &irbuilder);
        void translatePopad(llvm::IRBuilder<> &irbuilder);
        void translatePush(llvm::Value *val, llvm::IRBuilder<> &irbuilder);
        llvm::Value *translatePop(llvm::IRBuilder<> &irbuilder);

        //
        //==============================================================================
        // Genaytyk branching instruction translation methods.
        //==============================================================================
        //
        
        llvm::BranchInst* translateCreateJMP(llvm::BasicBlock* destination, llvm::IRBuilder<> &irbuilder);
        llvm::BranchInst* translateCreateJZ(llvm::Value* l, llvm::Value* r, llvm::BasicBlock* destination, llvm::BasicBlock* next_addr, llvm::IRBuilder<> &irbuilder);
        llvm::BranchInst* translateCreateJNZ(llvm::Value* l, llvm::Value* r, llvm::BasicBlock* destination, llvm::BasicBlock* next_addr, llvm::IRBuilder<> &irbuilder);
        llvm::BranchInst* translateCreateJA(llvm::Value* l, llvm::Value* r, llvm::BasicBlock* destination, llvm::BasicBlock* next_addr, llvm::IRBuilder<> &irbuilder);
        llvm::BranchInst* translateCreateJB(llvm::Value* l, llvm::Value* r, llvm::BasicBlock* destination, llvm::BasicBlock* next_addr, llvm::IRBuilder<> &irbuilder);
        llvm::BranchInst* translateCreateJNB(llvm::Value* l, llvm::Value* r, llvm::BasicBlock* destination, llvm::BasicBlock* next_addr, llvm::IRBuilder<> &irbuilder);
        llvm::BranchInst* translateCreateJBE(llvm::Value* l, llvm::Value* r, llvm::BasicBlock* destination, llvm::BasicBlock* next_addr, llvm::IRBuilder<> &irbuilder);
        llvm::CallInst *translateCreateCall(llvm::Function* callee, llvm::IRBuilder<> &irbuilder);

        //
        //==============================================================================
        // getters
        //==============================================================================
        //

        /**
         * Method to get a pointer to a buffer, this pointer must be translated to
         * the necessary destination type as LLVM IR is strongly typed.
         */
        llvm::Value* getPointerFromAddress(
            llvm::IRBuilder<> irbuilder,      // ir builder
            llvm::Type* array_type,         // memory array type
            llvm::Value* base,              // base of memory address
            llvm::Value* offset,             // offset to access
            llvm::Type* dest_type          // destination type
        );

        llvm::GlobalVariable *getRegister(uint32_t r);
        llvm::Type *getRegisterType(uint32_t r);
        uint32_t getParentRegister(uint32_t r);

        //
        //==============================================================================
        // Utilities
        //==============================================================================
        //

        /**
         * Method to extent zeroes or trunk values from one type
         * to the other
         */
        llvm::Value* generateTypeConversion(llvm::IRBuilder<>& irb, llvm::Value* from, llvm::Type* to);

    private:
        llvm::Module *module = nullptr;

        /// Variables to store the functions that
        /// translate to unexisting instructions
        /// in LLVM IR
        llvm::Function *ror_function = nullptr;
        llvm::Function *rol_function = nullptr;
        llvm::Function *pushad = nullptr;
        llvm::Function *popad = nullptr;
        /// For obtaining name of a register given
        /// its register number, these number are
        /// part of genaytyk VM, and structure will
        /// be in the code.
        std::map<uint32_t, std::string> reg2name;
        /// For obtaining llvm type given the number
        /// of a register
        std::map<uint32_t, llvm::Type *> reg2type;

        /// Registers will be represented as global
        /// llvm variables, so it is necessary to
        /// create a map between those registers
        /// and the register number
        std::map<llvm::GlobalVariable *, uint32_t> llvm2GenaytykRegs;
        std::map<uint32_t, llvm::GlobalVariable *> GenaytykRegs2Llvm;

        /// Maps register numbers to numbers of their parents depending on the
        /// original basic mode (e.g. X86_REG_AH to X86_REG_EAX in 32-bit mode,
        /// or to X86_REG_RAX in 64-bit mode).
        /// Unhandled mappings are set to X86_REG_INVALID (e.g. mapping of
        /// X86_REG_EAX in 16-bit mode).
        /// Once generated, it does not change.
        /// Register's number is a key into the array of parent number values.
        /// Only values of the Capstone's original @c x86_reg enum are handled,
        /// our added enums (e.g. @c x86_reg_rflags) are not.
        /// Always use @c getParentRegister() method to get values from this
        /// map -- it will deal with added enums.
        std::vector<uint32_t> reg2parentMap;
    };

} // namespace genaytyk

#endif