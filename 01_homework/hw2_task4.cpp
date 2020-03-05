#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <random>
#include <mutex>
#include <array>

constexpr int dining_time = 10;
constexpr int population = 5;
constexpr int starvation_treshold = 50;

struct Fork
{
   std::mutex mutex;
};

struct Table
{
    bool food = true;

    std::array<Fork, population> forks;
    std::array<int, population> ownership;
    std::array<int, population> starvation;

    std::mutex print_mutex;
    std::mutex starvation_mutex;

    Table();

    void print(bool lock, int id);
    bool check_neighbour_stavation(int id);
};

std::string print_fork(int value) {
    if (value < 0) return "|  ";
    else if (value == 0) return " - ";
    else return "  |";
}

Table::Table() {
    std::fill(ownership.begin(), ownership.end(), 0);
    std::fill(starvation.begin(), starvation.end(), 0);
    print(false, 0);
}

void Table::print(bool lock, int id) {
    std::lock_guard<std::mutex> print_lock(print_mutex);

    // Check border case
    int tmp = (id + 1) % population;

    if (lock) {
        ownership[id] = 1;
        ownership[tmp] = -1;
    } else {
        ownership[id] = 0;
        ownership[tmp] = 0;
    }

    std::stringstream buff;

    for (int i = 0; i < population; i++) {
        buff << print_fork(ownership[i]);
        buff << static_cast<char>(i + 'A');
    }
    buff << print_fork(ownership[0]);
    buff << '\n';

    std::cout << buff.str();
}

bool Table::check_neighbour_stavation(int id) {
    int neighbour_left = (id - 1) % population;
    int neighbour_right = (id + 1) % population;

    std::lock_guard<std::mutex> lock(starvation_mutex);
    if(starvation[neighbour_left] - starvation[id] > starvation_treshold)
        return false;
    if(starvation[neighbour_right] - starvation[id] > starvation_treshold)
        return false;

    return true;
}

void eat(int id, Table &table, Fork &fork_left, Fork &fork_right, std::mt19937 &seed) {
    int result = 0;

    while(result != -1) {
        if(table.check_neighbour_stavation(id)) {
            result = std::try_lock(fork_left.mutex, fork_right.mutex);
        }
        {
            std::lock_guard<std::mutex> lock(table.starvation_mutex);
            table.starvation[id]++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::lock_guard<std::mutex> lock_left(fork_left.mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_right(fork_right.mutex, std::adopt_lock);

    table.print(true, id);
    {
        std::lock_guard<std::mutex> lock(table.starvation_mutex);
        table.starvation[id] = 0;
    }

    std::uniform_int_distribution<> sleep_time{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time(seed) * 200));

    table.print(false, id);
}

void think(int id, std::mt19937 &seed) {
    std::uniform_int_distribution<> sleep_time{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time(seed) * 100));
}

void philosopher(int id, Table &table, Fork &fork_left, Fork &fork_right) {
    std::mt19937 seed{ std::random_device{}() };

    while(table.food) {
        think(id, seed);
        eat(id, table, fork_left, fork_right, seed);
    }
}

int main(int argc, char *argv[]) {
    Table table;
    std::vector<std::thread> philosophers;

    // Run philosophers threads
    for(int i = 0; i < population - 1; i++) {
        philosophers.emplace_back(philosopher,
                                  i,
                                  std::ref(table),
                                  std::ref(table.forks[i]),
                                  std::ref(table.forks[i+1]));
    }
    // Last one shares fork with first once
    philosophers.emplace_back(philosopher,
                              population - 1,
                              std::ref(table),
                              std::ref(table.forks[population - 1]),
                              std::ref(table.forks[0]));

    // Wait till diner is over
    std::this_thread::sleep_for(std::chrono::seconds(dining_time));

    // Call for ending dinner and wait for all thread to close
    table.food = false;
    for(auto && person : philosophers)
  	    if(person.joinable())
            person.join();

    std::cout << "Dinner is over" << std::endl;
    return 0;
}
