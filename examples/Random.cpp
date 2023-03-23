//
// Created by 62305 on 2023/3/23.
//

#include <iostream>
#include "../Random.hpp"

using namespace rng;

int main() {
    auto &rand = Random::GetInstance();
    for (int i = 0; i < 10; i++) {
        std::cout << rand.nextInteger(1, 10) << std::endl;
    }
}
