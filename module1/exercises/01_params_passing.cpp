#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    std::thread t(add, 3, 4);
    t.join();
    // run add function in a thread
    // pass 3 and 4 as arguments
    return 0;
}
