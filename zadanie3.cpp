#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <numeric>
#include <iterator>
#include <chrono>
#include <assert.h>

constexpr size_t minDataSize = 5000;

template <typename IT, typename UP>
int t_count_if(IT first, IT last, UP predicate)
{
    const size_t distance = std::distance(first, last);
    const size_t maxThreads = std::thread::hardware_concurrency();
    if(distance < minDataSize or maxThreads == 0)
        return std::count_if(first, last, predicate);

    const size_t neededThreads = std::min(distance/minDataSize, maxThreads);
    const size_t dataChunk = distance / neededThreads;
    std::vector<size_t> results (neededThreads);
    std::vector<std::thread> vThreads (neededThreads-1);

    auto begin = first;
    
    for(size_t i = 0; i < neededThreads-1; ++i)
    {
        auto end = std::next(begin, dataChunk);
        vThreads[i] = std::thread([](IT first, IT last, UP predicate, size_t &result)
        {   
            result = std::count_if(first, last, predicate);
        }, begin, end, predicate, std::ref(results[i]));
        begin = end;
    };

    results[neededThreads-1]=std::count_if(begin, last, predicate);
    std::for_each(vThreads.begin(), vThreads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(std::begin(results), std::end(results), 0);
}

void printTime (const decltype(std::chrono::high_resolution_clock::now()) &startStandard,
                const decltype(startStandard) &stopStandard,
                const decltype(startStandard) &startPararel,
                const decltype(startPararel) &stopPararel,
                const size_t &elements)
{
     std::cout << "\nPararel algorithm for [" << elements << "]: " 
		<< std::chrono::duration_cast<std::chrono::microseconds>(stopPararel - startPararel).count() 
		<< "us" << std::endl;
	std::cout << "\nNormal algorithm for [" << elements << "]: "
		<< std::chrono::duration_cast<std::chrono::microseconds>(stopStandard - startStandard).count() 
		<< "us" << std::endl;
}

void useAlgorithm(const std::vector<int> &v)
{
    auto start = std::chrono::high_resolution_clock::now();
    size_t standardCount = std::count_if(v.begin(), v.end(), [](const int &a){return a%2 == 0;});
    auto stop = std::chrono::high_resolution_clock::now();

    auto start3 = std::chrono::high_resolution_clock::now();
    size_t pararelCount = t_count_if(std::begin(v), std::end(v), [](const int &a){return a%2 == 0;});
    auto stop3 = std::chrono::high_resolution_clock::now();

    assert(standardCount==pararelCount);

    printTime(start, stop, start3, stop3, v.size());
}

int main()
{
    std::vector <int> v (400'000'000);
    std::generate(v.begin(), v.end(),  [n=0] () mutable {return n++;});
    useAlgorithm(v);

    v.resize(100'000'000);
    useAlgorithm(v);

    v.resize(5'000);
    useAlgorithm(v);

    v.resize(2'500);
    useAlgorithm(v);
    
    return 0;
}