#pragma once

#include <optional>
#include <functional>
#include <mutex>
#include <type_traits>

namespace lazy {
template<typename T, typename Ctor>
class Lazy final {
    mutable std::optional<T> m_value;
    mutable std::optional<Ctor> m_initializer;
    mutable std::once_flag m_flag;

    void initialize() const {
        std::call_once(m_flag, [this] {
            m_value = (*m_initializer)();
            m_initializer = std::nullopt;
        });
    }

public:
    Lazy(const Lazy &) = delete;

    Lazy &operator=(const Lazy &) = delete;

    Lazy(Lazy &&) = delete;

    Lazy &operator=(Lazy &&) = delete;

    Lazy(Ctor initializer) : m_value(std::nullopt), m_initializer(std::move(initializer)) { // NOLINT(*-explicit-constructor)
    }

    bool initialized() const {
        return m_value.has_value();
    }

    T &ref() const {
        initialize();
        return *m_value;
    }

    T *ptr() const {
        initialize();
        return &*m_value;
    }

    T get() const requires std::copy_constructible<T> {
        initialize();
        return *m_value;
    }
};

template<typename Initializer>
Lazy(Initializer) -> Lazy<std::invoke_result_t<Initializer>, Initializer>;
}
