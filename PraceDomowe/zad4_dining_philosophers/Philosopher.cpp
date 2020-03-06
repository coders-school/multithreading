#include "Philosopher.hpp"

Philosopher::Philosopher(std::string name, Fork& lFork, Fork& rFork):
    philosopherName(name), leftFork(lFork), rightFork(rFork), philosopherThread(&Philosopher::dine, this), haveForks(false) {}

Philosopher::~Philosopher(){
    philosopherThread.join();
}

std::string Philosopher::getPhilosopherName() const{
    return philosopherName;
}

bool Philosopher::getHaveForks() const{
    return haveForks;
}

void Philosopher::eat(){
    std::scoped_lock lockForks(leftFork.getMutex(), rightFork.getMutex());

    haveForks = true;
    std::stringstream eatStart;
    eatStart << this->philosopherName << "start eating." << std::endl;
    std::cout << eatStart.rdbuf();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::stringstream eatStop;
    eatStop << this->philosopherName << "stop eating." << std::endl;
    std::cout << eatStop.rdbuf();
    haveForks = false;
    }

void Philosopher::think(){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::stringstream trinkString;
        trinkString << this->philosopherName << "is thinking." << std::endl;
        std::cout << trinkString.rdbuf();

    }

void Philosopher::dine(){
    // dodac petle
        think();
        eat();
    }
