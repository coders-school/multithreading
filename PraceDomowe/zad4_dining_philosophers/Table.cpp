#include "Table.hpp"

Table::Table(int number):
    philosophersNumber(number) {
        std::vector<Fork> forks(philosophersNumber);

        //wersja 1 - nic nei przekazujemy do kontruktora flozofow
        std::vector<Philosopher> philosophers(philosophersNumber);
        //wersja 2 
        std::vector<Philosopher> philosophers;
        for(int i = 0; i < philosophersNumber-1; i++){
          std::string name = "F" + std::to_string(i+1);
          philosophers.emplace_back(Philosopher{name, forks[i], forks[i+1]});
        }
        std::string name = "F" + std::to_string(philosophersNumber);
        philosophers.emplace_back(Philosopher{name, forks[philosophersNumber-1], forks[0]});
}