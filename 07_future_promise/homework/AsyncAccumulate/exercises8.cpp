#include <iterator>
#include <iostream>
#include <numeric>
#include <thread>
#include <future>
#include <vector>



template<typename IT, typename T>
struct pipelineAccumulate
{
    T operator()(IT first, IT last) {
        return std::accumulate(first, last, 0);
    }
};

template<typename IT, typename T>
T parallelAccumulate(IT first, IT last, T init)
{
    auto ArraySize = std::distance(first, last);
    if(!ArraySize)
        return init;

    auto numberOfThreads = std::thread::hardware_concurrency();
    auto availableThreads = numberOfThreads != 0 ? numberOfThreads : 2;
    auto dataSize = ArraySize / availableThreads;
    
    std::vector<std::future<T>> threadvector(availableThreads - 1);

    IT begin = first;
    for (auto && ifuture : threadvector)
    {
        IT end = begin;
        std::advance(end, dataSize);
        ifuture = std::async(
            std::launch::async,
            pipelineAccumulate<IT, T>(), 
            begin,
            end
        );
        begin = end;
    }

    T result = init + pipelineAccumulate<IT, T>()( begin, last);

    
    for (auto && ifuture : threadvector)
        result += ifuture.get();

    return result;
}

int main()
{
    std::vector<int> v(1'000'000);
    std::iota(v.begin(), v.end(), 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    auto result = parallelAccumulate(v.begin(), v.end(), 0);
    auto stop = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';

    return 0;
}