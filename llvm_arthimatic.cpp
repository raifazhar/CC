#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//Intialize the Context, Module, and Builder
LLVMContext Context;
Module *TheModule = new Module("my_module", Context);
IRBuilder<> Builder(Context);

//******************************************************************************/

Constant *FormatStrConst = ConstantDataArray::getString(Context, "%f\00", true);
GlobalVariable *FormatStr = new GlobalVariable(
    *TheModule, FormatStrConst->getType(), true, GlobalValue::PrivateLinkage, FormatStrConst, "format_str"
);

//******************************************************************************/

    // Define the "Hello, World!" format string globally
Constant *HelloWorldStr = ConstantDataArray::getString(Context, "Hello, World!\n", true);
GlobalVariable *HelloWorldGV = new GlobalVariable(
    *TheModule, HelloWorldStr->getType(), true, GlobalValue::PrivateLinkage, HelloWorldStr, "hello_world"
);


//******************************************************************************/

// Define types for function args
Type *Int32Ty = Builder.getInt32Ty();
Type *FloatTy = Builder.getFloatTy();
Type *DoubleTy = Builder.getDoubleTy();
Type *Int8PtrTy = Builder.getInt8Ty()->getPointerTo();

int main() {

    // Define function: float sum(int count, ...)
    std::vector<Type*> params = { Int32Ty,Int32Ty };
    FunctionType* arthimaticFuncType = FunctionType::get(FloatTy, params, true);
    Function *arthimaticFunc = Function::Create(arthimaticFuncType, Function::ExternalLinkage, "arthimatic", TheModule);

    // Name the first (fixed) argument
    Function::arg_iterator args = arthimaticFunc->arg_begin();
    Value *countArg = args++;
    countArg->setName("count");

    Value *SwitchArg = args++;
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

    
    //******************************************************************************/

    // Allocate memory for the sum (initialize with 0)
    Value *result_ptr = Builder.CreateAlloca(FloatTy, nullptr, "result_ptr");
    Builder.CreateStore(ConstantFP::get(FloatTy, 0.0), result_ptr);  // Store initial sum value

    // Allocate memory to store a mutable count
    Value *CountPtr = Builder.CreateAlloca(Int32Ty, nullptr, "count_ptr");
    Builder.CreateStore(countArg, CountPtr);  // Store the initial count value

    // Allocate memory to Swtich value
    Value *SwitchPtr = Builder.CreateAlloca(Int8PtrTy, nullptr, "Switch_ptr");
    Builder.CreateStore(SwitchArg, SwitchPtr);  // Store the initial count value


    //******************************************************************************/

    // Create basic blocks for the loop and end
    BasicBlock *LoopBB = BasicBlock::Create(Context, "loop", arthimaticFunc);
    BasicBlock *EndBB = BasicBlock::Create(Context, "end", arthimaticFunc);

    // Switch based on the operation (0 = Add, 1 = Subtract, 2 = Multiply, 3 = Divide)
    BasicBlock *AddBB = BasicBlock::Create(Context, "Add", arthimaticFunc);
    BasicBlock *SubBB = BasicBlock::Create(Context, "Sub", arthimaticFunc);
    BasicBlock *MulBB = BasicBlock::Create(Context, "Mul", arthimaticFunc);
    BasicBlock *DivBB = BasicBlock::Create(Context, "Div", arthimaticFunc);

    // Create basic blocks for the result one and skip result one
    BasicBlock *MulIntialize = BasicBlock::Create(Context, "MulIntialize", arthimaticFunc);

    // Create basic blocks for the div intialize and skip div intialize
    BasicBlock *DivIntialize = BasicBlock::Create(Context, "DivIntialize", arthimaticFunc);

    // Create basic blocks for the add or sub intialize
    BasicBlock *AddOrSubIntialize = BasicBlock::Create(Context, "AddOrSubIntialize", arthimaticFunc);

    // Create basic blocks for the end condition and end
    BasicBlock *EndConditonBB = BasicBlock::Create(Context, "endcondition", arthimaticFunc);

    //******************************************************************************/

    SwitchInst *SwitchResult = Builder.CreateSwitch(SwitchArg, AddOrSubIntialize, 2);

    SwitchResult->addCase(ConstantInt::get(Type::getInt32Ty(Context), 2), MulIntialize);
    SwitchResult->addCase(ConstantInt::get(Type::getInt32Ty(Context), 3), DivIntialize);

    //**************************************************************************** */

    // Set the insertion point to the mul intialize
    Builder.SetInsertPoint(MulIntialize);
    Builder.CreateStore(ConstantFP::get(FloatTy, 1.0), result_ptr); 
    Builder.CreateBr(AddOrSubIntialize);

    //**************************************************************************** */

    // Set the insertion point to the div intialize
    Builder.SetInsertPoint(DivIntialize);
    
    Value *NextDivArg = Builder.CreateVAArg(VAListCasted, Type::getDoubleTy(Context), "next_arg");
    NextDivArg = Builder.CreateFPTrunc(NextDivArg, FloatTy, "truncated");

    Value *CurrentDivCount = Builder.CreateLoad(Int32Ty, CountPtr, "current_count");
    Value *NewDivCount = Builder.CreateSub(CurrentDivCount, ConstantInt::get(Int32Ty, 1));

    Builder.CreateStore(NewDivCount, CountPtr);  // Store updated value
    Builder.CreateStore(NextDivArg, result_ptr);  // Corrected to 1.0 instead of 0.0

    Builder.CreateBr(AddOrSubIntialize);

    //******************************************************************************/

    // Set the insertion point to the add or sub intialize
    Builder.SetInsertPoint(AddOrSubIntialize);

    //******************************************************************************/

    // Create the branch to the loop block
    Builder.CreateBr(LoopBB);

    // Set the insertion point to the loop block
    Builder.SetInsertPoint(LoopBB);
    
    //******************************************************************************/

    // Load current count
    Value *CurrentCount = Builder.CreateLoad(Int32Ty, CountPtr, "current_count");

    // Extract variadic argument as double, then truncate to float
    Value *NextArg = Builder.CreateVAArg(VAListCasted, Type::getDoubleTy(Context), "next_arg");
    NextArg = Builder.CreateFPTrunc(NextArg, FloatTy, "truncated");

    // Load current sum, add the truncated value, and store it back
    Value *CurrentResult = Builder.CreateLoad(FloatTy, result_ptr, "current_result");

    // Create the switch instruction and set the default block to EndBB
    SwitchInst *SwitchInst = Builder.CreateSwitch(SwitchArg, EndBB, 4);

    // Create the switch instruction and set the default block to EndBB
    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 0), AddBB); // case 0: Add

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 1), SubBB); // case 1: Subtract

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 2), MulBB); // case 2: Multiply

    SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 3), DivBB); // case 3: Divide

    //******************************************************************************/

    // Set the insertion point to the add block
    Builder.SetInsertPoint(AddBB);
    Value *NewResultAdd = Builder.CreateFAdd(CurrentResult, NextArg, "new_result");
    Builder.CreateStore(NewResultAdd, result_ptr); 
    Builder.CreateBr(EndConditonBB);    

    //******************************************************************************/

    // Set the insertion point to the sub block
    Builder.SetInsertPoint(SubBB);
    Value *NewResultSub = Builder.CreateFSub(CurrentResult, NextArg, "new_result"); 
    Builder.CreateStore(NewResultSub, result_ptr); 
    Builder.CreateBr(EndConditonBB);

    //******************************************************************************/

    // Set the insertion point to the mul block
    Builder.SetInsertPoint(MulBB); // Reload after correction
    Value *NewResultMul = Builder.CreateFMul(CurrentResult, NextArg, "new_result");
    Builder.CreateStore(NewResultMul, result_ptr); 
    Builder.CreateBr(EndConditonBB);

    //******************************************************************************/

    // Set the insertion point to the div block
    Builder.SetInsertPoint(DivBB);
    Value *NewResultDiv = Builder.CreateFDiv(CurrentResult, NextArg, "new_result");


    Builder.CreateStore(NewResultDiv, result_ptr); 
    Builder.CreateBr(EndConditonBB);

    //******************************************************************************/

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

    //******************************************************************************/

    // Declare printf function to accept (char*)
    FunctionType *PrintfType = FunctionType::get(
        Builder.getInt32Ty(), 
        {Builder.getInt8Ty()->getPointerTo()},  // Only the format string
        true  // Variadic function
    );


    FunctionCallee Printf = TheModule->getOrInsertFunction("printf", PrintfType);
    //Function *Printf=Function::Create(PrintfType, Function::ExternalLinkage, "printf", TheModule);


    // Inside main, get the string pointer
    Value *HelloWorldPtr = Builder.CreateGEP(
        HelloWorldGV->getValueType(), HelloWorldGV, 
        {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)}
    );

    // Call printf with string arguments
    Builder.CreateCall(Printf, {HelloWorldPtr});

    //******************************************************************************/

    // Define the count, format, and arguments
    Value *count = ConstantInt::get(Int32Ty, 4); // count = 3
    Value *format = ConstantInt::get(Int32Ty, 0);
    Value *arg1 = ConstantFP::get(DoubleTy, 3.0);
    Value *arg2 = ConstantFP::get(DoubleTy, 4.0);
    Value *arg3 = ConstantFP::get(DoubleTy, 7.0);
    Value *arg4 = ConstantFP::get(DoubleTy, 1.0);

    //******************************************************************************/
    // Call the arthimatic function
    Value *resultPtr = Builder.CreateCall(arthimaticFunc, {count,format, arg1, arg2, arg3, arg4},"result");
    Value *result_double = Builder.CreateFPExt(resultPtr, DoubleTy, "result_double");

    // Get the format string pointer
    Value *format_str_ptr = Builder.CreateGEP(FormatStr->getValueType(), FormatStr, {ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)});

    // Call printf with string arguments
    Value *printf_call = Builder.CreateCall(Printf, {format_str_ptr, result_double});

    // Return the printf call
    Builder.CreateRet(printf_call);

    //******************************************************************************/
    // Print the IR
    TheModule->print(outs(), nullptr);


    delete TheModule;
    return 0;
}
