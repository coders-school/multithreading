#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    std::mutex m;
    std::condition_variable cv;
    std::vector<int> v;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    auto consume = [&] {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul);
        std::cout << v.back();
        v.pop_back();
    };
    for (int i = 0; i < 10; i++) {
        consumers.emplace_back(consume);
    }

    auto produce = [&](int i) {
        {
            std::lock_guard<std::mutex> lg(m);
            v.push_back(i);
        }
        cv.notify_all();
    };
    for (int i = 0; i < 10; i++) {
        producers.emplace_back(produce, i);
    }

    for (auto && p : producers) {
        p.join();
    }
    for (auto && c : consumers) {
        c.join();
    }

    return 0;
}
