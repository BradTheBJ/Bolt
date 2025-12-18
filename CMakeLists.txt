cmake_minimum_required(VERSION 3.15)
project(Bolt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set LLVM-specific variables
llvm_map_components_to_libnames(llvm_libs
    core
    irreader
    support
    native
)

# Include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_definitions(${LLVM_DEFINITIONS})

# Source files
set(BOLT_SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
)

# Create executable
add_executable(bolt ${BOLT_SOURCES})

# Link libraries
target_link_libraries(bolt ${llvm_libs})

# Set output directory
set_target_properties(bolt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

