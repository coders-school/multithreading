#pragma once
#include "Object.hpp"
#include <memory>
#include <future>
#include <mutex>
#include <iostream>

class ActiveObject
{
    std::unique_ptr<Object> object_;

public:
    ActiveObject(std::unique_ptr<Object> obj)
        : object_(std::move(obj))
    {}

    std::future<void> push([[maybe_unused]] const int value)
    {
        // TODO: Run push() on object_ asynchronously, print this_thread::get_id(), return a future
        return {};
    }

    std::future<int> pop()
    {
        // TODO: Run pop() on object_ asynchronously, print this_thread::get_id(), return a future
        return {};
    }
};
