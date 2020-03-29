#include "Philosopher.hpp"

Philosopher::Philosopher(std::string name, Fork& lFork, Fork& rFork):
    philosopherName(name), leftFork(lFork), rightFork(rFork), haveForks(false) {}

Philosopher::Philosopher(Philosopher && other):
    philosopherName(other.philosopherName), leftFork(other.leftFork), rightFork(other.rightFork), haveForks((other.haveForks).load()) {}

Philosopher::~Philosopher(){
    if(philosopherThread.joinable())
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
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    haveForks = false;
}

void Philosopher::think(){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::stringstream trinkString;
    trinkString << "   " << this->philosopherName << " is thinking." << std::endl;
    std::cout << trinkString.rdbuf();

}

void Philosopher::dine(){
    for(std::size_t i = 0; i < numberOfIteration; ++i){
        think();
        eat();
    }
}

void Philosopher::startThread(){
    philosopherThread = std::thread {&Philosopher::dine, this};
}
