#pragma once
#include "ThreadSafeQueue.hpp"
#include "Task.hpp"


class ThreadPool
{
public:
    ThreadPool(const unsigned size)
    {
        // TODO: Implement me :)
    }

    ~ThreadPool()
    {
        // TODO: Implement me :)
    }

    auto enqueue([[maybe_unused]] Task && task)
    {
       // TODO: Implement me :)
       return std::future<Task::PromiseType>{};
    }
};
