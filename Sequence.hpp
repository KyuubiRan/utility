#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <optional>
#include <algorithm>

namespace container {

template<typename T>
class Sequence {
protected:
    std::vector<T> m_data;

    using FilterFn = std::function<bool(const T &)>;
    using LoopFn = std::function<void(const T &)>;
public:
    Sequence() : m_data() {}

    template<class Container>
    explicit Sequence(const Container &data) : m_data(data.begin(), data.end()) {}

    Sequence(const std::initializer_list<T> &list) : m_data(list) {}

    Sequence(const Sequence &other) {
        m_data += other.m_data;
    }

    virtual ~Sequence() = default;

    Sequence &operator=(const Sequence &other) {
        m_data = other.m_data;
        return *this;
    }

    Sequence &operator+=(const Sequence &other) {
        m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
        return *this;
    }

    template<template<typename> class Container>
    Sequence &operator+=(const Container<T> &other) {
        m_data.insert(m_data.end(), other.begin(), other.end());
        return *this;
    }

    Sequence &operator+=(const T &other) {
        m_data.push_back(other);
        return *this;
    }

    Sequence &operator-=(const Sequence &other) {
        for (const auto &item: other.m_data)
            m_data.erase(std::remove(m_data.begin(), m_data.end(), item), m_data.end());
        return *this;
    }

    Sequence &operator-=(const std::vector<T> &other) {
        for (const auto &item: other)
            m_data.erase(std::remove(m_data.begin(), m_data.end(), item), m_data.end());
        return *this;
    }

    Sequence &operator-=(const T &other) {
        m_data.erase(std::remove(m_data.begin(), m_data.end(), other), m_data.end());
        return *this;
    }

    Sequence &contact(const Sequence &other) {
        m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
        return *this;
    }

    template<template<typename> class Container>
    Sequence &contact(const Container<T> &other) {
        m_data.insert(m_data.end(), other.begin(), other.end());
        return *this;
    }

    Sequence &contact(const T &other) {
        m_data.push_back(other);
        return *this;
    }

    Sequence &filter(const FilterFn &func) {
        std::erase_if(m_data, [&func](const auto &o) { return !func(o); });
        return *this;
    }

    std::optional<T> first(const FilterFn &func) {
        if (m_data.empty()) return std::nullopt;
        for (const auto &item: m_data)
            if (func(item)) return item;
        return std::nullopt;
    }

    std::optional<T> last(const FilterFn &func) {
        if (m_data.empty()) return std::nullopt;
        for (auto it = m_data.rbegin(); it != m_data.rend(); ++it)
            if (func(*it)) return *it;
        return std::nullopt;
    }

    std::optional<T> first() {
        if (m_data.empty()) return std::nullopt;
        return m_data.front();
    }

    std::optional<T> last() {
        if (m_data.empty()) return std::nullopt;
        return m_data.back();
    }

    std::optional<T> at(size_t index) {
        if (index >= m_data.size()) return std::nullopt;
        return m_data[index];
    }

    std::optional<T> operator[](size_t index) {
        return at(index);
    }

    template<typename Container, typename Func, typename Elem = std::decay_t<std::invoke_result_t<Func, const T &>>>
    Container mapTo(Func &&func) {
        std::vector<Elem> vec{};
        vec.reserve(m_data.size());
        std::transform(m_data.begin(), m_data.end(), std::back_inserter(vec), std::forward<Func>(func));
        Container result(vec.begin(), vec.end());
        return result;
    }

    template<class Elem>
    Sequence<Elem> map(const std::function<Elem(const T &)> &func) {
        Sequence<Elem> result;
        result.m_data.reserve(m_data.size());
        std::transform(m_data.begin(), m_data.end(), std::back_inserter(result.m_data), func);
        return result;
    }

    void forEach(const LoopFn &func) {
        for (const auto &item: m_data) func(item);
    }

    Sequence &onEach(const LoopFn &func) {
        for (auto &item: m_data) func(item);
        return *this;
    }

    [[nodiscard]] size_t size() const {
        return m_data.size();
    }

    [[nodiscard]] bool empty() const {
        return m_data.empty();
    }

    [[nodiscard]] bool notEmpty() const {
        return !m_data.empty();
    }

    [[nodiscard]] bool any(const FilterFn &func) const {
        return std::ranges::any_of(m_data, func);
    }

    [[nodiscard]] bool none(const FilterFn &func) const {
        return std::ranges::none_of(m_data, func);
    }

    [[nodiscard]] bool all(const FilterFn &func) const {
        return std::ranges::all_of(m_data, func);
    }

    template<class Container>
    static Sequence Of(const Container &data) {
        return Sequence(data);
    }

    static Sequence Of(const std::initializer_list<T> &list) {
        return Sequence(list);
    }

    std::vector<T> &&toVector() {
        return std::move(m_data);
    }

    auto begin() {
        return m_data.begin();
    }

    auto end() {
        return m_data.end();
    }

    auto cbegin() {
        return m_data.cbegin();
    }

    auto cend() {
        return m_data.cend();
    }

    auto rbegin() {
        return m_data.rbegin();
    }

    auto rend() {
        return m_data.rend();
    }

    auto crbegin() {
        return m_data.crbegin();
    }

    auto crend() {
        return m_data.crend();
    }
};

}
