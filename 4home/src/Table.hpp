#pragma once
#include <vector>
#include <thread>
#include "Philosopher.hpp"
#include <memory>

class Table
{
    private:
        const int &numberOfPhilosophers_;
        std::vector<Fork> forks;
        std::vector<std::thread> threads;
        std::vector<Philosopher> philosophersPTR;

    public:
        Table(const int &numberOfPhilosophers);
        ~Table();
        void StartDinner();
};
