#include "Philosopher.hpp"

Philosopher::Philosopher(std::string name, Fork& lFork, Fork& rFork):
    philosopherName(name), leftFork(lFork), rightFork(rFork), philosopherThread(&Philosopher::dine, this) {}

Philosopher::Philosopher(Philosopher && other):
    philosopherName(other.philosopherName), leftFork(other.leftFork), rightFork(other.rightFork), philosopherThread(&Philosopher::dine, this) {}

Philosopher::~Philosopher(){
    philosopherThread.join();
}

void Philosopher::eat(){
    std::scoped_lock lockForks(leftFork.getMutex(), rightFork.getMutex());

    std::cout << this->philosopherName << "start eating." << std::endl;
    std::cout << this->philosopherName << "stop eating." << std::endl;
    }

void Philosopher::think(){
        std::cout << this->philosopherName << "is thinking" << std::endl;
    }

void Philosopher::dine(){
        think();
        eat();
    }
