//
// Created by kitsune on 2025/12/8.
//

#include "../Lazy.hpp"

#include <iostream>

int main() {
    lazy::Lazy value([] {
        std::cout << "Initializing value..." << std::endl;
        return 42;
    });

    std::cout << value.get() << std::endl;
    std::cout << value.ref() << std::endl;
    std::cout << value.ptr() << std::endl;
}
