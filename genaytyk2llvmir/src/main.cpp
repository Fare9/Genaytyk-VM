#include "genaytyk_impl.h"
#include "genaytyk-dis.h"
#include <memory>
#include <iostream>
#include <fstream>

// for writing
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

static llvm::LLVMContext &context = llvm::getGlobalContext();
static llvm::Module *moduleOb = new llvm::Module("genaytyk", context);
static llvm::IRBuilder<> irbuilder(context);


int main(int argc, char **argv)
{
    std::unique_ptr<llvm::tool_output_file> Out(
        new llvm::tool_output_file(
            "./genaytyk.bc",
            llvm::sys::fs::F_None 
        )
    );

    if (argc != 2)
    {
        llvm::outs() << "[-] USAGE: " << argv[0] << " <file_with_code>\n";
        exit(-1);
    }

    std::ifstream code_file;
    code_file.open(argv[1], std::ios::in | std::ios::binary);

    size_t file_size = static_cast<std::uint64_t>(code_file.tellg());
    code_file.seekg(0, std::ios::end);
    file_size = static_cast<std::uint64_t>(code_file.tellg()) - file_size;
    code_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> file_content(file_size);

    code_file.read(reinterpret_cast<char *>(file_content.data()), file_size);

    std::unique_ptr<genaytyk::disassembler::Genaytyk_Disassembler> disassembler(new genaytyk::disassembler::Genaytyk_Disassembler(file_content.data(), file_size, moduleOb, irbuilder));

    disassembler->disassemble(irbuilder);

    moduleOb->dump();

    llvm::outs() << "\n\nGenaytyk has been completely translated to LLVM IR\n\n";

    return 0;
}


/*
int
main(int argc, char **argv)
{
    
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

    auto* a1 = genaytyk_translator->storeRegister(genaytyk::REG_EAX, irbuilder, add_instruction);

    auto* next_basic_block = genaytyk_translator->createBB(entry_point_function, "next_addr");
    auto* jump_address = genaytyk_translator->createBB(entry_point_function, "jump_address");

    auto* a2 = genaytyk_translator->storeRegister(genaytyk::REG_EBX,irbuilder,irbuilder.getInt32(2));
    auto* a3 = genaytyk_translator->loadRegister(genaytyk::REG_EBX,irbuilder,irbuilder.getInt32Ty());
    
    genaytyk_translator->translateCreateJNZ(genaytyk_translator->getRegister(genaytyk::REG_EAX), genaytyk_translator->getRegister(genaytyk::REG_EBX), jump_address, next_basic_block, irbuilder);

    irbuilder.SetInsertPoint(next_basic_block);

    auto* add_instruction2 = genaytyk_translator->translateAdd(genaytyk_translator->getRegister(genaytyk::REG_0x24), add_instruction, irbuilder);

    genaytyk_translator->storeRegister(genaytyk::REG_0x24, irbuilder, add_instruction2);

    irbuilder.SetInsertPoint(jump_address);

    irbuilder.CreateRet(genaytyk_translator->getRegister(genaytyk::REG_EAX));

    moduleOb->dump();

    return 0;
}
*/