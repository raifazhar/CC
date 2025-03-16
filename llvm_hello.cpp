#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int main() {
    LLVMContext Context;
    Module *TheModule = new Module("HelloModule", Context);
    IRBuilder<> Builder(Context);

    // Declare printf function
    FunctionType *PrintfType = FunctionType::get( Builder.getInt32Ty(), {Builder.getInt8Ty()->getPointerTo()}, true);
    FunctionCallee Printf = TheModule->getOrInsertFunction("printf", PrintfType);

    // Create a global string "Hello, World!\n"
    Constant *HelloWorldStr = ConstantDataArray::getString(Context, "Hello, World!\n", true);
    GlobalVariable *HelloWorldGV = new GlobalVariable( *TheModule, HelloWorldStr->getType(), true, GlobalValue::PrivateLinkage, HelloWorldStr, "hello_world");

    // Create a main function
    FunctionType *MainType = FunctionType::get(Builder.getInt32Ty(), false);
    Function *MainFunc = Function::Create(MainType, Function::ExternalLinkage, "main", TheModule);
    BasicBlock *EntryBB = BasicBlock::Create(Context, "entry", MainFunc);
    Builder.SetInsertPoint(EntryBB);

    // Get pointer to "Hello, World!" string
    Value *HelloWorldPtr = Builder.CreatePointerCast(HelloWorldGV, Builder.getInt8Ty()->getPointerTo());

    // Call printf
    Builder.CreateCall(Printf, {HelloWorldPtr});

    // Return 0
    Builder.CreateRet(Builder.getInt32(0));

    // Print the generated LLVM IR
    TheModule->print(outs(), nullptr);
    delete TheModule;
    return 0;
}
