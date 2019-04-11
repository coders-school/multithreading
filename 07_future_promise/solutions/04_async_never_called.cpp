#include <iostream>
#include <future>
using namespace std;

void f() {
  this_thread::sleep_for(1s);
}

int main() {
    auto fut = async(f);

    if (fut.wait_for(0s) == future_status::deferred) {
        cout << "Scheduled as deffered. Calling wait() to enforce execution\n";
        fut.wait();
    } else {
        while (fut.wait_for(100ms) != future_status::ready) {   
            cout << "Waiting...\n";
        }
        cout << "Finally...\n";
    }
}
