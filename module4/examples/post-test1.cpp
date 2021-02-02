#include <thread>
#include <mutex>
#include <iostream>

std::once_flag flag;

void mayThrow(bool shouldThrow) {
    if (shouldThrow) {
        std::cout << "exceptional call\n";
        throw std::exception{};
    }
    std::cout << "returning call\n";
}

void doOnce(bool shouldThrow) try {
    std::call_once(flag, mayThrow, shouldThrow);
} catch(...) {
    // silently ignore
}

int main() {
    std::thread t1(doOnce, true);
    std::thread t2(doOnce, true);
    std::thread t3(doOnce, false);
    std::thread t4(doOnce, true);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
