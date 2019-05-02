#pragma once
#include <vector>
#include <mutex>

class Object
{
    std::mutex m_;
    std::vector<int> data_ = {1, 2, 4};

public:
    void push(const int value)
    {
        std::lock_guard<std::mutex> lg(m_);
        data_.emplace_back(value);
    }

    int pop()
    {
        std::lock_guard<std::mutex> lg(m_);
        auto value = data_.back();
        data_.pop_back();
        return value;
    }
};
