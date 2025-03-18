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

// Define the "Hello, World!" format string globally
Constant *HelloWorldStr = ConstantDataArray::getString(Context, "Hello, World!\n", true);
GlobalVariable *HelloWorldGV = new GlobalVariable(
    *TheModule, HelloWorldStr->getType(), true, GlobalValue::PrivateLinkage, HelloWorldStr, "hello_world"
);

//******************************************************************************/

// Define the "Format String" globally
Constant *FormatStrConst = ConstantDataArray::getString(Context, "%s %f\n\00", true);
GlobalVariable *FormatStr = new GlobalVariable(
    *TheModule, FormatStrConst->getType(), true, GlobalValue::PrivateLinkage, FormatStrConst, "format_str"
);

//******************************************************************************/

// Define the "Addition Result is" string as a global constant
Constant *AdditionResultStrConst = ConstantDataArray::getString(Context, "Addition Result is", true);
GlobalVariable *AdditionResultStr = new GlobalVariable(
    *TheModule, AdditionResultStrConst->getType(), true, GlobalValue::PrivateLinkage, AdditionResultStrConst, "addition_result_str"
);

// Define the "Subtraction Result is" string as a global constant
Constant *SubtractionResultStrConst = ConstantDataArray::getString(Context, "Subtraction Result is", true);
GlobalVariable *SubtractionResultStr = new GlobalVariable(
    *TheModule, SubtractionResultStrConst->getType(), true, GlobalValue::PrivateLinkage, SubtractionResultStrConst, "subtraction_result_str"
);

// Define the "Multiplication Result is" string as a global constant
Constant *MultiplicationResultStrConst = ConstantDataArray::getString(Context, "Multiplication Result is", true);
GlobalVariable *MultiplicationResultStr = new GlobalVariable(
    *TheModule, MultiplicationResultStrConst->getType(), true, GlobalValue::PrivateLinkage, MultiplicationResultStrConst, "multiplication_result_str"
);

// Define the "Division Result is" string as a global constant
Constant *DivisionResultStrConst = ConstantDataArray::getString(Context, "Division Result is", true);
GlobalVariable *DivisionResultStr = new GlobalVariable(
    *TheModule, DivisionResultStrConst->getType(), true, GlobalValue::PrivateLinkage, DivisionResultStrConst, "division_result_str"
);

//******************************************************************************/

// Define types for function args
Type *Int32Ty = Builder.getInt32Ty();
Type *FloatTy = Builder.getFloatTy();
Type *DoubleTy = Builder.getDoubleTy();
Type *Int8PtrTy = Builder.getInt8Ty()->getPointerTo();

//******************************************************************************/

// Get the hello string pointer
Value *HelloWorldPtr = Builder.CreateGEP(HelloWorldGV->getValueType(), HelloWorldGV,{ConstantInt::get(Int32Ty, 0), ConstantInt::get(Int32Ty, 0)});

// Get the format string pointer    // Get the format string pointer
Value *format_str_ptr = Builder.CreateGEP(FormatStrConst->getType(), FormatStr, {ConstantInt::get(Type::getInt32Ty(Context), 0), ConstantInt::get(Type::getInt32Ty(Context), 0)});

// Get a pointer to "Addition Result is"
Value *addition_str_ptr = Builder.CreateGEP(AdditionResultStrConst->getType(), AdditionResultStr, {ConstantInt::get(Type::getInt32Ty(Context), 0), ConstantInt::get(Type::getInt32Ty(Context), 0)});

// Get a pointer to "Subtraction Result is"
Value *subtraction_str_ptr = Builder.CreateGEP(SubtractionResultStrConst->getType(), SubtractionResultStr, {ConstantInt::get(Type::getInt32Ty(Context), 0), ConstantInt::get(Type::getInt32Ty(Context), 0)});

// Get a pointer to "Multiplication Result is"
Value *multiplication_str_ptr = Builder.CreateGEP(MultiplicationResultStrConst->getType(), MultiplicationResultStr, {ConstantInt::get(Type::getInt32Ty(Context), 0), ConstantInt::get(Type::getInt32Ty(Context), 0)});

