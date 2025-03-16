#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h> // Ensure function correctness
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

int main() {
    LLVMContext Context;

    // Create a module to hold the IR
    std::unique_ptr<Module> TheModule = std::make_unique<Module>("my_module", Context);
    IRBuilder<> Builder(Context);

    // Define int32 type correctly
    Type* Int32Ty = Builder.getInt32Ty();

    // Define function parameter types (Must use Type* instead of Type)
    std::vector<Type*> params = {Int32Ty, Int32Ty};

    // Create function type (return type: int32, parameters: int32, int32)
    FunctionType *addFuncType = FunctionType::get(Int32Ty, params, false);

    // Create function (External linkage)
    Function* sumFunc = Function::Create(addFuncType, Function::ExternalLinkage, "sum", TheModule.get());

    // Create function entry block
    BasicBlock* entry = BasicBlock::Create(Context, "entry", sumFunc);
    Builder.SetInsertPoint(entry);

    // Extract function arguments
    Function::arg_iterator args = sumFunc->arg_begin();
    Value* arg1 = args++;
    arg1->setName("a");

    Value* arg2 = args++;
    arg2->setName("b");

    // Create addition instruction
    Value* Valueadd = Builder.CreateAdd(arg1, arg2, "add");

    // Return statement
    Builder.CreateRet(Valueadd);

    // Verify function correctness
    verifyFunction(*sumFunc);

    // Print generated LLVM IR
    TheModule->print(errs(), nullptr);

    return 0;
}