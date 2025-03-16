cmake_minimum_required(VERSION 3.10)
project(LLVMExample)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Define the executable
add_executable(llvm_arthimatic llvm_arthimatic.cpp)
add_executable(llvm_hello llvm_hello.cpp)
# Link with LLVM libraries
target_link_libraries(llvm_arthimatic LLVM)
target_link_libraries(llvm_hello LLVM)
