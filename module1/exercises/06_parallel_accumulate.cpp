#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>

template <typename It, typename T>
T parallelAccumulate([[maybe_unused]] It first,[[maybe_unused]] It last, T init) {
    return init;
}

int main() {
    std::vector<int> vec(1'000'000);
    std::generate(begin(vec), end(vec), [x{0}]() mutable { return ++x; });
    auto start = std::chrono::steady_clock::now();
    parallelAccumulate(std::begin(vec), std::end(vec), 0);
    auto stop = std::chrono::steady_clock::now();

    auto start2 = std::chrono::steady_clock::now();
    std::accumulate(std::begin(vec), std::end(vec), 0);
    auto stop2 = std::chrono::steady_clock::now();

    std::cout << "\nParallel algorithm: "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
              << "us" << std::endl;
    std::cout << "\nNormal algorithm: "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count()
              << "us" << std::endl;

    return 0;
}
