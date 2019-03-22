#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    this_thread::sleep_for(1s);
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    return 0;
}

