#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <future>
using namespace std;

std::random_device rd;

int main() {
    auto task = []() {
        std::mt19937 gen(rd());
        std::bernoulli_distribution d(0.5);
        if (d(gen)) {
            throw std::runtime_error("WTF");
        } else {
            return "success";
        }
    };

    auto f = std::async(task);

    std::cout << "Some heavy task on main thread\n";
    std::this_thread::sleep_for(1s);

    try {
        auto result = f.get();
        std::cout << "Task exited normally with result: " << result << '\n';
    } catch(const std::exception& ex) {
        std::cout << "Task exited with an exception: " << ex.what() << "\n";
    }

    return 0;
}
