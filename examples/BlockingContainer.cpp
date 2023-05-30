#include "../BlockingContainer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <deque>

int main() {
    container::Blocking<std::deque<int>> container;

    auto t1 = std::thread([&] {
        for (int i = 0; i < 5; ++i) {
            int v = container.pop_front();
            std::cout << "pop_front: " << v << std::endl;
        }
    });

    auto t2 = std::thread([&] {
        for (int i = 0; i < 5; ++i) {
            container.push_back(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });

    t1.join();
    t2.join();
}