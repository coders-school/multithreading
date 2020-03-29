#pragma once

#include "Fork.hpp"
#include "Philosopher.hpp"
#include <vector>

class Table{ 
private:
    int philosophersNumber;
    std::vector<Fork> forks;
    std::vector<Philosopher> philosophers;
    
public:
    Table(int number);

    void showTable();
    bool stopEating();
};
