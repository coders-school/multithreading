#pragma once
#include "Object.hpp"
#include <memory>
#include <future>
#include <mutex>
#include <iostream>
#include <vector>
//template <typename T>
class ActiveObject
{
    std::unique_ptr<Object> object_;
    // std::vector<std::future<T>> futures;
public:
    ActiveObject(std::unique_ptr<Object> obj)
        : object_(std::move(obj))
    {}

    std::future<void> push([[maybe_unused]] const int value)
    {
        auto lambda = [&, value](){return object_->push(value);};
        // TODO: Run push() on object_ asynchronously, print this_thread::get_id(), return a future
        return std::async(std::launch::async , lambda);
    }

    std::future<int> pop()
    {
        auto lambda = [&](){ return object_->pop();};

        // TODO: Run pop() on object_ asynchronously, print this_thread::get_id(), return a future
        return std::async(std::launch::async, lambda);
    }
};
