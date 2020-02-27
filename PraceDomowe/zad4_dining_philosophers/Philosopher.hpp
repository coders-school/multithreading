#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include "Fork.hpp"

class Philosopher{
private:
    std::string philosopherName;
    Fork& leftFork;
    Fork& rightFork;
    std::thread philosopherThread;

public:
    Philosopher(std::string name, Fork& lFork, Fork& rFork);
    Philosopher(Philosopher && other) = default;
    ~Philosopher();

    void eat();
    void think();
    void dine();
};
