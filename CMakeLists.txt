cmake_minimum_required(VERSION 3.24)
project(utility)

set(CMAKE_CXX_STANDARD 20)

add_executable(example_random examples/Random.cpp)
add_executable(example_sequence examples/Sequence.cpp)
add_executable(example_event examples/Event.cpp)
add_executable(example_blocking_container examples/BlockingContainer.cpp)
add_executable(example_thread_pool examples/ThreadPool.cpp)
add_executable(example_auto_dispose examples/AutoDisposable.cpp)