#include <vector>
#include <thread>
#include <iterator>
#include <numeric>
#include <iostream>
#include <algorithm>
//#include <execution>

template<typename T>
using iter_type = typename std::iterator_traits<T>::difference_type;

template<typename Iterator, typename Predicate, typename T>
struct count_if_block
{
    void operator()(Iterator first, Iterator last, Predicate &policy, T& result)
    {
        result = std::count_if(first, last, policy);
    }
};

template<typename Iterator, typename Predicate>
iter_type<Iterator> parallel_count_if(
    Iterator first,
    Iterator last,
    Predicate policy)
{
    auto length = std::distance(first, last);

    // if empty return 0
    if (length == 0)
        return 0;

    // calculate no. of threads
    auto hardware_threads = std::thread::hardware_concurrency() == 0 ?
            std::thread::hardware_concurrency() : 2;
    auto chunk_size = length / hardware_threads;

    std::vector<iter_type<Iterator>> count_results(hardware_threads);
    std::vector<std::thread> count_threads(hardware_threads - 1);

    // create threads
    Iterator chunk_start = first;
    for (unsigned long i = 0; i < (hardware_threads - 1); ++i)
    {
        Iterator chunk_end = chunk_start;
        std::advance(chunk_end, chunk_size);
        count_threads[i] = std::thread(count_if_block<Iterator, Predicate, iter_type<Iterator>>(),
                                chunk_start,
                                chunk_end,
                                std::ref(policy),
                                std::ref(count_results[i]));
        chunk_start = chunk_end;
    }

    // find result for last chunk
    count_if_block<Iterator, Predicate, iter_type<Iterator>>()
        (chunk_start, last, policy, count_results[hardware_threads - 1]);

    // wait for all threads
    for (auto && thread : count_threads)
        thread.join();

    iter_type<Iterator> ret = 0;
    // Add up results and return
    return std::accumulate(count_results.begin(), count_results.end(), ret);
}

int main() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);

    std::cout << "Supported hardware threads: " << std::thread::hardware_concurrency() << '\n';

    std::cout << "========== Count If ===========\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto result = std::count_if(data.begin(), data.end(), [](int i){return i % 7 == 0;});
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';

    std::cout << "====== Parallel Count If ======\n";
    start = std::chrono::high_resolution_clock::now();
    result = parallel_count_if(data.begin(), data.end(), [](int i){return i % 7 == 0;});
    stop = std::chrono::high_resolution_clock::now();

    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';
/* my compiler doesn't support execution lib :(
    std::cout << "======== Count If::par ========\n";
    start = std::chrono::high_resolution_clock::now();
    result = count_if(std::execution::par, data.begin(), data.end(), 0);
    stop = std::chrono::high_resolution_clock::now();

    duration = stop - start;
    std::cout << "Duration: " << duration.count() << '\n'
              << "Result: " << result << '\n';
*/
    return 0;
}
