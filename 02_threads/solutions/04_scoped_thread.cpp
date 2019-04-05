#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    std::thread t_;
public:
    explicit scoped_thread(std::thread t) 
        : t_(std::move(t))
    {
        if (not t_.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread() {
        if (t_.joinable()) {
            t_.join();
        }   
    }
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread(scoped_thread &&) = default;
    scoped_thread& operator=(const scoped_thread &) = delete;
    scoped_thread& operator=(scoped_thread &&) = default;
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

