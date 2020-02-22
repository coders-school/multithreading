#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

template<typename It, typename T>
struct accumulator {
    void operator()(It first, It last, T &result) {
        result = std::accumulate(first, last, result);
    }
};

void set_th_parameters(long length, unsigned int &th_quantity, long &block_size) {
    auto hw_threads = std::thread::hardware_concurrency();
    std::cout << "Threads used " << hw_threads << std::endl;
    th_quantity = hw_threads != 0 ? hw_threads : 2;
    block_size = length / th_quantity;
}

template<typename It, typename T>
T th_accumulate(It first, It last, T init) {
    auto length = std::distance(first, last);
    if (!length) return init;

    unsigned int th_quantity;
    long block_size;
    It block_start = first;

    set_th_parameters(length, th_quantity, block_size);

    std::vector<T> results(th_quantity);
    std::vector<std::thread> threads(th_quantity - 1);

    for (unsigned long i = 0; i < (th_quantity - 1); ++i) {
        It block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulator<It, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    accumulator<It, T>()(block_start, last, results[th_quantity - 1]);

    for (auto &&th : threads) th.join();

    return std::accumulate(results.begin(), results.end(), init);
}

void benchmark_and_report(std::vector<int> v) {
    auto start = std::chrono::high_resolution_clock::now();
    auto acc = th_accumulate(v.begin(), v.end(), 0);
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> delta = stop - start;

    start = std::chrono::high_resolution_clock::now();
    auto reference_acc = std::accumulate(v.begin(), v.end(), 0);
    stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> reference_delta = stop - start;

    std::cout << "Time " << delta.count() << " vs " << reference_delta.count() << " for STL" << std::endl;
    std::cout << "Acc  " << acc << std::endl;
    if (acc == reference_acc) {
        std::cout << "Result equal to reference" << std::endl;
    } else {
        std::cout << "Calculation error: result equal to reference" << std::endl;
    }

}

int main() {
    std::vector<int> v(1'000'000);
    std::iota(v.begin(), v.end(), 1);

    benchmark_and_report(std::move(v));

    return 0;
}


