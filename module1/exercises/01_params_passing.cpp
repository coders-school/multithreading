#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    int a = 3, b = 4;
    thread t(add, a, b);
    t.join();
    return 0;
}

