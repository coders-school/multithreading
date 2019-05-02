#pragma once
#include "ThreadSafeQueue.hpp"
#include "Task.hpp"
#include <vector>
#include <thread>
#include <future>
#include <iostream>


class ThreadPool
{
    ThreadSafeQueue<Task> taskQueue_{};
    // ThreadSafeQueue<Task> completedTasks_{};
    std::vector<std::thread> pool_;

public:
    ThreadPool(const unsigned size)
    {
        auto job = [&]()
        {
            while(true)
            {
                auto&& task = taskQueue_.pop();
                if (not task.algo)
                    return;
                // std::cout << task.id << " started processing\n";
                task.algo(task.input, task.output);
                task.promise.set_value(std::make_pair(task.id, task.output));
                // std::cout << task.id << " finished processing\n";
                // completedTasks_.push(std::move(task));
            }
        };

        pool_.reserve(size);
        for (auto i = 0u; i < size; i++)
            pool_.emplace_back(job);
    }

    ~ThreadPool()
    {
        for (auto i = 0u; i < pool_.size(); i++)
            taskQueue_.push(Task{});

        for (auto && t : pool_)
            t.join();
    }

    auto enqueue(Task && task)
    {
        if (task.algo)
        {
            auto fut = task.promise.get_future(); 
            taskQueue_.push(std::move(task));
            return fut;
        }
        std::promise<Task::PromiseType> p;
        p.set_exception(std::make_exception_ptr(std::logic_error{"Quit task"}));
        return p.get_future();
    }

    // void showCompleted()
    // {
    //     for (const auto & task : completedTasks_)
    //     {
    //         std::cout << task.id << ", output: { ";
    //         for (const auto & item : task.output)
    //             std::cout << item << " ";
    //         std::cout << "}\n";
    //     }
    // }
};