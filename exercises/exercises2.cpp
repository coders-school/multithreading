#include "iterator"
#include "iostream"
#include "numeric"
#include "thread"
#include "vector"


template<typename IT, typename T>
struct pipelineAccumulate
{
    void operator()(IT first, IT last, T &result) {
        result = std::accumulate(first, last, 0);
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
    
    std::vector<T> result(availableThreads);
    std::vector<std::thread> threadvector(availableThreads - 1);

    IT begin = first;
    int i = 0;
    for (auto && iThread : threadvector)
    {
        IT end = begin;
        std::advance(end, dataSize);
        iThread = std::thread(
            pipelineAccumulate<IT, T>(), 
            begin, 
            end, 
            std::ref(result[i])
        );
        begin = end;
        ++i;
    }
    pipelineAccumulate<IT, T>()( begin, last, result[i] );

    for (auto && iThread : threadvector)
        iThread.join();
    
    return std::accumulate( result.begin(), result.end(), init);
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