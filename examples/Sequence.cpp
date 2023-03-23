//
// Created by 62305 on 2023/3/23.
//

#include <iostream>
#include <set>
#include <vector>
#include "../Sequence.hpp"

using namespace container;

int main() {
    Sequence<int> seq1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    seq1.filter([](int i) { return i % 2 == 0; }).forEach([](int i) { std::cout << i << std::endl; });

    std::cout << "---------Filter End---------" << std::endl;

    auto vec = seq1.mapTo<std::vector<float>>([](int item) { return item * 2.23f; });
    auto seq2 = Sequence<float>(vec);
    seq2.forEach([](float i) { std::cout << i << std::endl; });

    std::cout << "---------Map End 1---------" << std::endl;

    auto set = seq1.mapTo<std::set<double>>([](int item) { return item * 2.23; });
    auto seq3 = Sequence<double>(set);
    seq3.forEach([](double i) { std::cout << i << std::endl; });

    std::cout << "---------Map End 2---------" << std::endl;

    auto vec2 = Sequence<int>::of({1, 2, 3}).onEach([](int i) { std::cout << i << std::endl; }).toVector();

    std::cout << "---------OnEach End---------" << std::endl;

    auto seq5 = Sequence<int>::of(vec2);
    auto elem1 = seq5.at(0);
    if (elem1) {
        std::cout << "Has elem1: " << *elem1 << std::endl;
    } else {
        std::cout << "No elem1" << std::endl;
    }
    auto elem2 = seq5[99];
    if (elem2) {
        std::cout << "Has elem2: " << *elem1 << std::endl;
    } else {
        std::cout << "No elem2" << std::endl;
    }

    std::cout << "---------At End---------" << std::endl;
}