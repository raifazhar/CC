#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

    LLVMContext Context;
    Module *TheModule = new Module("my_module", Context);
    IRBuilder<> Builder(Context);

    GlobalVariable *FormatStr = new GlobalVariable(
    *TheModule,
    ArrayType::get(Type::getInt8Ty(Context), 3),  // The array type for the format string
    true,  // isConstant
    GlobalValue::PrivateLinkage,  // Linkage type
    ConstantDataArray::getString(Context, "%f\00", true),  // The format string constant
    "format_str"  // Name of the global variable
    );

    // Define types for function args
    Type *Int32Ty = Builder.getInt32Ty();
    Type *FloatTy = Builder.getFloatTy();
    Type *DoubleTy = Builder.getDoubleTy();
    Type *Int8PtrTy = Builder.getInt8Ty()->getPointerTo();

int main() {



    //******************************************************************************/
    // Define function: float sum(int count, ...)
    std::vector<Type*> params = { Int32Ty,Int32Ty };
    FunctionType* arthimaticFuncType = FunctionType::get(FloatTy, params, true);
    Function *arthimaticFunc = Function::Create(arthimaticFuncType, Function::ExternalLinkage, "arthimatic", TheModule);

    // Name the first (fixed) argument
    Function::arg_iterator args = arthimaticFunc->arg_begin();
    Value *countArg = &*args++;
    countArg->setName("count");

    Value *SwitchArg = &*args++;
    SwitchArg->setName("SwitchArg");

    // Create function entry block
    BasicBlock* arthimaticEntryBB = BasicBlock::Create(Context, "entry", arthimaticFunc); // Create the entry block for the function
    Builder.SetInsertPoint(arthimaticEntryBB);

    //******************************************************************************/


    // Allocate va_list
    Value *VAList = Builder.CreateAlloca(Int8PtrTy, nullptr, "va_list");

    // Get llvm.va_start declaration
    FunctionType *VAStartType = FunctionType::get(Type::getVoidTy(Context), {Int8PtrTy}, false);
    FunctionCallee VAStart = TheModule->getOrInsertFunction("llvm.va_start", VAStartType);

    // Bitcast VAList to match expected i8* argument
    Value *VAListCasted = Builder.CreateBitCast(VAList, Int8PtrTy);
    Builder.CreateCall(VAStart, {VAListCasted});
    
    // Allocate memory for the sum (initialize with 0)
    Value *result_ptr = Builder.CreateAlloca(FloatTy, nullptr, "result_ptr");
    Builder.CreateStore(ConstantFP::get(FloatTy, 0.0), result_ptr);  // Store initial sum value

    // Allocate memory to store a mutable count
    Value *CountPtr = Builder.CreateAlloca(Int32Ty, nullptr, "count_ptr");
    Builder.CreateStore(countArg, CountPtr);  // Store the initial count value

    Value *SwitchPtr = Builder.CreateAlloca(Int8PtrTy, nullptr, "Switch_ptr");
    Builder.CreateStore(SwitchArg, SwitchPtr);  // Store the initial count value


    //******************************************************************************/



    BasicBlock *LoopBB = BasicBlock::Create(Context, "loop", arthimaticFunc);
    BasicBlock *EndBB = BasicBlock::Create(Context, "end", arthimaticFunc);

    // Switch based on the operation (0 = Add, 1 = Subtract, 2 = Multiply, 3 = Divide)
    BasicBlock *AddBB = BasicBlock::Create(Context, "Add", arthimaticFunc);
    BasicBlock *SubBB = BasicBlock::Create(Context, "Sub", arthimaticFunc);
    BasicBlock *MulBB = BasicBlock::Create(Context, "Mul", arthimaticFunc);
    BasicBlock *DivBB = BasicBlock::Create(Context, "Div", arthimaticFunc);

    BasicBlock *ResultOneBB = BasicBlock::Create(Context, "ResultOne", arthimaticFunc);
    BasicBlock *SkipResultOneBB = BasicBlock::Create(Context, "SkipResultOne", arthimaticFunc);

    BasicBlock *DivIntialize = BasicBlock::Create(Context, "DivIntialize", arthimaticFunc);
    BasicBlock *SkipDivIntialize = BasicBlock::Create(Context, "SkipDivIntialize", arthimaticFunc);

    BasicBlock *EndConditonBB = BasicBlock::Create(Context, "end", arthimaticFunc);

    //******************************************************************************/

    Builder.CreateBr(LoopBB);
    Builder.SetInsertPoint(LoopBB);
    

    // Load current count
    Value *CurrentCount = Builder.CreateLoad(Int32Ty, CountPtr, "current_count");

    // Extract variadic argument as double, then truncate to float
    Value *NextArg = Builder.CreateVAArg(VAListCasted, Type::getDoubleTy(Context), "next_arg");
    NextArg = Builder.CreateFPTrunc(NextArg, FloatTy, "truncated");

    // Load current sum, add the truncated value, and store it back
    Value *CurrentResult = Builder.CreateLoad(FloatTy, result_ptr, "current_result");
        // Create the switch instruction and set the default block to EndBB
    SwitchInst *SwitchInst = Builder.CreateSwitch(SwitchArg, EndBB, 4);

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 0), AddBB); // case 0: Add

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 1), SubBB); // case 1: Subtract

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 2), MulBB); // case 2: Multiply

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 3), DivBB); // case 3: Divide


    Builder.SetInsertPoint(AddBB);
    Value *NewResultAdd = Builder.CreateFAdd(CurrentResult, NextArg, "new_result");
    Builder.CreateStore(NewResultAdd, result_ptr); 
    Builder.CreateBr(EndConditonBB);    

    Builder.SetInsertPoint(SubBB);
    Value *NewResultSub = Builder.CreateFSub(CurrentResult, NextArg, "new_result"); 
    Builder.CreateStore(NewResultSub, result_ptr); 
    Builder.CreateBr(EndConditonBB);

    Builder.SetInsertPoint(MulBB);

    // Compare if CurrentResult == 0.0 (use FCmpOEQ for floating-point comparison)
    Value *CondMul = Builder.CreateFCmpOEQ(CurrentResult, ConstantFP::get(FloatTy, 0.0), "cond_mul");
    Builder.CreateCondBr(CondMul, ResultOneBB, SkipResultOneBB);

    // If result is zero, set it to 1.0
    Builder.SetInsertPoint(ResultOneBB);
    Builder.CreateStore(ConstantFP::get(FloatTy, 1.0), result_ptr);  // Corrected to 1.0 instead of 0.0
    Builder.CreateBr(SkipResultOneBB);

    Builder.SetInsertPoint(SkipResultOneBB);
    Value *UpdatedCurrentResult = Builder.CreateLoad(FloatTy, result_ptr, "updated_current_result");  // Reload after correction
    Value *NewResultMul = Builder.CreateFMul(UpdatedCurrentResult, NextArg, "new_result");
    Builder.CreateStore(NewResultMul, result_ptr); 
    Builder.CreateBr(EndConditonBB);

    Builder.SetInsertPoint(DivBB);
    Value *CondDiv = Builder.CreateFCmpOEQ(CurrentResult, ConstantFP::get(FloatTy, 0.0), "cond_div");
    Builder.CreateCondBr(CondDiv, DivIntialize, SkipDivIntialize);

    Builder.SetInsertPoint(DivIntialize);
    Builder.CreateStore(NextArg, result_ptr);  // Corrected to 1.0 instead of 0.0
    Builder.CreateBr(EndConditonBB);
    
    Builder.SetInsertPoint(SkipDivIntialize);

    Value *NewResultDiv = Builder.CreateFDiv(CurrentResult, NextArg, "new_result");
    Builder.CreateStore(NewResultDiv, result_ptr); 
    Builder.CreateBr(EndConditonBB);

    Builder.SetInsertPoint(EndConditonBB);

    // Decrement count
    Value *NewCount = Builder.CreateSub(CurrentCount, ConstantInt::get(Int32Ty, 1));
    Builder.CreateStore(NewCount, CountPtr);  // Store updated value

    // Condition check for termination
    Value *Cond = Builder.CreateICmpEQ(NewCount, ConstantInt::get(Int32Ty, 0));
    Builder.CreateCondBr(Cond, EndBB, LoopBB);

    //******************************************************************************/

    // End block
    Builder.SetInsertPoint(EndBB);
    FunctionCallee VAEnd = TheModule->getOrInsertFunction("llvm.va_end", VAStartType);
    Builder.CreateCall(VAEnd, {VAListCasted});


    // Load final sum and return it
    Value *FinalSum = Builder.CreateLoad(FloatTy, result_ptr, "final_sum");
    Builder.CreateRet(FinalSum);

    //******************************************************************************/

    // Define Main Function
    FunctionType *MainFuncType = FunctionType::get(Builder.getInt32Ty(), false);
    Function *MainFunc = Function::Create(MainFuncType, Function::ExternalLinkage, "main", TheModule);

    // Create entry block for main function
    BasicBlock *MainEntryBB = BasicBlock::Create(Context, "entry", MainFunc);
    Builder.SetInsertPoint(MainEntryBB);

    // Declare printf function (external function)  
    FunctionType *PrintfType = FunctionType::get(Int32Ty, {Int8PtrTy, DoubleTy}, true);
    FunctionCallee Printf = TheModule->getOrInsertFunction("printf", PrintfType);  

    Value *count = ConstantInt::get(Int32Ty, 3); // count = 3
    Value *format = ConstantInt::get(Int32Ty, 1);
    Value *arg1 = ConstantFP::get(DoubleTy, 3.0);
    Value *arg2 = ConstantFP::get(DoubleTy, 2.0);
    Value *arg3 = ConstantFP::get(DoubleTy, 7.0);

    Value *resultPtr = Builder.CreateCall(arthimaticFunc, {count,format, arg1, arg2, arg3},"result");
    Value *result_double = Builder.CreateFPExt(resultPtr, DoubleTy, "result_double");

    Value *format_str_ptr = Builder.CreateGEP(FormatStr->getValueType(), FormatStr, {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)});

    Value *printf_call = Builder.CreateCall(Printf, {format_str_ptr, result_double});

    Builder.CreateRet(printf_call);
    // Print the IR
    TheModule->print(outs(), nullptr);


    delete TheModule;
    return 0;
}
