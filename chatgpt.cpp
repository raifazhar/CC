#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"

using namespace llvm;

int main() {
    LLVMContext Context;
    Module *Mod = new Module("my_module", Context);
    IRBuilder<> Builder(Context);

    // Define float and int types
    Type *FloatTy = Type::getFloatTy(Context);
    Type *Int32Ty = Type::getInt32Ty(Context);

    // Create function type: float (int, ...)
    std::vector<Type*> paramTypes = { Int32Ty };
    FunctionType *FT = FunctionType::get(FloatTy, paramTypes, true);
    
    // Create function
    Function *Func = Function::Create(FT, Function::ExternalLinkage, "myFunction", Mod);
    
    // Name the first argument (fixed argument)
    auto ArgIter = Func->arg_begin();
    Value *CountArg = ArgIter;
    CountArg->setName("count");

    // Create a basic block
    BasicBlock *EntryBB = BasicBlock::Create(Context, "entry", Func);
    Builder.SetInsertPoint(EntryBB);

    // Allocate va_list (use i8* for va_list in LLVM)
    Value *VAList = Builder.CreateAlloca(Type::getInt8Ty(Context), nullptr, "va_list");

    // Call llvm.va_start
    Function *VAStart = Intrinsic::getDeclaration(Mod, Intrinsic::vastart);
    Builder.CreateCall(VAStart, {VAList});

    // Loop through arguments
    Value *Sum = ConstantFP::get(FloatTy, 0.0);

    BasicBlock *LoopBB = BasicBlock::Create(Context, "loop", Func);
    BasicBlock *EndBB = BasicBlock::Create(Context, "end", Func);

    Builder.CreateBr(LoopBB);
    Builder.SetInsertPoint(LoopBB);

    // Get next variadic argument as double (float is promoted to double)
    Function *VAArgFunc = Intrinsic::getDeclaration(Mod, Intrinsic::vaarg);
    Value *NextArg = Builder.CreateCall(VAArgFunc, {VAList}, "next_arg");
    NextArg = Builder.CreateFPTrunc(NextArg, FloatTy); // Convert to float

    Sum = Builder.CreateFAdd(Sum, NextArg, "sum");

    // Check loop condition (this part assumes CountArg is used as loop counter)
    Value *NewCount = Builder.CreateSub(CountArg, ConstantInt::get(Int32Ty, 1));
    Builder.CreateCondBr(Builder.CreateICmpEQ(NewCount, ConstantInt::get(Int32Ty, 0)), EndBB, LoopBB);
    
    // End block
    Builder.SetInsertPoint(EndBB);
    Function *VAEnd = Intrinsic::getDeclaration(Mod, Intrinsic::vaend);
    Builder.CreateCall(VAEnd, {VAList});
    Builder.CreateRet(Sum);

    // Print the IR
    Mod->print(outs(), nullptr);

    delete Mod;
    return 0;
}
