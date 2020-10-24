#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>

[[maybe_unused]] constexpr int minSize = 100;

template <typename It, typename T>
T parallelAccumulate([[maybe_unused]] It first,[[maybe_unused]] It last, T init) {
    auto totalSize = std::distance(first, last);
    if (totalSize < minSize) {
        return std::accumulate(first, last, init);
    }
    auto numberOfThreads = 1000u;
    if (numberOfThreads == 0) {
        numberOfThreads = 2;
    }
    auto chunkSize = totalSize / numberOfThreads;
    std::vector<T> results(numberOfThreads - 1);
    std::vector<std::thread> threads;

    auto partialAccumulate = [](It start, It end, T& result){
        result = std::accumulate(start, end, T{});
    };

    auto localFirst = first;

    for (unsigned int i = 0; i < numberOfThreads - 1; ++i) {
        auto localLast = std::next(localFirst, chunkSize);
        std::thread t{partialAccumulate, localFirst, localLast, std::ref(results[i])};
        threads.emplace_back(std::move(t));
        localFirst = localLast;
    }

    T res = 0;
    partialAccumulate(localFirst, last, res);

    for (auto && th : threads) {
        th.join();
    }

    return std::accumulate(results.begin(), results.end(), init + res);
}

int main() {
    std::vector<double> vec(1'500'001);
    std::generate(begin(vec), end(vec), [x{0.5}]() mutable { return ++x; });
    auto start = std::chrono::steady_clock::now();
    auto parallelResult = parallelAccumulate(std::begin(vec), std::end(vec), 0.0);
    auto stop = std::chrono::steady_clock::now();

    auto start2 = std::chrono::steady_clock::now();
    auto result = std::accumulate(std::begin(vec), std::end(vec), 0.0);
    auto stop2 = std::chrono::steady_clock::now();

    std::cout << "\nParallel algorithm: "
              << parallelResult << '\n'
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
              << "us" << std::endl;
    std::cout << "\nNormal algorithm: "
              << result << '\n'
              << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count()
              << "us" << std::endl;

    return 0;
}
