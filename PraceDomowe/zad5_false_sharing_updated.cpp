#include <iostream>
#include <thread>
#include <chrono>
#include <array>


int operatinos = 1'000'000'000;
const int numberOfThreads = 4;
const int spacing = 16; 
const int padding = 4;

void thread_func(int* data){
    for (unsigned i = 0; i < operatinos / numberOfThreads; ++i)    {
        (*data)++;
    }
}

int main(){
    auto startCount = std::chrono::steady_clock::now();
    const int arraySize = numberOfThreads*spacing + padding;
    std::array <int, arraySize> arr;
    std::array <std::thread, numberOfThreads> threads;
    for (int i = 0; i < numberOfThreads; ++i){
        threads[i] = std::thread(thread_func, &(arr[i*spacing + padding]));
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

  
  
