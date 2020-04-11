#include "Calculate.hpp"
#include <functional>


int calculate(const std::string & q, const std::string & a)
{
    std::hash<std::string> h;
    std::string tmp = q + a;
    return h(tmp) % 1234;
}