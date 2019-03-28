#include <thread>
#include <iostream>
using namespace std;

int main()
{
    thread t([]
    {
        cout << "Hello ";
        cout << this_thread::get_id();
        cout << " thread" << endl;
    });
    // can do other stuff here
    t.join();
    return 0;
}

