#include <iostream>
#include <future>
using namespace std;

void f() {
  this_thread::sleep_for(1s);
}

int main() {
    auto fut = async(f);

    while (fut.wait_for(100ms) != future_status::ready) {   
    	// loop until f has finished running... which may never happen!
    	// TODO: Fix to always finish without specifying policy, which is obvious :)
    	// Hint: check future_status
        cout << "Waiting...\n";
    }
    cout << "Finally...\n";
}
