#pragma once

#include "Fork.hpp"
#include <array>

const int philosophersNumber = 5;

class Table{ 
    std::array <Fork, philosophersNumber> forks;
};
