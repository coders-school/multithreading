#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <string>

auto globalLambda = [](int a, int b) {
    std::cout << "globalLambda:\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return std::pow(a, b);
};

void localPackagedTask() {
    std::cout << "\nlocalPackagedTask:\n";
    std::packaged_task<int(int, int)> task(globalLambda);
    auto result = task.get_future();

    std::cout << "before task execution\n";
    task(2, 9);
    std::cout << "after task execution\n";

    std::cout << "getting result:\t" << result.get() << '\n';
}

void remotePackagedTask() {
    std::cout << "\nremotePackagedTask:\n";
    std::packaged_task<int(int, int)> task(globalLambda);
    auto result = task.get_future();

    std::cout << "before task execution\n";
    std::thread t(std::move(task), 2, 9);
    std::cout << "after task execution\n";
    t.detach();
    std::cout << "getting result:\t" << result.get() << '\n';
}

void remoteAsync() {
    std::cout << "\nremoteAsync:\n";
    auto result = std::async(std::launch::async, globalLambda, 2, 9);
    std::cout << "getting result:\t" << result.get() << '\n';
}

int main() {
    localPackagedTask();
    remotePackagedTask();
    remoteAsync();
}
