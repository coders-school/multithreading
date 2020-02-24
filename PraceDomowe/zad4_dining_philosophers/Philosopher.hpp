#pragma once

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include "Fork.hpp"

class Philosopher{
private:
    std::string philosopherName;
    Fork& leftFork;
    Fork& rightFork;
    std::thread philosopherThread;

public:
    Philosopher(std::string name, Fork lFork, Fork rFork);
    ~Philosopher();

    void eat();
    void think();
    void dine();
};
