#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <random>
#include <mutex>
#include <array>
//#include "Table.hpp"
//#include "Philosopher.hpp"

std::mutex print_mutex;

struct Fork
{
   std::mutex mutex;
};

//template<std::size_t S>
struct Table
{
    bool food = true;
    std::array<Fork, 5> forks;
    std::array<int, 5> ownership;
    std::array<bool, 5> thinking;
    // array for starving values

    void print(bool lock, int id);
};

std::string print_fork(int value) {
    if (value < 0) return "|  ";
    else if (value == 0) return " - ";
    else return "  |";
}

void Table::print(bool lock, int id) {
    std::lock_guard<std::mutex> print_lock(print_mutex);

    int tmp = id + 1;
    if (id == 5) tmp = 0;

    if (lock) {
        ownership[id] = 1;
        ownership[tmp] = -1;
        thinking[id] = false;
    } else {
        ownership[id] = 0;
        ownership[tmp] = 0;
        thinking[id] = true;
    }

    std::stringstream buff;
    int ascii_value = 65;
    if (thinking[id]) ascii_value = 97;

    for (int i = 0; i < 5; i++) {
        buff << print_fork(ownership[i]);
        buff << static_cast<char>(i + ascii_value);
    }
    buff << print_fork(ownership[0]);
    buff << '\r';

    std::cout << buff.str();
}

void eat (int id, Table &table, Fork &fork_left, Fork &fork_right, std::mt19937 &seed) {
    //std::scoped_lock try_lock(fork_left.mutex, fork_right.mutex);
    //int result = std::try_lock(fork_left.mutex, fork_right.mutex);
    std::lock(fork_left.mutex, fork_right.mutex);
    std::lock_guard<std::mutex> lock_left(fork_left.mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_right(fork_right.mutex, std::adopt_lock);

    //if (result == -1) {
        table.print(true, id);

        std::uniform_int_distribution<> sleep_time{1, 10};
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time(seed) * 100));

        table.print(false, id);
    //}
}

void philosopher(int id, Table &table, Fork &fork_left, Fork &fork_right) {
    std::mt19937 seed{ std::random_device{}() };

    while(table.food) {
        std::uniform_int_distribution<> eat_time{1, 10};
        std::this_thread::sleep_for(std::chrono::milliseconds(eat_time(seed) * 20));

        //print(" is thinking ");

        eat(id, table, fork_left, fork_right, seed);
    }
    // try to get forks
    // eat if successful
    // // incearse starving counter if fails
    // time for some thinking
}

int main(int argc, char *argv[]) {
    Table table;

    //std::cout << "Hello world!" << table.food << std::endl;
    std::vector<std::thread> philosophers;

    for(int i = 0; i < 4; i++) {
        philosophers.emplace_back(philosopher,
                                  i,
                                  std::ref(table),
                                  std::ref(table.forks[i]),
                                  std::ref(table.forks[i+1]));
    }
    //last one shares fork with first once
    philosophers.emplace_back(philosopher,
                              5,
                              std::ref(table),
                              std::ref(table.forks[5]),
                              std::ref(table.forks[0]));

    for(auto && person : philosophers)
  	    person.join();

    std::this_thread::sleep_for(std::chrono::seconds(5));
    table.food = false;
}
