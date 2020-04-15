#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <parallel/algorithm>
#include <thread>
#include <vector>

template <typename IT, typename Pred>
int p_count_if(IT first, IT last, Pred pred);

int main()
{   
    std::vector<int> v(1'000'000);

    std::iota(v.begin(), v.end(), 0);

    auto lambda = [](int &a){ return (a % 11 == 0); };
    // Example function for multithreaded count_if (p_count_if)
    // How many numbers from 0 to 1`000`000 are divisible by 11 without a remainder 

    auto start = std::chrono::high_resolution_clock::now();
    auto result1 = p_count_if(begin(v), end(v), lambda);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> difference = end - start;
    std::cout << "Multithreaded version: " << difference.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto result2 = std::count_if(std::begin(v), std::end(v), lambda);
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    std::cout << "Singlethread version: " << difference.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto result3 = __gnu_parallel::count_if(std::begin(v), std::end(v), lambda);
    end = std::chrono::high_resolution_clock::now();
    difference = end - start;
    std::cout << "Built-in parallel version (__gnu_parallel::count_if): " << difference.count() << " ms" << std::endl;
    // Requires -fopenmp flag
    
    assert(result1 == result2);
    // Compare results of std::count_if and the user-defined p_count_if

    return 0;
}

template <typename IT, typename Pred>
int p_count_if(IT first, IT last, Pred pred)
{
    const size_t size = std::distance(first, last);

    if (!size)
        return 0;
   
    const size_t threadsInCPU = std::thread::hardware_concurrency();
    const size_t usedThreads = threadsInCPU != 0 ? threadsInCPU : 2;
    const size_t dataChunk = size / usedThreads;

    std::vector<int> results(usedThreads);
    std::vector<std::thread> threads(usedThreads - 1);

    IT begin  = first;
    for (size_t i = 0; i < (usedThreads - 1); i++)
    {
        IT end = std::next(begin, dataChunk);
        threads[i] = std::thread([=](IT begin, IT end, int&results)
        {
            results  = count_if(begin, end, pred);
        }, begin, end, std::ref(results[i]));
    begin = end;
    }

    results[usedThreads - 1] = count_if(begin, last, pred);

    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));

    return std::accumulate(std::begin(results), std::end(results), 0);
}