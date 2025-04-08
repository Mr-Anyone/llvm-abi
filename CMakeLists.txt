cmake_minimum_required(VERSION 3.2)
project(llvm-abi)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED Config)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
message(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})



add_subdirectory(src)
add_subdirectory(test)
