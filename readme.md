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

### 2. **Global Variables and Format Strings**
- A global string constant `%f\00` is defined for printing floating-point values.
- Another global string, `Hello, World!\n`, is created for demonstration purposes.

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

### 5. **Variadic Argument Handling**
```cpp
// Allocate va_list on stack
Value *VAList = Builder.CreateAlloca(Int8PtrTy, nullptr, "va_list");

// Initialize va_list
Builder.CreateCall(VAStartFunc, VAList);
```

### 6. **Arithmetic Operations (Switch Case)**
```cpp
SwitchInst *SI = Builder.CreateSwitch(SwitchArg, DefaultBB, 4);
SI->addCase(ConstantInt::get(Type::getInt32Ty(Context), 0), AddBB); // case 0: Add
SI->addCase(ConstantInt::get(Type::getInt32Ty(Context), 1), SubBB); // case 1: Subtract
SI->addCase(ConstantInt::get(Type::getInt32Ty(Context), 2), MulBB); // case 2: Multiply
SI->addCase(ConstantInt::get(Type::getInt32Ty(Context), 3), DivBB); // case 3: Divide
```

### 7. **Function Completion**
- `llvm.va_end` is called to clean up the variadic list.
- The computed result is returned as a floating-point value.

### 8. **Main Function**
```cpp
int main() {
    printf("Hello, World!\n");
    float result = arthimatic(4, 0, 5.0, 3.0, 7.0, 0.0);
    printf("%f\n", result);
    return 0;
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
```sh
# Create a Build Directory
mkdir build && cd build

# Generate the Makefile
cmake .. -G "MinGW Makefiles"

# Build the executable
mingw32-make

# Generate LLVM IR
./llvm_arthimatic.exe > output.ll

# Run LLVM IR
lli output.ll
```

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