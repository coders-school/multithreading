#include <vector>
#include <thread>
#include <iterator>
#include <numeric>
#include <iostream>
#include <cassert>
#include <future>

template<typename Iterator>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last)
    {
        return std::accumulate(first, last, 0);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    auto length = std::distance(first, last);

    // If empty return init value
    if (length == 0)
        return init;

    // Calculate no. of threads
    auto hardware_threads = std::thread::hardware_concurrency() == 0 ? std::thread::hardware_concurrency() : 2;
    auto chunk_size = length / hardware_threads;

    std::vector<T> acc_results(hardware_threads);
    std::vector<std::future<T>> acc_futures(hardware_threads - 1);
    auto acc_func = [](Iterator first, Iterator last){
        return std::accumulate(first, last, 0);
    };

    // Create threads
    Iterator chunk_start = first;
    for (unsigned long i = 0; i < (hardware_threads - 1); ++i)
    {
        Iterator chunk_end = chunk_start;
        std::advance(chunk_end, chunk_size);
        acc_futures[i] = std::async(acc_func, chunk_start, chunk_end);
        chunk_start = chunk_end;
    }

    // Find result for last chunk
    acc_results[hardware_threads - 1] = acc_func(chunk_start, last);

    // Get all results
    for(unsigned long i = 0; i < (hardware_threads - 1); i++)
        acc_results[i] = acc_futures[i].get();

    // Add up results and return
    return std::accumulate(acc_results.begin(), acc_results.end(), init);
}

int main() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);

    std::cout << "Supported hardware threads: " << std::thread::hardware_concurrency() << '\n';

    std::cout << "========== Accumulate ===========\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto result_seq = std::accumulate(data.begin(), data.end(), 0);
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result_seq << '\n';

    std::cout << "====== Parallel Accumulate ======\n";
    start = std::chrono::high_resolution_clock::now();
    auto result_par = parallel_accumulate(data.begin(), data.end(), 0);
    stop = std::chrono::high_resolution_clock::now();

    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result_par << '\n';

    assert(result_seq == result_par);
    return 0;
}
