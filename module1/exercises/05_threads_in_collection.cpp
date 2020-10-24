#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

int main() {
    std::vector<std::thread> threads;

    for(int i = 0; i < 20; ++i) {
        threads.emplace_back([](int id){
            std::this_thread::sleep_for(1s);
            std::string txt = std::to_string(id) + ' ';
            std::cout << txt;
            // std::cout << id << ' ';
        }, i);
    }

    for (auto && t : threads) {
        t.join();
    }

    return 0;
}