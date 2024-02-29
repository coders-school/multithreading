#include <thread>
#include <iostream>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    std::thread t1{add, 3, 4};
    
    t1.join();

    return 0;
}
