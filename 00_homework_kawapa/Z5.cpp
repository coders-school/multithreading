#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

const int CORES = 4;
const int HOW_MANY = 10'000'000;

struct Entity
{
    int value;
    // int padding[1];
    // if padding == 15, the entity object has exactly 64 bytes (one cache line)
    // if 15 > padding >= 5, it still helps as at least one thread
    // ... can work indepedently
};

void hardWork(std::array<Entity, CORES> & entities, int i)
{    
    for (size_t j = 0; j < HOW_MANY; j++)
    {
        entities[i].value = entities[i].value + 2;
        // function hardWork overwrites one cell "HOW_MANY" times
        // program execution time will tell if padding helps
    }
}

int main()
{   
    std::array<Entity, CORES> entities;
    std::array<std::thread, CORES - 1> threads;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < CORES - 1; i++)
            threads[i]  = std::thread(hardWork, std::ref(entities),  i); 

        hardWork(std::ref(entities), CORES - 1);

        for (auto &&i : threads)
            i.join();

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> difference = end - start;
        std::cout << "This took: " << difference.count() << std::endl;

    return 0;
}