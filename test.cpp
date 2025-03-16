#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

int main() {
    LLVMContext Context;
    
    std::unique_ptr<Module> TheModule = std::make_unique<Module>("my_module", Context);
    IRBuilder<> Builder(Context);

    //*****************************//
    Type* Int32Ty = Builder.getInt32Ty();

    std::vector<Type*> params={Int32Ty,Int32Ty};

    FunctionType* addFuncType = FunctionType::get(Int32Ty, params, false);

    Function *sumFunc = Function::Create(addFuncType, Function::ExternalLinkage, "sum", TheModule.get());

    Function::arg_iterator args = sumFunc->arg_begin();
    Value *arg1=&*args++;
    arg1->setName("a");
    Value *arg2=&*args++;
    arg2->setName("b");

    //*****************************//

    BasicBlock *entry = BasicBlock::Create(Context, "entry", sumFunc);
    Builder.SetInsertPoint(entry);
    
    Value *add = Builder.CreateAdd(arg1, arg2, "add");
    Builder.CreateRet(add);

    // verifyFunction(*sumFunc);
    TheModule->print(errs(), nullptr);

    return 0;
}
