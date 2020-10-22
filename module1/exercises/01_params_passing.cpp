#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    thread t1(add, 3, 4);
    // auto t = thread(add, 3, 4);
    t1.join();
    return 0;
}
