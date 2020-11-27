#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

// Secure your code so that each thread can safely enter its full text into the stream.

void do_work(int id) {
    this_thread::sleep_for(100ms);
    std::string txt = "Thread [" + std::to_string(id) + "]: " + "Job done!\n";
    std::cout << txt;
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
