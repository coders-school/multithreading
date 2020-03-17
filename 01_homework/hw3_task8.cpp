#include <vector>
#include <thread>
#include <iterator>
#include <numeric>
#include <iostream>
#include <cassert>
#include <future>

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

    // if empty return init value
    if (length == 0)
        return init;

    // calculate no. of threads
    auto hardware_threads = std::thread::hardware_concurrency() == 0 ? std::thread::hardware_concurrency() : 2;
    auto chunk_size = length / hardware_threads;

    std::vector<T> acc_results(hardware_threads);
    std::vector<std::thread> acc_threads(hardware_threads - 1);

    // create threads
    Iterator chunk_start = first;
    for (unsigned long i = 0; i < (hardware_threads - 1); ++i)
    {
        Iterator chunk_end = chunk_start;
        std::advance(chunk_end, chunk_size);
        acc_threads[i] = std::thread(accumulate_block<Iterator, T>(),
                                 chunk_start,
                                 chunk_end,
                                 std::ref(acc_results[i]));
        chunk_start = chunk_end;
    }

    // find result for last chunk
    accumulate_block<Iterator, T>()(chunk_start, last, acc_results[hardware_threads - 1]);

    // wait for all threads
    for (auto && thread : acc_threads)
        thread.join();

    // Add up results and return
    return std::accumulate(acc_results.begin(), acc_results.end(), init);
}

template<typename Iterator, typename T>
T future_accumulate(Iterator first, Iterator last, T init)
{
    auto length = std::distance(first, last);

    // If empty return init value
    if (length == 0)
        return init;

    // Calculate no. of threads
    auto hardware_threads = std::thread::hardware_concurrency() == 0 ? std::thread::hardware_concurrency() : 2;
    auto chunk_size = length / hardware_threads;

    std::vector<T> acc_results(hardware_threads - 1);
    std::vector<std::future<T>> acc_futures(hardware_threads - 1);
    auto acc_func = [](Iterator first, Iterator last, T init){
        return std::accumulate(first, last, T{});
    };

    // Create threads
    Iterator chunk_start = first;
    for (unsigned long i = 0; i < hardware_threads - 1; ++i)
    {
        Iterator chunk_end = chunk_start;
        std::advance(chunk_end, chunk_size);
        acc_futures[i] = std::async(std::launch::async,
                                    acc_func,
                                    chunk_start,
                                    chunk_end,
                                    T{});
        chunk_start = chunk_end;
    }
    // Run last thread
    auto last_result = acc_func(chunk_start, last, T{});

    // Add up results and return
    auto sum_up = [](T a, std::future<T> &b){
        return std::move(a) + b.get();
    };

    return std::accumulate(acc_futures.begin(),
                           acc_futures.end(),
                           init + last_result,
                           sum_up);
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

    std::cout << "======= Future Accumulate =======\n";
    start = std::chrono::high_resolution_clock::now();
    auto result_fut = future_accumulate(data.begin(), data.end(), 0);
    stop = std::chrono::high_resolution_clock::now();

    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result_fut << '\n';

    assert(result_seq == result_fut);
    return 0;
}
