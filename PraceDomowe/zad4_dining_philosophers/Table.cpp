#include "Table.hpp"

Table::Table(int number):
    philosophersNumber(number) {
    std::vector<Fork> forks(number);

    std::vector<Philosopher> philosophers;
    for(int i = 0; i < number-1; i++){
      std::string name = "F" + std::to_string(i+1);
          Philosopher newPhilosopher {name, forks[i], forks[i+1]};
        philosophers.emplace_back(std::move(newPhilosopher));
    }
    std::string name = "F" + std::to_string(number);
    Philosopher lastPhilosopher  {name, forks[number-1], forks[0]};
    philosophers.emplace_back(std::move(lastPhilosopher));
}