#pragma once
#include "Object.hpp"
#include <memory>
#include <future>
#include <mutex>
#include <iostream>

class ActiveObject
{
    std::unique_ptr<Object> object_;
    std::mutex m_;

public:
    ActiveObject(std::unique_ptr<Object> obj)
        : object_(std::move(obj))
    {}

    std::future<void> push(const int value)
    {
        return std::async(std::launch::async, [&, value]()
        {
            std::lock_guard<std::mutex> lg(m_);
            std::cout << std::this_thread::get_id() << '\n';
            return object_->push(value);
        });
    }

    std::future<int> pop()
    {
        return std::async(std::launch::async, [&]()
        { 
            std::lock_guard<std::mutex> lg(m_);
            std::cout << std::this_thread::get_id() << '\n';
            return object_->pop();
        });
    }
};
