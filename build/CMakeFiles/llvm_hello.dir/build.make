# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\msys64\mingw64\bin\cmake.exe

# The command to remove a file.
RM = C:\msys64\mingw64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Semester6\CC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Semester6\CC\build

# Include any dependencies generated for this target.
include CMakeFiles/llvm_hello.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/llvm_hello.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/llvm_hello.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/llvm_hello.dir/flags.make

CMakeFiles/llvm_hello.dir/codegen:
.PHONY : CMakeFiles/llvm_hello.dir/codegen

CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj: CMakeFiles/llvm_hello.dir/flags.make
CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj: D:/Semester6/CC/llvm_hello.cpp
CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj: CMakeFiles/llvm_hello.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Semester6\CC\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj"
	C:\msys64\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj -MF CMakeFiles\llvm_hello.dir\llvm_hello.cpp.obj.d -o CMakeFiles\llvm_hello.dir\llvm_hello.cpp.obj -c D:\Semester6\CC\llvm_hello.cpp

CMakeFiles/llvm_hello.dir/llvm_hello.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/llvm_hello.dir/llvm_hello.cpp.i"
	C:\msys64\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Semester6\CC\llvm_hello.cpp > CMakeFiles\llvm_hello.dir\llvm_hello.cpp.i

CMakeFiles/llvm_hello.dir/llvm_hello.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/llvm_hello.dir/llvm_hello.cpp.s"
	C:\msys64\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Semester6\CC\llvm_hello.cpp -o CMakeFiles\llvm_hello.dir\llvm_hello.cpp.s

# Object files for target llvm_hello
llvm_hello_OBJECTS = \
"CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj"

# External object files for target llvm_hello
llvm_hello_EXTERNAL_OBJECTS =

llvm_hello.exe: CMakeFiles/llvm_hello.dir/llvm_hello.cpp.obj
llvm_hello.exe: CMakeFiles/llvm_hello.dir/build.make
llvm_hello.exe: C:/msys64/mingw64/lib/libLLVM-19.dll.a
llvm_hello.exe: CMakeFiles/llvm_hello.dir/linkLibs.rsp
llvm_hello.exe: CMakeFiles/llvm_hello.dir/objects1.rsp
llvm_hello.exe: CMakeFiles/llvm_hello.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Semester6\CC\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable llvm_hello.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\llvm_hello.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/llvm_hello.dir/build: llvm_hello.exe
.PHONY : CMakeFiles/llvm_hello.dir/build

CMakeFiles/llvm_hello.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\llvm_hello.dir\cmake_clean.cmake
.PHONY : CMakeFiles/llvm_hello.dir/clean

CMakeFiles/llvm_hello.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Semester6\CC D:\Semester6\CC D:\Semester6\CC\build D:\Semester6\CC\build D:\Semester6\CC\build\CMakeFiles\llvm_hello.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/llvm_hello.dir/depend

