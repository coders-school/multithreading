#include "ThreadPool.hpp"
#include "Task.hpp"
#include <algorithm>
#include <numeric>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

std::mutex coutMtx;

using namespace std::literals::chrono_literals;
using Lock = std::lock_guard<std::mutex>;

void printFuture(std::future<Task::PromiseType> && f)
{
    const auto & [id, collection] = f.get();
    Lock l(coutMtx);
    std::cout << id << ": ";
    for (const auto & item : collection)
        std::cout << item << ' ';
    std::cout << '\n';
}

int main()
{
    auto threads_number = std::thread::hardware_concurrency();
    std::cout << "Hardware Threads: " << threads_number << '\n';
    ThreadPool threadPool{threads_number};
    Task copyTask = {
        "copy",
        [](const Task::Numbers &lhs, Task::Numbers &rhs) {
            std::copy(begin(lhs), end(lhs), back_inserter(rhs));
        },
        {1, 2, 3, 9, 8, 7},       
    };
    Task partialSumTask = {
        "partialSum",
        [](const Task::Numbers &lhs, Task::Numbers &rhs) {
            std::partial_sum(begin(lhs), end(lhs), back_inserter(rhs));
        },
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    };
    Task adjacentDifferenceTask = {
        "adjacentDifference",
        [](const Task::Numbers &lhs, Task::Numbers &rhs) {
            std::adjacent_difference(begin(lhs), end(lhs), back_inserter(rhs));
        },
        {1, 2, 4, 8, 16, 32, 32, 555, 222, 1}
    };
    Task dummyTask = {
        "dummy",
        []([[maybe_unused]] const Task::Numbers &lhs, [[maybe_unused]] Task::Numbers &rhs) {
            std::this_thread::sleep_for(1s); // simulate long task
        }
    };


    auto copyFuture = threadPool.enqueue(std::move(copyTask));
    auto partialSumFuture = threadPool.enqueue(std::move(partialSumTask));
    auto dummyFuture = threadPool.enqueue(std::move(dummyTask));
    auto adjacentDifferenceFuture = threadPool.enqueue(std::move(adjacentDifferenceTask));
    
    std::vector<std::future<Task::PromiseType>> numbers;
    for (int i = 0; i < 40; i++)
        numbers.emplace_back(threadPool.enqueue(Task{
            "number",
            [i]([[maybe_unused]] const Task::Numbers &lhs, [[maybe_unused]] Task::Numbers &rhs) {
                {
                    Lock l(coutMtx);
                    std::cout << i << ' ' << std::this_thread::get_id() << '\n';
                }
                rhs.emplace_back(i);;
            }
        }));


    printFuture(std::move(copyFuture));
    printFuture(std::move(partialSumFuture));
    printFuture(std::move(dummyFuture));
    printFuture(std::move(adjacentDifferenceFuture));
    for (auto && fut : numbers)
        printFuture(std::move(fut));

    // threadPool.showCompleted();
    return 0;
}