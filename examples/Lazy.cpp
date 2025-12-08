//
// Created by kitsune on 2025/12/8.
//

#include "../Lazy.hpp"

#include <iostream>
#include <thread>

int main() {
    lazy::Lazy value([] {
        std::cout << "Initializing value on thread id: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return 42;
    });

    std::thread t1([&value] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << std::format("Thread 1: {}", value.get()) << std::endl;
    });

    std::thread t2([&value] {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << std::format("Thread 2: {}", value.get()) << std::endl;
    });

    t1.detach();
    t2.join();

    std::cout << "Main get: " << value.get() << std::endl;
    std::cout << "Main ref: " << value.ref() << std::endl;
    std::cout << "Main ptr: " << value.ptr() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
