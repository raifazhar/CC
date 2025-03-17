# LLVM IR Code Explanation

## 📖 Table of Contents
- [Overview](#overview)
- [Key Components](#key-components)
- [Compilation Steps](#-compilation-steps)
- [Problems Encountered](#-problems-while-constructing-code)

## 📝 Overview
This program is written in C++ and utilizes the LLVM framework to generate LLVM Intermediate Representation (IR) for a variadic arithmetic function. The generated function performs different arithmetic operations based on an opcode. It supports addition, subtraction, multiplication, and division.

## 🔧 Key Components

### 1. **LLVM Context, Module, and IRBuilder**
- The program initializes an `LLVMContext`, `Module`, and `IRBuilder`.
- `LLVMContext` is a global context object required for creating LLVM structures.
- `Module` represents a container for all LLVM IR objects (functions, global variables, etc.).
- `IRBuilder` is used to generate LLVM instructions in an easy-to-use manner.
```cpp
  LLVMContext Context;
  Module *TheModule = new Module("my_module", Context);
  IRBuilder<> Builder(Context);
```

### 2. **Global Variables and Format Strings**
- A global string constant `%f\00` is defined for printing floating-point values.
- Another global string, `Hello, World!\n`, is created for demonstration purposes.

```cpp
  Constant *FormatStrConst = ConstantDataArray::getString(Context, "%f\00", true);
  GlobalVariable *FormatStr = new GlobalVariable(*TheModule, FormatStrConst->getType(), true, GlobalValue::PrivateLinkage, FormatStrConst, "format_str");

  Constant *HelloWorldStr = ConstantDataArray::getString(Context, "Hello, World!\n", true);
  GlobalVariable *HelloWorldGV = new GlobalVariable(*TheModule, HelloWorldStr->getType(), true GlobalValue::PrivateLinkage, HelloWorldStr, "hello_world");
```

### 3. **Defining Types**
| LLVM Type | Description |
|-----------|------------|
| `Int32Ty` | 32-bit integer |
| `FloatTy` | 32-bit floating-point number |
| `DoubleTy` | 64-bit floating-point number |
| `Int8PtrTy` | Character pointer |

### 4. **Arithmetic Function Definition**
- The function `arthimatic(int count, int SwitchArg, ...)` is declared:
  - `count`: Number of variadic arguments.
  - `SwitchArg`: Determines the operation to be performed.
  - The function accepts a variable number of arguments.
```cpp
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

```

### 5. **Variadic Argument Handling**
```cpp
// Allocate va_list
Value *VAList = Builder.CreateAlloca(Int8PtrTy, nullptr, "va_list");

// Get llvm.va_start declaration
FunctionType *VAStartType = FunctionType::get(Type::getVoidTy(Context), {Int8PtrTy}, false);
FunctionCallee VAStart = TheModule->getOrInsertFunction("llvm.va_start", VAStartType);

// Bitcast VAList to match expected i8* argument
Value *VAListCasted = Builder.CreateBitCast(VAList, Int8PtrTy);
Builder.CreateCall(VAStart, {VAListCasted});

```

### 6. **Arithmetic Operations (Switch Case)**
```cpp
SwitchInst *SwitchInst = Builder.CreateSwitch(SwitchArg, EndBB, 4);

// Create the switch instruction and set the default block to EndBB
SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 0), AddBB); // case 0: Add

SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 1), SubBB); // case 1: Subtract

SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 2), MulBB); // case 2: Multiply

SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 3), DivBB); // case 3: Divide

```

### 7. **Function Completion**
- `llvm.va_end` is called to clean up the variadic list.
- The computed result is returned as a floating-point value.

```cpp
FunctionCallee VAEnd = TheModule->getOrInsertFunction("llvm.va_end", VAStartType);
Builder.CreateCall(VAEnd, {VAListCasted});
```

### 8. **Main Function**
```cpp
int main() {
    printf("Hello, World!\n");
    float result = arthimatic(4, 0, 5.0, 3.0, 7.0, 0.0);
    printf("%f\n", result);
    return 0;
}
```

```IR
define i32 @main() {
entry:
  %0 = call i32 (ptr, ...) @printf(ptr @hello_world)
  %result = call float (i32, i32, ...) @arthimatic(i32 4, i32 0, double 3.000000e+00, double 4.000000e+00, double 7.000000e+00, double 1.000000e+00)
  %result_double = fpext float %result to double
  %1 = call i32 (ptr, ...) @printf(ptr @format_str, double %result_double)
  ret i32 %1
}
```

---

# 🛠 Compilation Steps

### 📌 Prerequisites
| Dependency | Version |
|------------|---------|
| **CMake**  | 3.31+   |
| **MinGW**  | Any     |
| **LLVM**   | Latest  |

### 🚀 Steps
1. **Create a Build Directory** and navigate into it.
   ```sh
   mkdir build && cd build
   ```
2. **Generate the Makefile** using the following command:
   ```sh
   cmake .. -G "MinGW Makefiles"
   ```
   This will create the required `Makefile`.
3. **Build the executable** using:
   ```sh
   mingw32-make
   ```
   This will generate `llvm_arthimatic.exe`.
4. **Run the executable and generate LLVM IR**:
   ```sh
   ./llvm_arthimatic.exe > output.ll
   ```
   This will execute `llvm_arthimatic.exe` and store the LLVM IR in `output.ll`.
5. **Run the generated LLVM IR using `lli`**:
   ```sh
   lli output.ll
   ```
   This will execute the IR and display the output.

---

# ⚠️ Problems While Constructing Code

## ❌ Could Not Access `va_start`

To access `va_start`, you need to use `llvm/va_start` by inserting and calling the function. Initially, I encountered an issue where `va_start` was not accessible. I first attempted to use:

```cpp
Function *getDeclaration(); // Deprecated
```

However, this function is now **deprecated**. Instead, I resolved the issue using:

```cpp
getOrInsertFunction();
```

---

## 🔄 Creating `switch` Statements

While implementing a `switch` statement to determine the arithmetic operation, I encountered an error when adding `case` statements. The problematic line:

```cpp
SwitchInst->addCase(ConstantInt::get(Int32Ty, 0), AddBB);
```

This caused a **type compatibility error**. I resolved it by explicitly casting the type:

```diff
- SwitchInst->addCase(ConstantInt::get(Int32Ty, 0), AddBB);
+ SwitchInst->addCase(ConstantInt::get(Type::getInt32Ty(Context), 0), AddBB);
```

---

## ➗ Division Issue

For multiplication and division, I initially assigned a default value to `result`. Specifically:
- **Multiplication (`*`)**: Set `result = 1`
- **Division (`/`)**: Set `result` to the first argument of the variable list.

However, in the **division case**, I forgot to decrement the argument count. As a result, the loop iterated beyond the available arguments, causing the code to access an invalid value, leading to **infinity (`inf`)** in the division operation.

**Fix:** Adjusted the loop to correctly track argument count and avoid accessing past the last valid value.

---

## 🎯 Conclusion
This program showcases how LLVM can be used to generate and manipulate LLVM IR dynamically, enabling custom compilation pipelines and optimization techniques.

---