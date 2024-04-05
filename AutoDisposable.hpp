#pragma once

#include <mutex>

namespace disposable {

template<typename T>
struct IDisposable {
    static void OnDispose(T &data) {};
};

template<typename T>
class AutoDisposable {
    bool m_disposed = false;
    std::mutex m_lock;
    T m_data;

public:
    explicit AutoDisposable(T &&data) : m_data(std::move(data)) {}

    explicit AutoDisposable(const T &data) : m_data(data) {}

    void dispose() {
        std::lock_guard<std::mutex> _g(m_lock);

        if (m_disposed)
            return;

        m_disposed = true;
        IDisposable<T>::OnDispose(m_data);
    }

    ~AutoDisposable() {
        dispose();
    }

    bool isDisposed() {
        return m_disposed;
    }

    T &ref() {
        return m_data;
    }

    T *ptr() {
        return &m_data;
    }

    operator bool() { // NOLINT(*-explicit-constructor)
        return !m_disposed;
    }

    operator T &() { // NOLINT(*-explicit-constructor)
        return m_data;
    }

    operator T *() { // NOLINT(*-explicit-constructor)
        return &m_data;
    }
};

}
