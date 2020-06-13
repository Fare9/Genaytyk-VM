#include "genaytyk_impl.h"
#include <memory>

static llvm::LLVMContext &context = llvm::getGlobalContext();
static llvm::Module *moduleOb = new llvm::Module("genaytyk", context);



int
main(int argc, char **argv)
{
    static llvm::IRBuilder<> irbuilder(context);
    std::unique_ptr<genaytyk::GenaytykLlvmIrTranslatorGenaytyk_impl> genaytyk_translator(new genaytyk::GenaytykLlvmIrTranslatorGenaytyk_impl(moduleOb, irbuilder));
    

    llvm::Type* voidTy = irbuilder.getVoidTy();

    llvm::Type* charTy = irbuilder.getInt1Ty();
    
    llvm::Type* hardcodedType = llvm::VectorType::get(charTy, strlen("aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w") + 1);

    auto* global_var = genaytyk_translator->translateCreateGlobal(hardcodedType, "aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w");

    llvm::Constant *const_array_4 = llvm::ConstantDataArray::getString(moduleOb->getContext(), "aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w", true);

    global_var->setAlignment(1);
    global_var->setInitializer(const_array_4);
    
    auto* entry_point_function = genaytyk_translator->createFunc(irbuilder, "entry_point", voidTy, irbuilder.getVoidTy());

    // create a second function
    auto* called_function = genaytyk_translator->createFunc(irbuilder, "called_function", voidTy, irbuilder.getVoidTy());

    auto* entry_point_block = genaytyk_translator->createBB(entry_point_function, "entry");

    irbuilder.SetInsertPoint(entry_point_block);

    // create a call
    genaytyk_translator->translateCreateCall(called_function, irbuilder);

    auto* add_instruction = genaytyk_translator->translateAdd(genaytyk_translator->getRegister(genaytyk::REG_EAX), irbuilder.getInt32(5), irbuilder);

    genaytyk_translator->storeRegister(genaytyk::REG_EAX, irbuilder, add_instruction);

    auto* next_basic_block = genaytyk_translator->createBB(entry_point_function, "next_addr");
    auto* jump_address = genaytyk_translator->createBB(entry_point_function, "jump_address");

    genaytyk_translator->translateCreateJNZ(genaytyk_translator->getRegister(genaytyk::REG_EAX), irbuilder.getInt32(0), jump_address, next_basic_block, irbuilder);

    irbuilder.SetInsertPoint(next_basic_block);

    auto* add_instruction2 = genaytyk_translator->translateAdd(genaytyk_translator->getRegister(genaytyk::REG_0x24), irbuilder.getInt32(2), irbuilder);

    genaytyk_translator->storeRegister(genaytyk::REG_0x24, irbuilder, add_instruction2);

    irbuilder.SetInsertPoint(jump_address);

    irbuilder.CreateRet(genaytyk_translator->getRegister(genaytyk::REG_EAX));

    moduleOb->dump();

    return 0;
}