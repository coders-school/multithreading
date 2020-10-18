#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        this_thread::sleep_for(1s);
        cout << "Thread job done" << endl;
    });
    t.detach();
    casualJob();
    return 0;
}

