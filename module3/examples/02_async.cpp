#include <future>
#include <iostream>
#include <thread>

void promise_future_approach() {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    auto function = [](std::promise<int> promise) {
        // ...
        promise.set_value(10);
    };
    std::thread t(function, std::move(promise));
    std::cout << future.get() << std::endl;
    t.join();
}

void async_approach() {
    auto function = []() {
        // ...
        return 20;
    };
    std::future<int> future = std::async(function);
    std::cout << future.get() << std::endl;
}

int main() {
    promise_future_approach();
    async_approach();
    return 0;
}
