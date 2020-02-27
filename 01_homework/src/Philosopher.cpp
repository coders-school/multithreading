#include "Philosopher.hpp"

#include <

Philosopher::Philosopher(table const &table,
                         fork& f_left,
                         fork& f_right ) noexcept
    : table_(table),
      fork_left_(f_left),
      fork_right_(f_right) {}

Philosopher::~Philosopher() {
    //thread.join();
}

Philosopher::dine() {
    // try to get forks
    // eat if successful
    // // incearse starving counter if fails
    // time for some thinking
}
