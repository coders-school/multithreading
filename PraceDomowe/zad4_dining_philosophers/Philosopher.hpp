#pragma once

#include <chrono>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include "Fork.hpp"

class Philosopher{
private:
    std::string philosopherName;
    Fork& leftFork;
    Fork& rightFork;
    bool haveForks;
    std::thread philosopherThread;
    static const int numberOfIteration = 20;

public:
    Philosopher(std::string name, Fork& lFork, Fork& rFork);
    Philosopher(Philosopher && other) = default;
    ~Philosopher();

    std::string getPhilosopherName() const;
    bool getHaveForks() const;

    void eat();
    void think();
    void dine();
};