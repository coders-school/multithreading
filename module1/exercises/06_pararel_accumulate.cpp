#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>

template <typename IT, typename T>
T pararelAccumulate([[maybe_unused]] IT first,[[maybe_unused]] IT last, T init) {
    return init;
}

int main() {
    std::vector<int> vec(1'000);
    std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
    auto start = std::chrono::steady_clock::now();
    pararelAccumulate(std::begin(vec), std::end(vec), 0);
    auto stop = std::chrono::steady_clock::now();

    auto start2 = std::chrono::steady_clock::now();
    std::accumulate(std::begin(vec), std::end(vec), 0);
    auto stop2 = std::chrono::steady_clock::now();

    std::cout << "\nPararel algorithm: "
        << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
        << "us" << std::endl;
    std::cout << "\nNormal algorithm: "
        << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count()
        << "us" << std::endl;

    return 0;
}
