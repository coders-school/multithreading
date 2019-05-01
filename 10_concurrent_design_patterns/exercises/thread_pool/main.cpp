#include "ThreadPool.hpp"
#include "Task.hpp"
#include <algorithm>
#include <numeric>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

int main()
{
    auto threads_number = std::thread::hardware_concurrency();
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

    threadPool.enqueue(copyTask);
    threadPool.enqueue(partialSumTask);
    threadPool.enqueue(dummyTask);
    threadPool.enqueue(dummyTask);
    threadPool.enqueue(adjacentDifferenceTask);

    std::this_thread::sleep_for(2s);
    threadPool.showCompleted();
    return 0;
}