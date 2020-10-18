#pragma once
#include <condition_variable>
#include <mutex>
#include <deque>

template <typename T>
class ThreadSafeQueue
{
    std::deque<T> queue_;
    mutable std::mutex m_;
    std::condition_variable nonEmpty_;
    using Lock = std::unique_lock<std::mutex>;

public:
    auto begin()
    {
        return queue_.begin();
    }

    auto end()
    {
        return queue_.end();
    }

    void push(T && element)
    {
        Lock l(m_);
        queue_.emplace_front(std::move(element));
        nonEmpty_.notify_all();
    }

    T pop()
    {
        Lock l(m_);
        auto hasData = [&] { return not queue_.empty(); };
        nonEmpty_.wait(l, hasData);
        auto top = std::move(queue_.back());
        queue_.pop_back();
        return top;
    }
};
