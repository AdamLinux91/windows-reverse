cmake_minimum_required(VERSION 3.20)
project(NtDeoptLLVMPlugin LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_INSTALL_PREFIX}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_library(NtDeoptPass MODULE
    NtLoopRerollPass.cpp
)

set_target_properties(NtDeoptPass PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
    PREFIX "lib"
)
