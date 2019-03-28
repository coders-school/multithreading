#include <thread>
#include <iostream>
#include <functional>

void abc(int &a) { a = 2; }
void def(int &a) { a = 3; }

int main()
{
    int x = 1;
    std::thread t1(abc, std::ref(x));
    std::thread t2(def, std::ref(x));

    t1.join();
    t2.join();

    std::cout << x << std::endl; // what will be displayed?
}
