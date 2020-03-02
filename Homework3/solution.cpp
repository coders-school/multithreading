#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <cassert>

//Multithreading: 54176.1 microseconds
//Single thread: 111140 microseconds
//Results are the same


template<typename It, typename T>
int p_count_if(It first, It last, T pred) {
    const size_t length = std::distance(first, last);

    if (!length)
        return 0;

    const size_t hw_threads = std::thread::hardware_concurrency();
    const size_t threads_count = hw_threads != 0 ? hw_threads : 2;
    const size_t data_chunk = length / threads_count;

    std::vector<int> results(threads_count);
    std::vector<std::thread> threads(threads_count - 1);

    It begin = first;
    for (size_t i = 0; i < (threads_count - 1); i++) {
        It end = std::next(begin, data_chunk);
        threads[i] = std::thread([=](It begin, It end, int &results) {
            results = count_if(begin, end, pred);
        }, begin, end, std::ref(results[i]));
        begin = end;
    }

    results[threads_count - 1] = count_if(begin, last, pred);
    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    auto result = std::accumulate(std::begin(results), std::end(results), 0);

    return result;
}

int main() {
    std::vector<int> v(10'000'000);
    std::iota(v.begin(), v.end(), 0);

    auto predicate = [](int &x) { return (x % 3 == 0); };

    auto start = std::chrono::high_resolution_clock::now();
    auto result1 = p_count_if(begin(v), end(v), predicate);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> delta = end - start;
    std::cout << "Multithreading: " << delta.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto result2 = std::count_if(std::begin(v), std::end(v), predicate);
    end = std::chrono::high_resolution_clock::now();
    delta = end - start;
    std::cout << "Single thread: " << delta.count() << " microseconds" << std::endl;

    if (result1 == result2) {
        std::cout << "Results are the same" << std::endl;
    } else {
        std::cout << "Results do not match" << std::endl;
    }

    assert(result1==result2);

    return 0;
}
