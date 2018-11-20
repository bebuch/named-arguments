cmake_minimum_required(VERSION 3.12.4)

project(test)
find_package(Boost REQUIRED)

# Setup testing
enable_testing()

# TODO: Use C++20 for creating compile time string literals instead of GNU
#       extension
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS GNU)


# Add test cpp file
include_directories("../include")

add_executable(simple_wrap
    simple_wrap.cpp
)

# Link simple_wrap executable
target_link_libraries(simple_wrap)

add_test(
    NAME simple_wrap
    COMMAND simple_wrap
)
