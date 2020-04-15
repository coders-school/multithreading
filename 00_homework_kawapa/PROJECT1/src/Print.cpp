#include "Print.hpp"
#include <iostream>
#include <sstream>

void printFunc( const std::string &str, 
                const std::string &name )
{
    std::stringstream ss;
    ss << name << " " << str << std::endl;
    std::cout << ss.str();
}

void printFunc( const std::string &str, 
                const int &i, 
                const int &result, 
                const std::string &name )
{
    std::stringstream ss;
    ss << name << " " << str << " " << i + 1 << " answer" << " - " << result << std::endl;
    std::cout << ss.str();
}

void printFunc(const int &i, const int &j, const std::string &name)
{
    std::stringstream ss;
    ss << name << " answers " << i << " " << "for the " << j << "th question" << std::endl;
    std::cout << ss.str();
}