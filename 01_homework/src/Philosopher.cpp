#include "Philosopher.hpp"
#include <thread>
#include <chrono>
#include <iomanip>
#include <random>

void philosopher(Table & table, Fork &fork_left, Fork &fork_right) {
    std::mt19937 rng{ std::random_device{}()

    while(table.food) {
        static thread_local std::uniform_int_distribution<> wait(1, 6);
        std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));

        print(" is thinking ");

        std::lock(fork_left.mutex, fork_right.mutex);

        std::lock_guard<std::mutex> left_lock(fork_left.mutex,   std::adopt_lock);
        std::lock_guard<std::mutex> right_lock(fork_right.mutex, std::adopt_lock);

        print(" started eating.");

        static thread_local std::uniform_int_distribution<> dist(1, 6);
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

        print(" finished eating.");
    }
    // try to get forks
    // eat if successful
    // // incearse starving counter if fails
    // time for some thinking
}
