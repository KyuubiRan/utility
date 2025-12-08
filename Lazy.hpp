#pragma once

#include <optional>
#include <functional>

namespace lazy {
template<typename T, typename Ctor>
class Lazy final {
    std::optional<T> m_value;
    std::optional<Ctor> m_initializer;
    mutable std::once_flag m_flag;

    void initialize() {
        std::call_once(m_flag, [this] {
            m_value = (*m_initializer)();
            m_initializer = std::nullopt;
        });
    }

public:
    Lazy(Ctor initializer) : m_value(std::nullopt), m_initializer(std::move(initializer)) {
    }

    T &ref() {
        initialize();
        return *m_value;
    }

    T *ptr() {
        initialize();
        return &*m_value;
    }

    T get() {
        initialize();
        return *m_value;
    }
};

template<typename Initializer>
Lazy(Initializer) -> Lazy<std::invoke_result_t<Initializer>, Initializer>;
}
