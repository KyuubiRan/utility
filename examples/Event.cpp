#include <iostream>
#include "../Event.hpp"

void test(int i1) {
    std::cout << "test: " << i1 << std::endl;
}

class Test {
public:
    void test(int i1) {
        std::cout << "member test: " << i1 << std::endl;
    }
};

int main() {
    event::Event<int> e;
    auto fn = [](int i) { std::cout << "i: " << i << std::endl; };
    int i2 = 123123;
    auto fn2 = [i2](int i) { std::cout << "i: " << i << " i2: " << i2 << std::endl; };
    e += fn;
    e += fn2;
    e += test;
    e += test;

    Test t = Test();
    e += [&t](int i) { t.test(i); };

    e(666);
    std::cout << "------------Remove Test-----------" << std::endl;
    e -= test;
    e(666);
}