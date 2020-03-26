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
void fill(T* array, int size, int begin, int gap, int value){
    for (int i = begin; i < size; i+=gap){
        array->a = value;
        array+=gap;
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
    fill(array, size, 0, 1, fill_it_up);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of fun -> fill(array_without_padding):          " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t([&](){fill(array, size, 0, 2, fill_it_up);});
    std::thread t1([&](){fill(array, size, 1, 2, fill_it_up);});
    t.join();
    t1.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel fun -> fill(array_without_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    fill(array64, size, 0, 1, fill_it_up);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of fun -> fill(array_with_padding):             " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t3([&](){fill(array64, size, 0, 2, fill_it_up);});
    std::thread t4([&](){fill(array64, size, 1, 2, fill_it_up);});
    t3.join();
    t4.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel fun -> fill(array_with_padding):    " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    std::cout<< "\n---------------------------------------small size: \n\n";
    
    start = std::chrono::high_resolution_clock::now();
    fill(small_array, small_size, 0, 1, fill_it_up);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of fun -> fill(small_array_without_padding):          " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t5([&](){fill(small_array, small_size, 0, 2, fill_it_up);});
    std::thread t6([&](){fill(small_array, small_size, 1, 2, fill_it_up);});
    t5.join();
    t6.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel fun -> fill(small_array_without_padding): " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    fill(small_array64, small_size, 0, 1, fill_it_up);
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of fun -> fill(small_array_with_padding):             " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::thread t7([&](){fill(small_array64, small_size, 0, 2, fill_it_up);});
    std::thread t8([&](){fill(small_array64, small_size, 1, 2, fill_it_up);});
    t7.join();
    t8.join();
    stop = std::chrono::high_resolution_clock::now();
    std::cout<< "Time of parallel fun -> fill(small_array_with_padding):    " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "\n\n";

    return 0;
}

/*
Size of array:   400000
Size of array64: 6400000

Size of small_array:   4000
Size of small_array64: 64000

---------------------------------------default size: 

Time of fun -> fill(array_without_padding):          435

Time of parallel fun -> fill(array_without_padding): 528

Time of fun -> fill(array_with_padding):             2534

Time of parallel fun -> fill(array_with_padding):    1313


---------------------------------------small size: 

Time of fun -> fill(small_array_without_padding):          12

Time of parallel fun -> fill(small_array_without_padding): 179

Time of fun -> fill(small_array_with_padding):             60

Time of parallel fun -> fill(small_array_with_padding):    89


-----------

Size of array:   400000
Size of array64: 6400000

Size of small_array:   4000
Size of small_array64: 64000

---------------------------------------default size: 

Time of fun -> fill(array_without_padding):          347

Time of parallel fun -> fill(array_without_padding): 518

Time of fun -> fill(array_with_padding):             2538

Time of parallel fun -> fill(array_with_padding):    1209


---------------------------------------small size: 

Time of fun -> fill(small_array_without_padding):          11

Time of parallel fun -> fill(small_array_without_padding): 140

Time of fun -> fill(small_array_with_padding):             36

Time of parallel fun -> fill(small_array_with_padding):    134


*/
