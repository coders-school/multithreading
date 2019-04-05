// solution based on Anthony Williams, C++ Concurrency in Action, Mannings, listing 2.8
// some updates introduced:
// - std::for_each + std::mem_fn replaced to range based for loop
// - automatic type deduction
// - time measurement added

#include <vector>
#include <thread>
#include <iostream>
#include <numeric>
#include <iterator>

template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    auto length = std::distance(first, last);
    
    if (!length)
        return init;
        
    auto hardware_threads = std::thread::hardware_concurrency();
    auto num_threads = hardware_threads != 0 ? hardware_threads : 2;
    auto block_size = length / num_threads;
    
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1); 
    // one less, because we already have one (main thread)  
    
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        // iterator calculation outside threads
        threads[i] = std::thread(accumulate_block<Iterator, T>(),
                                 block_start,
                                 block_end,
                                 std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
    for (auto && thread : threads)
        thread.join();
        
    return std::accumulate(results.begin(), results.end(), init);
}

int main() {
    std::vector<int> v(1'000'000);
    std::iota(v.begin(), v.end(), 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    auto result = parallel_accumulate(v.begin(), v.end(), 0);
    auto stop = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';
              
    return 0;
}
