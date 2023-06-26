#include "../ThreadPool.hpp"
#include <iostream>

int main() {
    thread::ThreadPool pool(4);
    auto tsk1 = pool.post([] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Task 1: Sleep 3s" << std::endl;
    });
    auto tsk2 = pool.post([] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task 2: Sleep 2s" << std::endl;
    });
    auto tsk3 = pool.post([] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Task 3: Sleep 1s" << std::endl;
    });

    tsk1.wait();
    return 0;
}