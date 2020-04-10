#include <iostream>
#include <thread>
#include <chrono>

alignas(64)int a = 0;
alignas(64)int b = 0;

int c = 0;
int d = 0;
 

void func1 ()
{
    for(; a < 9e8; ++a);
}

void func2()
{
    for(; b < 9e8; ++b);
}

void func3 ()
{
    for(; c < 9e8; ++c);
}

void func4()
{
    for(; d < 9e8; ++d);
}


int main()
{
    auto now = std::chrono::steady_clock::now();
    std::thread t1(func1);
    std::thread t2(func2);


    t1.join();
    t2.join();
    auto end = std::chrono::steady_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::milliseconds>(end-now).count();
    std::cout << "Result without false sharing: " << result << std::endl;




    auto now1 = std::chrono::steady_clock::now();
    std::thread t3(func3);
    std::thread t4(func4);


    t3.join();
    t4.join();
    auto end1 = std::chrono::steady_clock::now();
    auto result1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1-now1).count();
    std::cout << "Result with false sharing: " << result1 << std::endl;
}