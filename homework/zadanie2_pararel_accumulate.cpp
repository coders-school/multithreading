#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>


template <typename IT, typename T>
T pararelAccumulate(IT first, IT last, T init) {
    if (!init) {
        const size_t size = std::distance(first, last);
        const size_t hardware_threads = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 2;
        const size_t chunkSize = size / hardware_threads;
        std::vector<std::thread> threads(hardware_threads - 1);
        std::vector<T> results(hardware_threads);

        auto begin = first;
        for (size_t i = 0; i < hardware_threads - 1; ++i) {
            auto end = std::next(begin, chunkSize);
            threads[i] = std::thread([](IT first, IT last, T& result)
                {
                    result = std::accumulate(first, last, T{});
                }, begin, end, std::ref(results[i]));
            begin = end;
        }

        results[hardware_threads - 1] = std::accumulate(begin, last, T{});

        for (auto && t : threads) {
            t.join();
        }

        return std::accumulate(std::begin(results), std::end(results), init);
    }
    return init;
}


int main() {

    std::vector<int> vec(1'000'000);
    std::generate(begin(vec), end(vec), [x{ 0 }]()mutable{ return ++x; });

    std::cout<< "\nStart of pararel accumulate. \n";
    auto start = std::chrono::steady_clock::now();
    pararelAccumulate(std::begin(vec), std::end(vec), 0);
    auto stop = std::chrono::steady_clock::now();
    std::cout<< "End of paralel accumulate. \n";
    std::cout<< "Time of pararel accumulate function: " 
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n"; 

    std::cout<< "Start of std::accumulate. \n";
    start = std::chrono::steady_clock::now();
    std::accumulate(std::begin(vec), std::end(vec), 0);
    stop = std::chrono::steady_clock::now();
    std::cout<< "End of std::accumulate. \n";
    std::cout<< "Time of std::accumulate function: "
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";
    

    return 0;
}


