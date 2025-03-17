# LLVM IR Code Explanation

## Overview
This program is written in C++ and utilizes the LLVM framework to generate LLVM Intermediate Representation (IR) for a variadic arithmetic function. The generated function performs different arithmetic operations based on a opcode it supports addition, subtraction, multiplication, and division.

## Key Components

### 1. **LLVM Context, Module, and IRBuilder**
- The program initializes an `LLVMContext`, `Module`, and `IRBuilder`.
- `LLVMContext` is a global context object required for creating LLVM structures.
- `Module` represents a container for all LLVM IR objects (functions, global variables, etc.).
- `IRBuilder` is used to generate LLVM instructions in an easy-to-use manner.

### 2. **Global Variables and Format Strings**
- A global string constant `%f\00` is defined for printing floating-point values.
- Another global string, `Hello, World!\n`, is created for demonstration purposes.

### 3. **Defining Types**
- Several LLVM types are defined:
  - `Int32Ty` for 32-bit integers
  - `FloatTy` for 32-bit floating-point numbers
  - `DoubleTy` for 64-bit floating-point numbers
  - `Int8PtrTy` for character pointers

### 4. **Arithmetic Function Definition**
- The function `arthimatic(int count, int SwitchArg, ...)` is declared:
  - `count`: Number of variadic arguments.
  - `SwitchArg`: Determines the operation to be performed.
  - The function accepts a variable number of arguments.

### 5. **Variadic Argument Handling**
- `va_list` is allocated on the stack.
- `llvm.va_start` is used to initialize the variadic argument list.
- The function iterates through all arguments and performs operations accordingly.

### 6. **Arithmetic Operations (Switch Case)**
- The switch statement decides which operation to perform:
  - `0`: Addition (`FAdd`)
  - `1`: Subtraction (`FSub`)
  - `2`: Multiplication (`FMul`)
  - `3`: Division (`FDiv`)
- Special initialization cases exist for multiplication (result initialized to 1) and division (first argument used as the initial value).
- The loop iterates through the arguments and applies the selected operation.

### 7. **Function Completion**
- `llvm.va_end` is called to clean up the variadic list.
- The computed result is returned as a floating-point value.

### 8. **Main Function**
- A `main` function is created:
  - Calls `printf` to print "Hello, World!".
  - Calls the `arthimatic` function with 4 arguments.
  - Prints the result using `printf`.
- Finally, the generated LLVM IR is printed.

## Output
The program generates LLVM IR, which can be compiled and executed with an LLVM-based backend to generate machine code.

## Dependencies
- LLVM development libraries and headers.
- C++ compiler with LLVM support.

## Compilation and Execution
1. Compile the program using `clang++` with LLVM flags.
2. Run the compiled executable to generate and print LLVM IR.
3. Use `lli` to execute the IR if required.

# LLVM Arithmetic Compiler Guide

This guide walks you through compiling and running the **LLVM Arithmetic Compiler** using CMake and MinGW Makefiles.

---

## 📌 Prerequisites

Ensure you have the following installed:
- **CMake** (Version 3.31 or later)
- **MinGW** (for `mingw32-make`)
- **LLVM** (for `lli` to execute LLVM IR)

---

## ⚙️ Compilation Steps

### **1️⃣ Create and Enter the Build Directory**
First, create a `build` directory and navigate into it:
```sh
mkdir build && cd build

2️⃣ Generate the Makefile
Run CMake to generate a Makefile for MinGW:

sh
Copy
Edit
cmake .. -G "MinGW Makefiles"
3️⃣ Build the Executable
Compile the source code using MinGW Make:

sh
Copy
Edit
mingw32-make
This will generate llvm_arthimatic.exe.

🚀 Running the Compiler
4️⃣ Generate LLVM IR
Run the executable and redirect its output to output.ll:

sh
Copy
Edit
./llvm_arthimatic.exe > output.ll
5️⃣ Execute the LLVM IR
Run the generated LLVM IR using lli:

sh
Copy
Edit
lli output.ll
## Conclusion
This program showcases how LLVM can be used to generate and manipulate LLVM IR dynamically, enabling custom compilation pipelines and optimization techniques.

