#include "Philosopher.hpp"
#include <cstdlib>
#include <sstream>
#include <thread>
#include <iostream>

Philosopher::Philosopher(const std::string &name, Fork &leftFork, Fork &rightFork) 
: leftFork_(leftFork), 
  rightFork_(rightFork),
  name_(name) {}

void Philosopher::eat()
{
    if(countTimeOfLastMeal() >= haveToWaitBeforeNextMeal and !full_)
    {
        std::scoped_lock lock(leftFork_.mutex, rightFork_.mutex);
        printEat();
        wait();
        print("finished eating");
        full_ = true;
        lastMeal_ = std::chrono::steady_clock::now();
        ++time_;
    }
}

void Philosopher::think()
{
    printThink();
    wait();
    print("finished eating");
    full_ = false;
    ++time_;
}

void Philosopher::dine()
{
    while(time_ < 10)
    {
        if(!full_)
            eat();
        else
            think();
    }
}

void Philosopher::print(const std::string &message)
{
    std::stringstream ss;
    ss << "!" << name_ << "!   " << message << std::endl;
    std::cout << ss.rdbuf();
}

void Philosopher::wait()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1500));
}

void Philosopher::printEat()
{
    std::stringstream ss;
    ss << "-" << name_ << "-   is eating\n";
    std::cout << ss.rdbuf();
}

void Philosopher::printThink()
{
    std::stringstream ss;
    ss << " " << name_ << "    is thinking\n";
    std::cout << ss.rdbuf();
}

uint64_t Philosopher::countTimeOfLastMeal()
{
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - lastMeal_).count();
}
