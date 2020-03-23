#include <iostream>
#include <thread>
#include <chrono>
#include <new>
#include <vector>

struct alignas(64) Int64{
    int a;
};

struct Int{
    int a;
};

template<typename T>
void fill(T* array, int begin, int end, int value){
    for (int i = begin; i < end; i++){
        array->a = value;
        array++;
    }
}

int main(){

    int size = 100000;
    int small_size = 1000;
    int fill_it_up = 2;

    Int array[size];
    Int64 array64[size];

    Int small_array[small_size];
    Int64 small_array64[small_size];
    
    std::cout<< "\nSize of array:   " << sizeof(array) << "\n";
    std::cout<< "Size of array64: " << sizeof(array64) << "\n";
    std::cout<< "\nSize of small_array:   " << sizeof(small_array) << "\n";
    std::cout<< "Size of small_array64: " << sizeof(small_array64) << "\n";

    std::cout<< "\n---------------------------------------default size: \n\n";

    auto start = std::chrono::high_resolution_clock::now();
    fill(array, 0, size, fill_it_up);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of function -> fill(array_without_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    fill(array64, 0, size, fill_it_up);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of function -> fill(array_with_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t([&](){fill(array, 0, size-1, fill_it_up);});
    std::thread t1([&](){fill(array, size/2, size, fill_it_up);});
    t.join();
    t1.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel function -> fill(array_without_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t3([&](){fill(array64, 0, size-1, fill_it_up);});
    std::thread t4([&](){fill(array64, size/2, size, fill_it_up);});
    t3.join();
    t4.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel function -> fill(array_with_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    std::cout<< "\n---------------------------------------small size: \n\n";
    
    start = std::chrono::high_resolution_clock::now();
    fill(small_array, 0, small_size, fill_it_up);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of function -> fill(small_array_without_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    fill(small_array64, 0, small_size, fill_it_up);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of function -> fill(small_array_with_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t5([&](){fill(small_array, 0, small_size-1, fill_it_up);});
    std::thread t6([&](){fill(small_array, small_size/2, small_size, fill_it_up);});
    t5.join();
    t6.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel function -> fill(small_array_without_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t7([&](){fill(small_array64, 0, small_size-1, fill_it_up);});
    std::thread t8([&](){fill(small_array64, small_size/2, small_size, fill_it_up);});
    t7.join();
    t8.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel function -> fill(small_array_with_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    return 0;
}

/*
Size of array:   400000
Size of array64: 6400000

Size of small_array:   4000
Size of small_array64: 64000

---------------------------------------default size: 

Time of function -> fill(array_without_padding): 841

Time of function -> fill(array_with_padding): 4482

Time of parallel function -> fill(array_without_padding): 526

Time of parallel function -> fill(array_with_padding): 1598


---------------------------------------small size: 

Time of function -> fill(small_array_without_padding): 14

Time of function -> fill(small_array_with_padding): 25

Time of parallel function -> fill(small_array_without_padding): 114

Time of parallel function -> fill(small_array_with_padding): 81


-----------


Size of array:   400000
Size of array64: 6400000

Size of small_array:   4000
Size of small_array64: 64000

---------------------------------------default size: 

Time of function -> fill(array_without_padding): 475

Time of function -> fill(array_with_padding): 2418

Time of parallel function -> fill(array_without_padding): 476

Time of parallel function -> fill(array_with_padding): 1429


---------------------------------------small size: 

Time of function -> fill(small_array_without_padding): 14

Time of function -> fill(small_array_with_padding): 32

Time of parallel function -> fill(small_array_without_padding): 104

Time of parallel function -> fill(small_array_with_padding): 112

*/
