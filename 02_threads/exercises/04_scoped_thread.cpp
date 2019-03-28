#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    // your implementation goes here
public:
    scoped_thread(thread) {}
};

void do_sth(int) {
    this_thread::sleep_for(1s);
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    auto st3 = move(st);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed

