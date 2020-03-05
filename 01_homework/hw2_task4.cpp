#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <random>
#include <mutex>
#include <array>

#define LOG

constexpr int dining_time = 10;
constexpr int population = 5;
constexpr int starvation_treshold = 50;

std::mutex print_mutex;

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

    std::mutex starvation_mutex;

    void print(bool lock, int id);
    bool check_neighbour_stavation(int id);
};

#ifdef LOG
void log_print(int id, std::string_view text)
{
   std::lock_guard<std::mutex> cout_lock(print_mutex);
   std::cout << static_cast<char>(id+65) << text << std::endl;
}
#endif

std::string print_fork(int value) {
    if (value < 0) return "|  ";
    else if (value == 0) return " - ";
    else return "  |";
}

void Table::print(bool lock, int id) {
    std::lock_guard<std::mutex> print_lock(print_mutex);

    // Check border case
    int tmp = id + 1;
    if (tmp == population)
        tmp = 0;

    if (lock) {
        ownership[id] = 1;
        ownership[tmp] = -1;
    } else {
        ownership[id] = 0;
        ownership[tmp] = 0;
    }

    std::stringstream buff;
    const int ascii_value = 65;

#ifdef LOG
    buff << "[" << id << "," << lock << "]: ";
#endif

    for (int i = 0; i < population; i++) {
        buff << print_fork(ownership[i]);
        buff << static_cast<char>(i + ascii_value);
#ifdef LOG
        buff << check_neighbour_stavation(i) << "." << starvation[i];
#endif
    }
    buff << print_fork(ownership[0]);
    buff << '\n';

    std::cout << buff.str();
}

bool Table::check_neighbour_stavation(int id) {
    int neighbour_left = id - 1;
    int neighbour_right = id + 1;

    // Check border cases
    if(neighbour_left < 0)
        neighbour_left = population - 1;
    if(neighbour_right == population)
        neighbour_right = 0;

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
#ifdef LOG
    log_print(id, " started eating.");
#endif
    std::uniform_int_distribution<> sleep_time{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time(seed) * 200));

    table.print(false, id);
#ifdef LOG
    log_print(id, " finished eating.");
#endif
}

void think(int id, std::mt19937 &seed) {
    std::uniform_int_distribution<> sleep_time{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time(seed) * 100));

#ifdef LOG
    log_print(id, " is done thinking ");
#endif
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
    std::fill(table.ownership.begin(), table.ownership.end(), 0);
    std::fill(table.starvation.begin(), table.starvation.end(), 0);
    table.print(false, 0);

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
