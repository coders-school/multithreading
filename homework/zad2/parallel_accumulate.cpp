#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <execution>
#include <parallel/numeric>


template <typename Iterator, typename T>
T p_accumulate(Iterator first, Iterator last, T init){
    const size_t size = std::distance(first, last);
    if(size == 0) // Co jeżeli: The value may be negative if random-access iterators are used and first is reachable from last (since C++11)? dodać warunek na łapanie < od 0
        return init;
    const size_t hardware_threads = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 2;
    const size_t block_size_for_thread = size / hardware_threads;
    const size_t used_threads = std::min(block_size_for_thread, hardware_threads);
    const size_t data_chunk = size / used_threads;

    std::vector<T> results(used_threads);
    std::vector<std::thread> threads(used_threads - 1);
    Iterator begin_it = first;
    for(size_t i = 0; i < used_threads - 1; ++i)
    {
        Iterator end = std::next(begin_it, data_chunk); //Można jawnie skonwertować data_chunk na iterator z template - Iterator? Przy static_cast<Iterator>(data_chunk) rzuca że nie może zmaczować, a tak to warningi rzuca o niejawnej konwersji :/
        threads[i] = std::thread(
                [](Iterator begin_it, Iterator end_it, T& result){
                    result = std::accumulate(begin_it, end_it, T{});
                }, begin_it, end, std::ref(results[i])
                );
        begin_it = end;
    }
    results[used_threads - 1] = std::accumulate(begin_it, last, T{});
    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    return std::accumulate(std::begin(results), std::end(results), init);
}

int main() {
    std::vector<int> vec(1'000'000);

    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    std::generate(std::begin(vec), std::end(vec), [i{0}]() mutable { return i++; }); //std::iota też to zrobi

    auto result = p_accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Compare results from created function: " << result << std::endl;
    std::cout << " with STL " << std::accumulate(begin(vec), end(vec), 0) <<std::endl; //zagnieżdzanie namespaceow dlatego std:: nie trzeba przy begin i end

    auto start = std::chrono::high_resolution_clock::now();
    result = p_accumulate(vec.begin(), vec.end(), 0);
    auto stop = std::chrono::high_resolution_clock::now();

    auto start1 = std::chrono::high_resolution_clock::now();
    auto result1 = std::accumulate(begin(vec), end(vec), 0);
    auto stop1 = std::chrono::high_resolution_clock::now();

    auto start2 = std::chrono::high_resolution_clock::now();
    auto result2 = __gnu_parallel::accumulate(begin(vec), end(vec), 0);
    auto stop2 = std::chrono::high_resolution_clock::now();


    std::chrono::duration<float> duration = stop - start;
    std::cout << "The duration of the p_accumulate function created: \t" << duration.count() << "\n Result: " << result << '\n';

    std::chrono::duration<float> duration1 = stop1 - start1;
    std::cout << "The duration of the STL accumulate function: \t" << duration1.count() << "\n Result: " << result1 << '\n';

    std::chrono::duration<float> duration2 = stop2 - start2;
    std::cout << "The duration of the STL accumulate with execution::par function: \t" << duration2.count() << "\n Result: " << result2 << '\n';
}
