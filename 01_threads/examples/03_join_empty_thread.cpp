#include <thread>
#include <iostream>
using namespace std;

int main() {
    thread t;
    t.join();   // not allowed on an empty thread
    t.detach(); // not allowed on an empty thread
    return 0;
}

