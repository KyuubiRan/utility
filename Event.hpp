#pragma once

#include <vector>
#include <functional>

#ifdef EVENT_NO_LOCK
#define EVENT_LOCK_T std::monostate
#define EVENT_LOCK_G
#else
#include <mutex>
#define EVENT_LOCK_T std::mutex
#endif // EVENT_NO_LOCK

#ifndef EVENT_NO_LOCK
#ifdef EVENT_USE_LOCK_GUARD
#define EVENT_LOCK_G(lock) std::lock_guard<std::mutex> _g(lock)
#else
#define EVENT_LOCK_G(lock) std::scoped_lock _g(lock)
#endif // EVENT_USE_LOCK_GUARD
#endif // EVENT_NO_LOCK

namespace event {
template<typename... Args>
class Event {
protected:
    using HandlerFn = std::function<void(Args...)>;

    EVENT_LOCK_T m_lock;
    std::vector<std::pair<HandlerFn, size_t> > m_handlers;

public:
    Event() = default;

    virtual ~Event() = default;

    void invoke(Args... args) {
        EVENT_LOCK_G(m_lock);
        for (const auto &[handler, p]: m_handlers) handler(args...);
    }

    void operator()(Args... args) {
        invoke(args...);
    }

    Event &addHandler(const HandlerFn &handler) {
        EVENT_LOCK_G(m_lock);
        auto ptr = reinterpret_cast<size_t>(handler.template target<void (*)(Args...)>());
        if (ptr) for (const auto &[h, p]: m_handlers) if (p == ptr) return *this;
        m_handlers.emplace_back(handler, ptr);
        return *this;
    }

    Event &removeHandler(const HandlerFn &handler) {
        EVENT_LOCK_G(m_lock);
        auto p = reinterpret_cast<size_t>(handler.template target<void (*)(Args...)>());
        if (p) std::erase_if(m_handlers, [p](const auto &h) { return p == h.second; });
        return *this;
    }

    void clear() {
        EVENT_LOCK_G(m_lock);
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
    using Event<Args..., bool &>::invoke;
    using Event<Args..., bool &>::operator();

public:
    CancelableEvent() = default;

    ~CancelableEvent() override = default;

    void invoke(Args... args) {
        EVENT_LOCK_G(this->m_lock);
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

#ifdef EVENT_LOCK_T
#undef EVENT_LOCK_T
#endif
#ifdef EVENT_LOCK_G
#undef EVENT_LOCK_G
#endif
