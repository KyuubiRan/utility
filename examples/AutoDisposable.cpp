#include "../AutoDisposable.hpp"
#include <iostream>
#include <cstring>

class Test {
    int i;
    bool moved = false;

public:
    explicit Test(int i) : i(i) {
        std::cout << "Test " << i << " ctor" << std::endl;
    }

    Test(Test &&t) noexcept: i(t.i) {
        moved = true;
        std::cout << "Test " << i << " move ctor" << std::endl;
    }

    ~Test() {
        std::cout << "Test(moved: " << std::boolalpha << moved << ") " << i << " dtor" << std::endl;
    }
};

using namespace disposable;

template<>
struct IDisposable<Test *> {
    static void OnDispose(Test *test) {
        std::cout << "Auto IDisposable Test 1" << std::endl;
        delete test;
    }
};

template<>
struct IDisposable<Test> {
    static void OnDispose(Test &test) {
        std::cout << "Auto IDisposable Test 2" << std::endl;
    }
};

template<>
struct IDisposable<FILE *> {
    static void OnDispose(FILE *file) {
        std::cout << "Auto IDisposable FILE" << std::endl;
        fclose(file);
    }
};

int main() {
    {
        Test *t = new Test(1);
        auto test = AutoDisposable<Test *>(t);
    }

    {
        Test t = Test(2);
        auto test = AutoDisposable<Test>(std::move(t));
    }

    {
        FILE *file = fopen("test.txt", "w");
        auto test = AutoDisposable<FILE *>(file);
        char *buffer = "Hello, World!";
        fwrite(buffer, 1, strlen(buffer), file);
    }
}