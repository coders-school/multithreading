#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>

constexpr int minSize = 500'000;

template <typename IT, typename T>
T parallelAccumulate([[maybe_unused]] IT first,[[maybe_unused]] IT last, T init) {
    auto size = std::distance(first, last);
    if (size < minSize) {
        return std::accumulate(first, last, init);
    }

    auto numberOfThreads = std::thread::hardware_concurrency();
    auto chunkSize = size / numberOfThreads;
    std::vector<T> results(numberOfThreads - 1);
    std::vector<std::thread> threads;

    auto localFirst = first;
    auto start3 = std::chrono::steady_clock::now();
    for (size_t i = 0; i < numberOfThreads - 1; ++i) {
        auto localLast = std::next(localFirst, chunkSize);

        auto lambda = [&results, i](IT first, IT last){
            auto result = std::accumulate(first, last, T{});
            results[i] = result;
        };

        std::thread t{lambda, localFirst, localLast};
        threads.emplace_back(std::move(t));
        localFirst = localLast;
    }
    auto stop3 = std::chrono::steady_clock::now();

    auto res = std::accumulate(localFirst, last, T{});

    auto start4 = std::chrono::steady_clock::now();
    for (auto && t : threads) {
        t.join();
    }
    auto stop4 = std::chrono::steady_clock::now();
    std::cout << "\nThread creation: "
        << std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3).count()
        << "us" << std::endl;
    std::cout << "\nThread join: "
        << std::chrono::duration_cast<std::chrono::microseconds>(stop4 - start4).count()
        << "us" << std::endl;

    return std::accumulate(results.begin(), results.end(), init + res);
}

int main() {
    std::vector<int> vec(500'000);
    std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
    auto start = std::chrono::steady_clock::now();
    auto resultP = parallelAccumulate(std::begin(vec), std::end(vec), 0);
    auto stop = std::chrono::steady_clock::now();

    auto start2 = std::chrono::steady_clock::now();
    auto resultA = std::accumulate(std::begin(vec), std::end(vec), 0);
    auto stop2 = std::chrono::steady_clock::now();

    std::cout << "\nParallel algorithm: "
        << resultP << '\n'
        << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
        << "us" << std::endl;
    std::cout << "\nNormal algorithm: "
        << resultA << '\n'
        << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count()
        << "us" << std::endl;

    return 0;
}
