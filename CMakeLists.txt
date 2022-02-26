cmake_minimum_required(VERSION 3.21)
project(llvm_checkedc)

set(CMAKE_CXX_STANDARD 23)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# LLVM configuration
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(chclang main.cpp)

target_include_directories(chclang
        PRIVATE include)

# Use libc++ instead of libstdc++
target_compile_options(chclang
        PRIVATE -stdlib=libc++)

target_link_options(chclang
        PRIVATE -stdlib=libc++
        PRIVATE -lc++abi)

add_subdirectory(libs)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against libraries
target_link_libraries(chclang
        ${llvm_libs}
        GSL
        argparse
        magic_enum
        fmt::fmt-header-only)

add_subdirectory(base)
add_subdirectory(scanner)