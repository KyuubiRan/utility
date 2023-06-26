#pragma once

// Modified from: https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <condition_variable>
#include <future>

namespace thread {

using Task = std::function<void()>;

class ThreadPool {
    std::vector<std::thread> m_workers{};
    std::mutex m_lock{};
    std::queue<Task> m_tasks{};
    std::condition_variable m_condition{};
    bool m_stopped = false;

public:
    explicit ThreadPool(size_t size = 8) {
        for (size_t i = 0; i < size; ++i) {
            m_workers.emplace_back([this] {
                while (true) {
                    Task tsk;
                    {
                        std::unique_lock<std::mutex> lock(m_lock);
                        m_condition.wait(lock, [this] { return m_stopped || !m_tasks.empty(); });
                        if (m_stopped && m_tasks.empty())
                            return;
                        tsk = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    tsk();
                }
            });
        }
    }

    template<typename Func, typename ... Args>
    auto post(Func &&f, Args &&... args) -> std::future<std::invoke_result_t<Func, Args...>> {
        using Ret = std::invoke_result_t<Func, Args...>;

        auto tsk = std::make_shared<std::packaged_task<Ret()>>([f = std::forward<Func>(f), ...args = std::forward<Args>(args)]() mutable {
            return std::invoke(std::move(f), std::move(args)...);
        });

        std::future<Ret> result = tsk->get_future();
        {
            std::unique_lock<std::mutex> lock(m_lock);

            if (m_stopped)
                throw std::runtime_error("Post tsk on stopped ThreadPool!");

            m_tasks.emplace([tsk = std::move(tsk)]() { (*tsk)(); });
        }

        m_condition.notify_one();
        return result;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_lock);
            m_stopped = true;
        }
        m_condition.notify_all();
        for (auto &t : m_workers) {
            if (t.joinable())
                t.join();
        }
    }
};

}