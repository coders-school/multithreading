#pragma once
#include "ThreadSafeQueue.hpp"
#include "Task.hpp"

#include <tuple>



class ThreadPool
{
    std::vector<std::thread> threads;
    ThreadSafeQueue<Task> tasks;
    //ThreadSafeQueue<Task> completedTasks;
public:
    ThreadPool(const unsigned size)
    {
        for(auto i = 0u; i < size; i++){
            threads.emplace_back([&]{ 
                while (true) {
                    auto task = getTask();
                    if (task.id != "end") {
                        task.algo( task.input, task.output );
                        task.promise.set_value(std::make_pair(task.id, task.output));
                    }
                    else{
                        return;
                    }
                }
             });
        }
    }

    ~ThreadPool()
    {
        for([[maybe_unused]] auto && t: threads)
        {
        tasks.push(Task{"end",{}});
        }
        for(auto && t : threads)
        {
            t.join();
        }
    }

    auto enqueue(Task && task)
    {
       auto f = task.promise.get_future();
       tasks.push(std::move(task));
       return f;
    }

    Task getTask()
    {
        
        return tasks.pop();
        
    }
};