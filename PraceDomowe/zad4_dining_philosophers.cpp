#include <iostream>
#include <thread>
#include <mutex>
#include <string>

class Fork{                                 // DO ZMIANY - dodane aby kod się kompilowal
public:
    std::mutex mutex;
};

class Philosopher{
private:
    std::string philosopherName;
    Fork& leftFork;                         // uzycie obiektu klasy "Fork"
    Fork& rightFork;                        // uzycie obiektu klasy "Fork"
    std:: thread philosopherThread;

public:
    Philosopher(std::string name, Fork lFork, Fork rFork):
                philosopherName(name), leftFork(lFork), rightFork(rFork), philosopherThread(&Philosopher::dine, this){
    
    }

    ~Philosopher(){
        philosopherThread.join();
    }

    void eat(){                             //przyklad implementacji - uuzycie "mutex" z klasy Fork
    std::unique_lock<std::mutex> leftLock(leftFork.mutex, std::defer_lock);
    std::unique_lock<std::mutex> rightLock(rightFork.mutex, std::defer_lock);
    lock(leftLock, rightLock);

    std::cout << this->philosopherName << "start eating." << std::endl;
    std::cout << this->philosopherName << "stop eating." << std::endl;

    leftFork.mutex.unlock();
    rightFork.mutex.unlock();
    }

    void think(){
        std::cout << this->philosopherName << "is thinking" << std::endl;
    }

    void dine(){
        think();
        eat();
    }
};

int main(){

    return 0;
}