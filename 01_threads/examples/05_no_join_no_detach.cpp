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
    // no join() or detach()
    casualJob();
    return 0;
}

