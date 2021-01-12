#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
public:
    // TODO: Your implementation goes here
};

void do_sth(int) {
    this_thread::sleep_for(1s);
    cout << this_thread::get_id() << '\n';
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() try {
    // TODO: Uncomment
    // scoped_thread st(std::thread(do_sth, 42));
    // // auto st2 = st; // copying not allowed
    // [[maybe_unused]] auto st3 = move(st);
    // scoped_thread st4(do_sth, 42);
    do_sth_unsafe_in_current_thread();
    return 0;
} catch (const exception & e) {
    cout << e.what() << endl;
    return -1;
} // thread is safely destroyed
