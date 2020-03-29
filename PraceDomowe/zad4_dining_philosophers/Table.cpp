#include "Table.hpp"

Table::Table(int number):
    philosophersNumber(number), forks(number) {

    for(int i = 0; i < number-1; i++){
      std::string name = "F" + std::to_string(i+1);
          Philosopher newPhilosopher {name, forks[i], forks[i+1]};
        philosophers.emplace_back(std::move(newPhilosopher));
    }
    std::string name = "F" + std::to_string(number);
    Philosopher lastPhilosopher  {name, forks[number-1], forks[0]};
    philosophers.emplace_back(std::move(lastPhilosopher));

    for(auto& elem: philosophers)
        elem.startThread();
}

void Table::showTable(){
    std::cout << std::endl;
    for(auto& elem: philosophers){
        if(elem.getHaveForks() == true){
            std::stringstream eatingPhilosopher;
            eatingPhilosopher << "-" << elem.getPhilosopherName() << "-";
            std::cout << eatingPhilosopher.rdbuf();
        }
        else{
            std::stringstream notEatingPhilosopher;
            notEatingPhilosopher << " " << elem.getPhilosopherName() << " ";
            std::cout << notEatingPhilosopher.rdbuf();
        }
    }
    std::cout << std::endl;
}

bool Table::stopEating(){
    bool checker = true;
    for(auto& elem: philosophers){
        checker = (checker and not(elem.getHaveForks()));
    }
    return checker;
}
