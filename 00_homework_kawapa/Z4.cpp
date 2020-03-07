#include <atomic>
#include <chrono>
#include <iostream>
#include <sstream>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

struct Fork
{
    mutable std::mutex m_;
} forks[5];

struct Philosopher
{
    std::string name_;
    int forkIndexLeft_;
    int forkIndexRight_;
    bool hungry_ = true;

    void dine()
    {
        contemplate();
        eat();
    }

    void eat()
    {
        std::stringstream ss;
        ss << name_ << "is eating\n";
        std::cout << ss.str();
        std::atomic<bool> hungry {true};
    }

    void contemplate()
    {
        std::stringstream ss;
        ss << name_ << "is contemplating\n";
        std::cout << ss.str();
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 250));
    }

    Philosopher(const std::string &name, int left, int right)
        : name_(name), forkIndexLeft_(left), forkIndexRight_(right) { }
};

int main()
{
    size_t n = 5;
    std::vector<std::thread> threads(n);

    std::vector<Philosopher> philosophers {
            {"Tales", 0, 1          },
            {"Pitagoras", 1, 2      },
            {"Sokrates", 2, 3       },
            {"Platon", 3, 4         },
            {"Arystoteles", 4, 0    }
    };

    for (int i = 0; i < n; i++)
        threads[i] = std::thread(&Philosopher::dine, &philosophers[i]);

    for (auto &&i : threads)
        i.join();
    
    return 0;
}