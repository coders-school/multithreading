#include <thread>
#include <iostream>
using namespace std;

void add10(int & a)
{
    a += 10;
}

int main()
{
    int value = 5;
    // add10(value);
    std::thread t(add10, std::ref(value));
    t.join();
    std::cout << value << '\n';
    // run add10 function in a thread
    // pass 5 as an argument and read it's value
    return 0;
}
