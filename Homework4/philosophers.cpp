#include <chrono>
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <vector>

constexpr size_t n = 9;

std::mutex forks[n];

class Philosopher {
public:

    int meal = 0;

    void dine() {
        while (true) {
            contemplate();
            eat();
        }
    }

    Philosopher(int id, int left, int right, std::vector<Philosopher> &philosophers)
            : id(id), leftFork(left), rightFork(right), guests(philosophers) {}

private:
    int id;
    int leftFork;
    int rightFork;

    std::vector<Philosopher> &guests;

    void eat() {
        wait();
        if (meal <= guests[leftFork].meal || meal <= guests[rightFork].meal ||
            (meal == guests[leftFork].meal && meal == guests[rightFork].meal)) {
            std::scoped_lock l(forks[leftFork], forks[rightFork]);
            print("has 2 forks now and starts to eat");
            wait();
            print("has just finished his meal", meal);
            meal++;
        }
    }

    void contemplate() {
        print("is contemplating");
        wait();
    }

    static void wait() {
        std::this_thread::sleep_for(std::chrono::milliseconds{std::rand() % 5000});
    }

    void print(const std::string &str, int meal_ = -1) {
        std::stringstream ss;
        ss << "Philosopher " << id << " " << str;
        if (meal_ < 0)
            ss << " " << meal;
        ss << std::endl;
        std::cout << ss.str();
    }
};


int main() {

    std::vector<Philosopher> philosophers;

    std::vector<std::thread> threads(n);

    for (size_t i = 0; i < n - 1; i++)
        philosophers.emplace_back(i, i, i + 1, philosophers);
    philosophers.emplace_back(n - 1, n - 1, 0, philosophers);

    for (size_t i = 0; i < n; i++)
        threads[i] = std::thread(&Philosopher::dine, &philosophers[i]);

    for (auto &&i : threads)
        i.join();

    return 0;
}
