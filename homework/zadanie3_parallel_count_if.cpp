#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <chrono>
#include <functional>
#include <cassert>
#include <execution>

template <typename IT, typename Pre>
int parallelCountIf(IT first, IT last, Pre predicat) {
    const size_t size = std::distance(first, last);

    if (!size)
        return 0;

    const size_t hardware_threads = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 2;
    const size_t chunkSize = size / hardware_threads;
    std::vector<std::thread> threads(hardware_threads - 1);
    std::vector<int> results(hardware_threads);

    auto begin = first;

    for (size_t i = 0; i < hardware_threads - 1; ++i) {
        auto end = std::next(begin, chunkSize);
        threads[i] = std::thread([](IT first, IT last, Pre predicat, int& result)
            {
                result = std::count_if(first, last, predicat);
            }, begin, end, predicat, std::ref(results[i]));
        begin = end;
    }

    results[hardware_threads - 1] = std::count_if(begin, last, predicat);

    for (auto && t : threads) {
        t.join();
    }

    return std::accumulate(std::begin(results), std::end(results), 0);
}


int main() {

    std::vector<int> vec{3,4,5,2,10,44,65,34,1,0,15,20,40,60,33,3,8,9,0,1,111,10};
    std::vector<int> vec1m(1'000'000);
    size_t n =0;
    std::generate(vec1m.begin(), vec1m.end(), [&n](){return n+=5;});
    
    auto predicat = [](int& x){return x%5 == 0 and x != 0; };

    std::cout<< "Algorithms are working on small amount of elements in vector. \n\n";

    std::cout<< "\nStart of pararel count_if. \n";
    auto start = std::chrono::high_resolution_clock::now();
    auto dividedBy5Parallel = parallelCountIf(std::begin(vec), std::end(vec), predicat);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<< "End of parallel count_if. \n";
    std::cout<< "Time of parallel count_if function: " 
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n"; 
    std::cout<< "Results of parallel: " << dividedBy5Parallel << "\n\n";

    std::cout<< "Start of std::count_if. \n";
    start = std::chrono::high_resolution_clock::now();
    auto dividedBy5 = std::count_if(std::begin(vec), std::end(vec), predicat);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "End of std::count_if. \n";
    std::cout<< "Time of std::count_if function: "
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";
    std::cout<< "Results of non parallel: " << dividedBy5 << "\n\n";

    std::cout<< "Start of std::count_if with std::execution::par. \n";
    start = std::chrono::high_resolution_clock::now();
    auto dividedBy5_exec = std::count_if(std::execution::par, std::begin(vec), std::end(vec), predicat);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "End of std::count_if with std::execution::par. \n";
    std::cout<< "Time of std::count_if function with std::execution::par: "
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";
    std::cout<< "Results of non parallel with std::execution: " << dividedBy5_exec << "\n\n";

    assert(dividedBy5Parallel == dividedBy5);
    assert(dividedBy5Parallel == dividedBy5_exec);

    std::cout<< "Algorithms are working on big amount of elements in vector. \n\n";

    std::cout<< "\nStart of pararel count_if. \n";
    start = std::chrono::high_resolution_clock::now();
    dividedBy5Parallel = parallelCountIf(std::begin(vec1m), std::end(vec1m), predicat);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "End of parallel count_if. \n";
    std::cout<< "Time of parallel count_if function: "
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";
    std::cout<< "Results of parallel: " << dividedBy5Parallel << "\n\n";

    std::cout<< "Start of std::count_if with std::execution::par. \n";
    start = std::chrono::high_resolution_clock::now();
    dividedBy5_exec = std::count_if(std::execution::par, std::begin(vec1m), std::end(vec1m), predicat);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "End of std::count_if with std::execution::par. \n";
    std::cout<< "Time of std::count_if function with std::execution: "
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";
    std::cout<< "Results of non parallel: " << dividedBy5_exec << "\n\n";

    std::cout<< "Start of std::count_if. \n";
    start = std::chrono::high_resolution_clock::now();
    dividedBy5 = std::count_if(std::begin(vec1m), std::end(vec1m), predicat);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "End of std::count_if. \n";
    std::cout<< "Time of std::count_if function: "
             << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";
    std::cout<< "Results of non parallel: " << dividedBy5 << "\n\n";


    assert(dividedBy5Parallel == dividedBy5);
    assert(dividedBy5Parallel == dividedBy5_exec);

    return 0;
}

/*
Algorithms are working on small amount of elements in vector. 


Start of pararel count_if. 
End of parallel count_if. 
Time of parallel count_if function: 516

Results of parallel: 8

Start of std::count_if. 
End of std::count_if. 
Time of std::count_if function: 1

Results of non parallel: 8

Start of std::count_if with std::execution::par. 
End of std::count_if with std::execution::par. 
Time of std::count_if function with std::execution::par: 840

Results of non parallel with std::execution: 8

Algorithms are working on big amount of elements in vector. 


Start of pararel count_if. 
End of parallel count_if. 
Time of parallel count_if function: 11984

Results of parallel: 1000000

Start of std::count_if with std::execution::par. 
End of std::count_if with std::execution::par. 
Time of std::count_if function with std::execution: 9668

Results of non parallel: 1000000

Start of std::count_if. 
End of std::count_if. 
Time of std::count_if function: 16252

Results of non parallel: 1000000

*/
