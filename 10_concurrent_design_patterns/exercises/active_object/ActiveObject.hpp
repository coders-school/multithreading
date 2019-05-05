#pragma once
#include "Object.hpp"
#include <memory>
#include <future>
#include <mutex>
#include <iostream>

class ActiveObject
{
    std::unique_ptr<Object> object_;
    std::mutex coutMtx;
public:
    ActiveObject(std::unique_ptr<Object> obj)
        : object_(std::move(obj))
    {}

    std::future<void> push(const int value)
    {
        return std::async(std::launch::async, [&, value] ()
        {
            std::lock_guard<std::mutex> locker ( coutMtx );
            std::cout<<std::this_thread::get_id() << std::endl;
            return object_->push(value);
        });
    }

    std::future<int> pop()
    {
        // TODO: Run pop() on object_ asynchronously, print this_thread::get_id(), return a future
        return std::async(std::launch::async, [&] ()
        {
            std::lock_guard<std::mutex> locker ( coutMtx );
            std::cout<<std::this_thread::get_id() << std::endl;
            return object_->pop();
        });
    }
};
