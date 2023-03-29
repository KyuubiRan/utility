#pragma once

#include <random>
#include <concepts>

namespace rng {

template<typename T>
concept FloatingPointType = std::floating_point<T>;

template<typename T>
concept IntegerType = std::integral<T>;

class Random {
    std::mt19937_64 m_gen;

public:
    explicit Random(uint64_t seed) : m_gen(seed) {}

    Random() : m_gen((uint64_t) std::random_device()()) {}

    static Random &GetInstance() {
        static Random rand{};
        return rand;
    }

    template<typename T>
    requires IntegerType<T>
    T nextInteger(T from, T to) {
        if (from >= to) return to;
        std::uniform_int_distribution<T> d(from, to);
        return d(m_gen);
    }

    template<typename T>
    requires IntegerType<T>
    T nextInteger(T to) {
        return nextInteger((T) 0, to);
    }

    template<typename T>
    requires FloatingPointType<T>
    T nextFloating(T from, T to) {
        if (from >= to) return to;
        std::uniform_real_distribution<T> d(from, to);
        return d(m_gen);
    }

    template<typename T>
    requires FloatingPointType<T>
    T nextFloating(T to) {
        return (T) nextFloating((T) 0, to);
    }

    bool nextBool() {
        return nextInteger(1) == 1;
    }

    bool nextBool(const float ratio) {
        return nextFloating(1.f) < ratio;
    }
};

}