// Get a pointer to "Division Result is"
Value *division_str_ptr = Builder.CreateGEP(DivisionResultStrConst->getType(), DivisionResultStr, {ConstantInt::get(Type::getInt32Ty(Context), 0), ConstantInt::get(Type::getInt32Ty(Context), 0)});




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

    //******************************************************************************/
    // Call printf with string arguments
    Builder.CreateCall(Printf, {HelloWorldPtr});

    //******************************************************************************/

    // Define the count, format, and arguments
    Value *count_Addition = ConstantInt::get(Int32Ty, 4); // count = 3
    Value *format_Addition = ConstantInt::get(Int32Ty, 0);
    Value *arg1_Addition = ConstantFP::get(DoubleTy, 3.0);
    Value *arg2_Addition = ConstantFP::get(DoubleTy, 4.0);
    Value *arg3_Addition = ConstantFP::get(DoubleTy, 7.0);
    Value *arg4_Addition = ConstantFP::get(DoubleTy, 1.0);

    // Call the arthimatic function
    Value *AdditionResult_Ptr = Builder.CreateCall(arthimaticFunc, {count_Addition,format_Addition, arg1_Addition, arg2_Addition, arg3_Addition, arg4_Addition},"Addition_result_ptr");
    Value *AdditionResult_Double = Builder.CreateFPExt(AdditionResult_Ptr, DoubleTy, "Addition_result_double");

    // Call printf with correct arguments
    Value *printf_Addition_call = Builder.CreateCall(Printf, {format_str_ptr, addition_str_ptr, AdditionResult_Double});

    //******************************************************************************/

    // Define the count, format, and arguments
    Value *count_Subtraction = ConstantInt::get(Int32Ty, 4); // count = 3
    Value *format_Subtraction = ConstantInt::get(Int32Ty, 1);
    Value *arg1_Subtraction = ConstantFP::get(DoubleTy, 3.0);
    Value *arg2_Subtraction = ConstantFP::get(DoubleTy, 4.0);
    Value *arg3_Subtraction = ConstantFP::get(DoubleTy, 7.0);
    Value *arg4_Subtraction = ConstantFP::get(DoubleTy, 1.0);

    // Call the arthimatic function
    Value *SubtractionResult_Ptr = Builder.CreateCall(arthimaticFunc, {count_Subtraction,format_Subtraction, arg1_Subtraction, arg2_Subtraction, arg3_Subtraction, arg4_Subtraction},"Subtraction_result_ptr");
    Value *SubtractionResult_Double = Builder.CreateFPExt(SubtractionResult_Ptr, DoubleTy, "Subtraction_result_double");

    // Call printf with correct arguments
    Builder.CreateCall(Printf, {format_str_ptr, subtraction_str_ptr, SubtractionResult_Double});

    //******************************************************************************/

    // Define the count, format, and arguments
    Value *count_Multiplication = ConstantInt::get(Int32Ty, 4); // count = 3
    Value *format_Multiplication = ConstantInt::get(Int32Ty, 2);
    Value *arg1_Multiplication = ConstantFP::get(DoubleTy, 3.0);
    Value *arg2_Multiplication = ConstantFP::get(DoubleTy, 4.0);
    Value *arg3_Multiplication = ConstantFP::get(DoubleTy, 7.0);
    Value *arg4_Multiplication = ConstantFP::get(DoubleTy, 1.0);

    // Call the arthimatic function
    Value *MultiplicationResult_Ptr = Builder.CreateCall(arthimaticFunc, {count_Multiplication,format_Multiplication, arg1_Multiplication, arg2_Multiplication, arg3_Multiplication, arg4_Multiplication},"Multiplication_result_ptr");
    Value *MultiplicationResult_Double = Builder.CreateFPExt(MultiplicationResult_Ptr, DoubleTy, "Multiplication_result_double");

    // Call printf with correct arguments
    Builder.CreateCall(Printf, {format_str_ptr, multiplication_str_ptr, MultiplicationResult_Double});

    //******************************************************************************/

    // Define the count, format, and arguments
    Value *count_Division = ConstantInt::get(Int32Ty, 4); // count = 3
    Value *format_Division = ConstantInt::get(Int32Ty, 3);
    Value *arg1_Division = ConstantFP::get(DoubleTy, 3.0);
    Value *arg2_Division = ConstantFP::get(DoubleTy, 4.0);
    Value *arg3_Division = ConstantFP::get(DoubleTy, 7.0);
    Value *arg4_Division = ConstantFP::get(DoubleTy, 1.0);

    // Call the arthimatic function
    Value *DivisionResult_Ptr = Builder.CreateCall(arthimaticFunc, {count_Division,format_Division, arg1_Division, arg2_Division, arg3_Division, arg4_Division},"Division_result_ptr");
    Value *DivisionResult_Double = Builder.CreateFPExt(DivisionResult_Ptr, DoubleTy, "Division_result_double");

    // Call printf with correct arguments
    Builder.CreateCall(Printf, {format_str_ptr, division_str_ptr, DivisionResult_Double});

    //******************************************************************************/

    // Return 0
    Builder.CreateRet(ConstantInt::get(Int32Ty, 0));

    // Print the IR
    TheModule->print(outs(), nullptr);


    delete TheModule;
    return 0;
}
