#pragma once
#include "Reflection.hpp"
#include <shared_mutex>
#include <vector>

struct Book
{
    std::shared_mutex mutexBook_;
    std::vector<Reflection> reflections_;
};