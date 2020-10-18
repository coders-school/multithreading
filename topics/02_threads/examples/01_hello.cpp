#include <thread>
#include <iostream>
using namespace std;

void action() {
    cout << "Hello ";
    cout << this_thread::get_id();
    cout << " thread" << endl;
}

int main() {
    thread t(action);
    // can do other stuff here
    t.join();
    return 0;
}
