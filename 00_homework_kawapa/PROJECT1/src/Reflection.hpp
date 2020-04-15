#pragma once
#include <chrono>
#include <string>

using namespace std::chrono_literals;

struct Reflection
{
    std::string philosopher_;
    std::string answer_;
    int result_;
    int64_t period_;
    bool chosen_ {false};

    Reflection(std::string , std::string, int, int64_t);
};