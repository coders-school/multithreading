#include <vector>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 20; ++i) {
        std::thread t{[](int id){
            std::this_thread::sleep_for(1s);
            std::string str = std::to_string(id) + ' ';
            std::cout << str;
        }, i};
        threads.emplace_back(std::move(t));
    }
    for (auto && t : threads) {
        t.join();
    }
    std::cout << '\n';

    return 0;
}
