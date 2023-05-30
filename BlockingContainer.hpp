#pragma once

#include <mutex>
#include <condition_variable>

namespace container {

template<typename Container>
class Blocking {
private:
    Container m_data;
    std::mutex m_mutex;
    std::condition_variable m_cv;

public:
    typename Container::value_type pop_front() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [&] { return !m_data.empty(); });
        typename Container::value_type value = m_data.front();
        m_data.pop_front();
        return value;
    }

    typename Container::value_type pop_back() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [&] { return !m_data.empty(); });
        typename Container::value_type value = m_data.back();
        m_data.pop_back();
        return value;
    }

    void push_back(Container::value_type value) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_data.push_back(value);
        m_cv.notify_one();
    }

    void push_front(Container::value_type value) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_data.push_front(value);
        m_cv.notify_one();
    }
};

}