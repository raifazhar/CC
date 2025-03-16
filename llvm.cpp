#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

int main() {
    LLVMContext Context;
    Module *TheModule = new Module("my_module", Context);
    IRBuilder<> Builder(Context);

    Type* Int32Ty = Builder.getInt32Ty();
    Type* FloatTy = Builder.getFloatTy();
    Type* Int8Ty = Builder.getInt8Ty();
    Type* Int8PtrTy = Int8Ty->getPointerTo(); // Correct way to get i8*

    // Define function: float sum(int count, ...)
    FunctionType* sumFuncType = FunctionType::get(FloatTy, {Int32Ty}, true);
    Function* sumFunc = Function::Create(sumFuncType, Function::ExternalLinkage, "sum", TheModule);
    sumFunc->arg_begin()->setName("count");

    // Create entry block
    BasicBlock* EntryBB = BasicBlock::Create(Context, "entry", sumFunc);
    Builder.SetInsertPoint(EntryBB);

    // Allocate va_list (i8**)
    Value* VAList = Builder.CreateAlloca(Int8PtrTy, nullptr, "va_list");

    // Initialize varargs
    Function* VAStart = Intrinsic::getDeclaration(TheModule, Intrinsic::vastart);
    Builder.CreateCall(VAStart, {VAList});

    // Temporary return to fix terminator (replace with actual logic)
    Builder.CreateRet(ConstantFP::get(FloatTy, 0.0f));

    TheModule->print(outs(), nullptr);
    TheModule->print(errs(), nullptr);
    delete TheModule;
    return 0;
}