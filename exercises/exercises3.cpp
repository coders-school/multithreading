#include "algorithm"
#include "iterator"
#include "iostream"
#include "numeric"
#include "thread"
#include "vector"
#include "cassert"


template<typename IT, typename Predictor, typename T>
struct pipelineCount_if
{
    void operator()(IT first, IT last, Predictor predictor, T &result) {
        result = std::count_if(first, last, predictor);
    }
};

template<typename IT, typename Predictor>
std::ptrdiff_t parallelCount_if(IT first, IT last, Predictor predictor)
{
    auto ArraySize = std::distance(first, last);
    if(!ArraySize)
        return 0;

    auto numberOfThreads = std::thread::hardware_concurrency();
    auto availableThreads = numberOfThreads != 0 ? numberOfThreads : 2;
    auto dataSize = ArraySize / availableThreads;
    
    std::vector<std::ptrdiff_t> result(availableThreads);
    std::vector<std::thread> threadvector(availableThreads - 1);

    IT begin = first;
    int i = 0;
    for (auto && iThread : threadvector)
    {
        IT end = begin;
        std::advance(end, dataSize);
        iThread = std::thread(
            pipelineCount_if<IT, Predictor, std::ptrdiff_t>(), 
            begin, 
            end,
            predictor,
            std::ref(result[i])
        );
        begin = end;
        ++i;
    }
    pipelineCount_if<IT, Predictor, std::ptrdiff_t>()( begin, last, predictor, result[i] );

    for (auto && iThread : threadvector)
        iThread.join();
    
    return std::accumulate( result.begin(), result.end(), 0);
}

void Test1()
{
    size_t i = 0;
    std::vector<int> v(1'000'000);
    std::generate(
        v.begin(), 
        v.end(), 
        [&i]()
        { 
            auto r = i%2 == 0 ? 1 : 3;
            ++i;
            return r;
        }
    );
    //count_if
    auto start = std::chrono::high_resolution_clock::now();
    auto pCount_if = std::count_if(v.begin(), v.end(), [](auto it){ return it == 1;});
    auto stop = std::chrono::high_resolution_clock::now();
    // parallel count_if
    auto start1 = std::chrono::high_resolution_clock::now();
    auto result = parallelCount_if(v.begin(), v.end(), [](auto it){ return it == 1;});
    auto stop1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = stop - start;
    std::cout << "Count_if Result" << '\n'
              << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';

    std::chrono::duration<float> duration1 = stop1 - start1;
    std::cout << "Count_if in parallel Result" << '\n'
              << "Duration: " << duration1.count() << '\n'
              << "Result: " << result << '\n';
}

void Test2()
{
    size_t i = 0;
    std::vector<char> v(10'000'000);
    std::generate(
        v.begin(), 
        v.end(), 
        [&i]()
        { 
            auto r = i%2 == 0 ? 'C' : '+';
            ++i;
            return r;
        }
    );
    //count_if
    auto start = std::chrono::high_resolution_clock::now();
    auto pCount_if = std::count_if(v.begin(), v.end(), [](auto it){ return it == 'C';});
    auto stop = std::chrono::high_resolution_clock::now();
    // parallel count_if
    auto start1 = std::chrono::high_resolution_clock::now();
    auto result = parallelCount_if(v.begin(), v.end(), [](auto it){ return it == 'C';});
    auto stop1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = stop - start;
    std::cout << "Count_if Result" << '\n'
              << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';

    std::chrono::duration<float> duration1 = stop1 - start1;
    std::cout << "Count_if in parallel Result" << '\n'
              << "Duration: " << duration1.count() << '\n'
              << "Result: " << result << '\n';
}


int main()
{
    Test1();
    Test2();
    return 0;
}