#include <chrono>
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <vector>

constexpr size_t n = 15;
// number of philosophers and forks

std::mutex forks[n];

struct Philosopher
{
    int id_;
    int forkIndexLeft_;
    int forkIndexRight_;
    bool hungry_ = true;

    void dine()
    {
        wait();

        while (hungry_)
        {
            contemplate();
            eat();
        }
    }

    void eat()
    {
        wait();
        std::lock(forks[forkIndexLeft_], forks[forkIndexRight_]);
 
        std::lock_guard<std::mutex> mutex_l(forks[forkIndexLeft_], std::adopt_lock);
        print("has just picked up the left fork");

        wait();
        std::lock_guard<std::mutex> mutex_r(forks[forkIndexRight_], std::adopt_lock);
        print("has just picked up the right fork and he starts eating");

        wait();
        print("has finished his meal");
        hungry_ = false; 
    }

    void contemplate()
    {
        wait();
        print("is contemplating");
    }

    void wait()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 3000));
    }

    void print(std::string str)
    {
        std::stringstream ss;
        ss << "F" << id_ << " " << str << std::endl;
        std::cout << ss.str();
    }

    Philosopher(int id, int left, int right)
        : id_(id), forkIndexLeft_(left), forkIndexRight_(right) { }
};

int main()
{
    std::vector<std::thread> threads(n);
    std::vector<Philosopher> philosophers;

    for (size_t i = 0; i < n - 1; i++)
        philosophers.emplace_back(i, i, i+1);
    philosophers.emplace_back(n - 1, n - 1, 0);

    for (size_t i = 0; i < n; i++)
        threads[i] = std::thread(&Philosopher::dine, &philosophers[i]);

    for (auto &&i : threads)
        i.join();
    
    return 0;
}