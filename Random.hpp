#pragma once

#include <random>
#include <concepts>

namespace rng {
template<typename T>
concept FloatingPointType = std::floating_point<T>;

template<typename T>
concept IntegerType = std::integral<T>;

template<typename T>
concept CharType = std::same_as<T, char> || std::same_as<T, wchar_t> || std::same_as<T, char8_t> || std::same_as<T,
                       char16_t> || std::same_as<T, char32_t>;

class Random {
    std::mt19937_64 m_gen;

public:
    explicit Random(const uint64_t seed) : m_gen(seed) {
    }

    Random() : m_gen(std::random_device()()) {
    }

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
        return nextInteger(static_cast<T>(0), to);
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
        return static_cast<T>(nextFloating(static_cast<T>(0), to));
    }

    template<typename T>
        requires CharType<T>
    T nextChar(T from, T to) {
        if (from >= to)
            return to;
        std::uniform_int_distribution d(static_cast<int32_t>(from), static_cast<int32_t>(to));
        return static_cast<T>(d(m_gen));
    }

    template<typename T>
        requires CharType<T>
    T nextChar(T to) {
        return nextChar((T) 0, to);
    }

    bool nextBool() {
        return nextInteger(1) == 1;
    }

    bool nextBool(const float ratio) {
        return nextFloating(1.f) < ratio;
    }
};
}
