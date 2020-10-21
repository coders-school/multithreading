#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>

constexpr size_t minSize = 1'000'000;

bool IsOdd (int i) { return ((i%2)==1); }

template <typename IT, class UnaryPredicate>
long long parallelCountIf(IT first, IT last, UnaryPredicate pred) {
    size_t size = std::distance(first, last);
    if (size < minSize)
        return std::count_if(first, last, pred);

    unsigned int numberOfThreads = std::thread::hardware_concurrency();
    if( ! (numberOfThreads > 0 && numberOfThreads < 50)) {
        numberOfThreads = 2;
    }

    std::cout << "Will split work to " << numberOfThreads << " threads." << std::endl;

    std::vector<long long> results(numberOfThreads-1);
    std::vector<std::thread> threads;

    auto chunkSize = size / numberOfThreads;  // range for single thread
    auto localFirst = first;
    auto localLast = std::next(localFirst, chunkSize);
    std::cout << "Split " << size << " into subranges with size of " << chunkSize << std::endl;

    for (unsigned int i = 0; i < numberOfThreads-1; i++)
    {
        localLast = std::next(localFirst, chunkSize);
        auto lambda = [&results, i, &pred](IT first, IT last){
            auto result = std::count_if(first, last, pred);
            results[i] = result; };
        std::thread t{ lambda, localFirst, localLast };
        threads.emplace_back(std::move(t));
        localFirst = localLast;
    }

    long long result = std::count_if(localFirst, last, pred);

    for (auto && t: threads) {
        t.join();
    }

    result = std::accumulate(results.begin(), results.end(), result);

    return result;
}


int main() {
    std::vector<unsigned long long> vec(2'000'003);
    std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });
    auto start = std::chrono::steady_clock::now();
    auto resultA = parallelCountIf(std::begin(vec), std::end(vec), IsOdd);
    auto stop = std::chrono::steady_clock::now();

    auto start2 = std::chrono::steady_clock::now();
    auto resultB = std::count_if(std::begin(vec), std::end(vec), IsOdd);
    auto stop2 = std::chrono::steady_clock::now();

    std::cout << "\nPararel algorithm: "
        << resultA << '\n'
        << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
        << "us" << std::endl;
    std::cout << "\nNormal algorithm: "
        << resultB << '\n'
        << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count()
        << "us" << std::endl;

    return 0;
}
