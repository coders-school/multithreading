#pragma once
#include "ThreadSafeQueue.hpp"
#include "Task.hpp"
#include <vector>

class ThreadPool
{
std::vector<std::thread> threads;
ThreadSafeQueue<Task> queue_;

public:
    ThreadPool(const unsigned size)
    {
        threads.reserve(size);

        auto lambda = [&](){
            while(true)
            {
                auto task = queue_.pop();
                if (task.id == "kill" ){
                    break;
                }
                task.algo(task.input, task.output);
                auto result = std::make_pair(task.id, task.output);
                task.promise.set_value(result);
            }
        };
        for(unsigned int i = 0u; i < size; ++i){
            threads.emplace_back(lambda);
        }
    }

    ~ThreadPool()
    {
        for ( size_t i =0; i < threads.size(); ++i) {
            Task killTask{"kill", []([[maybe_unused]] const auto& a,[[maybe_unused]] auto& b){}};
            queue_.push(std::move(killTask));
        }
        for (auto &&i : threads) {
            i.join();
        }
        
    }

    auto enqueue([[maybe_unused]] Task && task)
    {
       auto t = task.promise.get_future();
       queue_.push(std::move(task));
       return t; // std::future<Task::PromiseType>{task.promise.get_future()};
    }
};
