#pragma once

#include <vector>
#include <functional>
#include <mutex>

namespace event {

template<typename... Args>
class Event {
protected:
    using HandlerFn = std::function<void(Args...)>;

    std::mutex m_lock;
    std::vector<std::pair<HandlerFn, size_t>> m_handlers;
public:
    Event() = default;

    virtual ~Event() = default;

    void invoke(Args... args) {
        std::lock_guard<std::mutex> _g(m_lock);
        for (const auto &[handler, p]: m_handlers) handler(args...);
    }

    void operator()(Args... args) {
        invoke(args...);
    }

    Event &addHandler(const HandlerFn &handler) {
        std::lock_guard<std::mutex> _g(m_lock);
        auto ptr = reinterpret_cast<size_t>(handler.template target<void (*)(Args...)>());
        if (ptr) for (const auto &[h, p]: m_handlers) if (p == ptr) return *this;
        m_handlers.emplace_back(handler, ptr);
        return *this;
    }

    Event &removeHandler(const HandlerFn &handler) {
        std::lock_guard<std::mutex> _g(m_lock);
        auto p = reinterpret_cast<size_t>(handler.template target<void (*)(Args...)>());
        if (p) std::erase_if(m_handlers, [p](const auto &h) { return p == h.second; });
        return *this;
    }

    void clear() {
        std::lock_guard<std::mutex> _g(m_lock);
        m_handlers.clear();
    }

    [[nodiscard]] bool empty() const {
        return m_handlers.empty();
    }

    [[nodiscard]] size_t size() const {
        return m_handlers.size();
    }

    Event &operator+=(const HandlerFn &handler) {
        return addHandler(handler);
    }

    Event &operator-=(const HandlerFn &handler) {
        return removeHandler(handler);
    }
};

template<typename... Args>
class CancelableEvent : public Event<Args..., bool &> {
private:
    using Event<Args..., bool &>::invoke;
    using Event<Args..., bool &>::operator();

public:
    CancelableEvent() = default;

    virtual ~CancelableEvent() = default;

    void invoke(Args... args) {
        std::lock_guard<std::mutex> _g(this->m_lock);
        bool cancel = false;
        for (const auto &[handler, p]: this->m_handlers) {
            handler(args..., cancel);
            if (cancel) break;
        }
    }

    void operator()(Args... args) {
        invoke(args...);
    }
};

}
