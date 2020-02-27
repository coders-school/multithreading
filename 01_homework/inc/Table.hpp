#pragma once
#include <mutex>
#include <array>

struct Fork
{
   std::mutex mutex;
};

//template<std::size_t S>
class Table
{
    //const std::size_t population = 5;
public:
    bool food = true;
    std::array<Fork, 5> forks;
    // array for starving values

    void print();
};
