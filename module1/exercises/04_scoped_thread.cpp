#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
private:
    thread t1;
public:
    explicit scoped_thread(std::thread && t) : t1(move(t)) {
        if (!t1.joinable()) {
            throw logic_error("cannot create empty scoped_thread");
        }
    };
    ~scoped_thread() {
        if (t1.joinable()) {
            t1.join();
        }
    }
    scoped_thread(const scoped_thread & other) = delete;
    scoped_thread& operator=(const scoped_thread & other) = delete;
    scoped_thread(scoped_thread && other) = default;
    scoped_thread& operator=(scoped_thread && other) = default;
};

void do_sth(int) {
    this_thread::sleep_for(1s);
    cout << this_thread::get_id() << '\n';
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() try {
    scoped_thread st(thread(&do_sh, 42));
    auto st3 = move(st);
    do_sth_unsafe_in_current_thread();
    return 0;
} catch (const exception & e) {
    cout << e.what() << endl;
    return -1;
} // thread is safely destroyed
