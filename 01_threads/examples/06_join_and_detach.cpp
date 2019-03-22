#include <thread>
#include <iostream>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    t.detach();
    return 0;
}

