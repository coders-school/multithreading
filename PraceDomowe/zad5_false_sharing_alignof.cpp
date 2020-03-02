#include <iostream>
#include <thread>
#include <chrono>
#include <array>



int operatinos = 1'000'000'000;
const int numberOfThreads = 4;

struct alignas(64) Int64 {
    int value;
};

void thread_func(Int64* data){
    for (unsigned i = 0; i < operatinos / numberOfThreads; ++i)    {
        (*data).value ++;
    }
}

int main(){
    auto startCount = std::chrono::steady_clock::now();
    std::array <Int64, numberOfThreads> arr;
    std::array <std::thread, numberOfThreads> threads;
    for (int i = 0; i < numberOfThreads; ++i){
        threads[i] = std::thread(thread_func, &(arr[i]));
    }
    for (auto& element : threads){
        element.join();
    }
    auto stopCount = std::chrono::steady_clock::now();
    
    std::cout << "Algorithm lasted: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(stopCount - startCount).count()
            << "ms\n";
    return 0;
}

  
  
