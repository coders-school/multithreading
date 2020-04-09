#pragma once
#include <mutex>
#include "Fork.hpp"
#include <chrono>
#include <atomic>

class Philosopher
{
    public:
        const std::string name_;
        bool full_ {false};
        Fork &leftFork_;
        Fork &rightFork_;
        std::chrono::steady_clock::time_point lastMeal_;
        uint8_t haveToWaitBeforeNextMeal = 5;
        int time_ = 0;

        Philosopher(const std::string &name, Fork &lefFork, Fork &rightFork);
        Philosopher() = delete;

        void printEat();
        void eat();
        void think();
        void dine();
        void wait();
        void print(const std::string &message);
        void printThink();
        bool getHaveForks() const;
        std::string getName() const;
        uint64_t countTimeOfLastMeal();
};