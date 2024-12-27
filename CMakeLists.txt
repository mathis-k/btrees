# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(btrees)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add compiler warnings conditionally based on the compiler
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address -g")
endif ()

# Define directories
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

# Create output directories
file(MAKE_DIRECTORY ${BIN_DIR})

# Find all source files in src and src/cli
file(GLOB_RECURSE SRCS "*.cpp")


# Add the executable for the main program
add_executable(btrees
        btree.cpp
        exceptions.cpp
)

# Set the output directory for the executable
set_target_properties(btrees PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/Debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/Release
)

# Add a custom target to run the application
add_custom_target(run
        COMMAND ${BIN_DIR}/$<CONFIG>/btrees
        DEPENDS btrees
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Add a custom target to clean build artifacts
add_custom_target(clean_build
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${BIN_DIR}/*
)
