#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>

template <typename IT, typename T>
T p_accumulate(IT first, IT last, T init);

int main()
{   
    std::vector<int> v(1'000'000);

    std::iota(v.begin(), v.end(), 0);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = p_accumulate(begin(v), end(v), 0);
    auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> difference = end - start;
    std::cout << "Execution time: " << difference.count() << " ms" << std::endl;  
    std::cout << "Result: " << result;

    return 0;
}

template <typename IT, typename T>
T p_accumulate(IT first, IT last, T init)
{
    const size_t size = std::distance(first, last);

    if (!size)
        return init;
   
    const size_t threadsInCPU = std::thread::hardware_concurrency();
    const size_t usedThreads = threadsInCPU != 0 ? threadsInCPU : 2;
    const size_t dataChunk = size / usedThreads;

    std::vector<T> results(usedThreads);
    IT begin  = first;

    for (size_t i = 0; i < (usedThreads - 1); i++)
    {
        IT end = std::next(begin, dataChunk);

        auto function = [&](){
        results[i] = std::accumulate(begin, end, T{});
        };

        std::async(function);
        begin = end;
    }

    results[usedThreads - 1] = std::accumulate(begin, last, T{});
    
    return std::accumulate(std::begin(results), std::end(results), init);
}