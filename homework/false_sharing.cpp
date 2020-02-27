// NOTE: This code is inspired from following articles:
// - https://en.wikipedia.org/wiki/False_sharing
// - https://parallelcomputing2017.wordpress.com/2017/03/17/understanding-false-sharing

#include <iostream>
#include <thread>

int array[100];
constexpr int firstElememtIdx     = 0;
constexpr int badElementIdx       = 1;
constexpr int goodElementIdx      = 99;
constexpr int loopCnt             = 100000000;

void expensiveFunction(int* table, int idx, int loopCnt)
{
    for ( int i = 0; i < loopCnt; ++i)
    {
        table[idx] += 1;
    }
}

void serialComputation()
{
    expensiveFunction(array, firstElememtIdx, loopCnt);
    expensiveFunction(array, badElementIdx, loopCnt);
}

void parallelComputationWithFalseSharing()
{
    std::thread t1(expensiveFunction, array, firstElememtIdx, loopCnt);
    std::thread t2(expensiveFunction, array, badElementIdx, loopCnt);
    t1.join();
    t2.join();
}

void parallelComputationWithoutFalseSharing()
{
    std::thread t1(expensiveFunction, array, firstElememtIdx, loopCnt);
    std::thread t2(expensiveFunction, array, goodElementIdx, loopCnt);
    t1.join();
    t2.join();
}


int main()
{
    std::cout << "False sharing test." << std::endl << std::endl;

    // Serial computation
    auto start = std::chrono::steady_clock::now();
    serialComputation();
    auto stop = std::chrono::steady_clock::now();

    std::cout << "Time taken in Sequential computing: ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms";
    std::cout << std::endl;

    // Computation with false sharing
    start = std::chrono::steady_clock::now();
    parallelComputationWithFalseSharing();
    stop = std::chrono::steady_clock::now();

    std::cout << "Time taken with false sharing:      ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms";
    std::cout << std::endl;

    // Computation without false sharing
    start = std::chrono::steady_clock::now();
    parallelComputationWithoutFalseSharing();
    stop = std::chrono::steady_clock::now();

    std::cout << "Time taken without false sharing:   ";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms";
    std::cout << std::endl;

    return 0;
}