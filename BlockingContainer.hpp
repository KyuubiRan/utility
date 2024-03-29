#pragma once

#include <mutex>
#include <optional>
#include <condition_variable>

namespace container {
    template<typename T>
    concept HasPopFront = requires(T a)
    {
        { a.pop_front() } -> std::same_as<void>;
    };

    template<typename T>
    concept HasPopBack = requires(T a)
    {
        { a.pop_back() } -> std::same_as<void>;
    };

    template<typename T>
    concept HasPushFront = requires(T a, typename T::value_type v)
    {
        { a.push_front(v) } -> std::same_as<void>;
    };

    template<typename T>
    concept HasPushBack = requires(T a, typename T::value_type v)
    {
        { a.push_back(v) } -> std::same_as<void>;
    };

    template<typename Container>
    class Blocking {
    private:
        Container m_data;
        std::mutex m_mutex;
        std::condition_variable m_cv;

    public:
        typename Container::value_type pop_front() requires HasPopFront<Container> {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [&] { return !m_data.empty(); });
            typename Container::value_type value = m_data.front();
            m_data.pop_front();
            return value;
        }

        typename Container::value_type pop_back() requires HasPopBack<Container> {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [&] { return !m_data.empty(); });
            typename Container::value_type value = m_data.back();
            m_data.pop_back();
            return value;
        }

        std::optional<typename Container::value_type> try_pop_front() requires HasPopFront<Container> {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_data.empty()) {
                return std::nullopt;
            }
            typename Container::value_type value = m_data.front();
            m_data.pop_front();
            return value;
        }

        std::optional<typename Container::value_type> try_pop_back() requires HasPopBack<Container> {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (m_data.empty()) {
                return std::nullopt;
            }
            typename Container::value_type value = m_data.front();
            m_data.pop_back();
            return value;
        }


        void push_back(typename Container::value_type value) requires HasPushBack<Container> {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_data.push_back(value);
            m_cv.notify_one();
        }

        void push_front(typename Container::value_type value) requires HasPushFront<Container> {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_data.push_front(value);
            m_cv.notify_one();
        }
    };
}
