#include <thread>
#include <iostream>
using namespace std;

void add10(int & a)
{
    a += 10;
    cout << "a in thread: " << a << endl;
}

int main() {
    int a = 5;
    thread t(add10, ref(a));
    t.join();
    cout << "a in main: " << a << endl;
    return 0;
}
