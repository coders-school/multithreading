#include "Table.hpp"
#include <utility>
#include <iostream>
#include <sstream>

Table::Table(const int &numberOfPhilosophers)
: forks{numberOfPhilosophers},
  numberOfPhilosophers_(numberOfPhilosophers) {}

Table::~Table()
{
    for (auto&& thread : threads)
    {
        thread.join();
    }
    std::cout<< "Dinner over\n\n";
}

void Table::StartDinner()
{
    std::cout << "\nDinner begins\n\n";
    for (size_t i=0; i < numberOfPhilosophers_; ++i)
    {
        
        if (i == 0)
        {
            philosophersPTR.emplace_back(Philosopher("Philo" + std::to_string(i), forks[numberOfPhilosophers_-1], forks[i]));
            threads.emplace_back(std::thread(&Philosopher::dine, philosophersPTR[i]));
        }
        else
        {
            philosophersPTR.emplace_back(Philosopher("Philo" + std::to_string(i), forks[i-1], forks[i]));
            threads.emplace_back(std::thread(&Philosopher::dine, philosophersPTR[i]));
        }
    }
}

