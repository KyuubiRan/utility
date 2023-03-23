cmake_minimum_required(VERSION 3.24)
project(utility)

set(CMAKE_CXX_STANDARD 20)

link_libraries(-static-libgcc -static-libstdc++)

add_executable(example_random examples/Random.cpp)
add_executable(example_sequence examples/Sequence.cpp)