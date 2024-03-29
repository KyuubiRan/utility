#include "../BlockingContainer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <deque>

int main() {
    container::Blocking<std::deque<int>> container1;
    container::Blocking<std::deque<int>> container2;

    auto t1 = std::thread([&] {
        int i = 10;
        while (--i) {
            int v = container1.pop_front();
            std::cout << "pop_front: " << v << std::endl;
        }
    });

    auto t2 = std::thread([&] {
        int i = 10;
        while (--i) {
            auto v = container2.try_pop_front();
            if (v)
                std::cout << "try_pop_front: " << *v << std::endl;
            else
                std::cout << "try_pop_front: " << "empty" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    auto t3 = std::thread([&] {
        for (int i = 1; i <= 10; ++i) {
            container1.push_back(i);
            container2.push_back(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });

    t1.join();
    t2.join();
    t3.join();
}
