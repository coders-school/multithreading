#include "Philosopher.hpp"
#include <string>
#include <thread>
#include <

class Philosopher {
   std::string const name;
   table const &     dinnertable;
   fork&             left_fork;
   fork&             right_fork;
   std::thread       lifethread;
};
