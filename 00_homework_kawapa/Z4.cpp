#include <atomic>
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
    int eated_ = 0;
    std::vector<Philosopher> & v_;

    void dine()
    {
        while (1)
        {
            contemplate();
            eat();
        }
    }

    void eat()
    {
        wait();
        if (eated_ <= v_[forkIndexLeft_].eated_ || eated_ <= v_[forkIndexRight_].eated_ ||
            (eated_ == v_[forkIndexLeft_].eated_ && eated_ == v_[forkIndexRight_].eated_))
        {
        std::scoped_lock l(forks[forkIndexLeft_], forks[forkIndexRight_]);

        print("has just picked up both forks and he starts eating");

        wait();
        print("has finished his meal", eated_);
        eated_++;
        }
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

    void print(std::string str, int eated)
    {
        std::stringstream ss;
        ss << "F" << id_ << " " << str << " " << eated_ << std::endl;
        std::cout << ss.str();
    }

    friend bool canIeat(const Philosopher & obj1, const Philosopher & obj2, const Philosopher & obj3);

    Philosopher(int id, int left, int right, std::vector<Philosopher> & philosophers)
        : id_(id), forkIndexLeft_(left), forkIndexRight_(right), v_(philosophers) { }

};

    std::vector<Philosopher> philosophers;

int main()
{
    std::vector<std::thread> threads(n);

    for (size_t i = 0; i < n - 1; i++)
        philosophers.emplace_back(i, i, i+1, philosophers);
    philosophers.emplace_back(n - 1, n - 1, 0, philosophers);

    for (size_t i = 0; i < n; i++)
        threads[i] = std::thread(&Philosopher::dine, &philosophers[i]);

    for (auto &&i : threads)
        i.join();
    
    return 0;
}