#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    std::thread t_;
public:
    scoped_thread(std::thread new_thread) : t_{std::move(new_thread)} {
        if (not t_.joinable()) {
            throw std::logic_error("No thread");
        }
    }

    template <typename ...Args>
    scoped_thread(Args&&... args) : t_{std::forward<Args>(args)...} {}

    scoped_thread(scoped_thread&& other) {
        if (t_.joinable()) {
            t_.join();
            t_ = std::move(other.t_);
        }
    }
    scoped_thread& operator=(scoped_thread&& other) {
        if (t_.joinable()) {
            t_.join();
            t_ = std::move(other.t_);
        }
        return *this;
    }
    scoped_thread(const scoped_thread&) = delete;
    scoped_thread& operator=(const scoped_thread&) = delete;

    ~scoped_thread() {
        if (t_.joinable()) {
            t_.join();
        }
    }
};

void do_sth(int) {
    this_thread::sleep_for(1s);
    cout << this_thread::get_id() << '\n';
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    // TODO: Uncomment
    scoped_thread st(std::thread(do_sth, 42));
    scoped_thread(std::thread{});
    // auto st2 = st; // copying not allowed
    [[maybe_unused]] auto st3 = move(st);
    scoped_thread st4(do_sth, 42);
    scoped_thread st5([]{
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Hello there5\n" << std::this_thread::get_id();
    });
    scoped_thread st6([]{
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Hello here6\n" << std::this_thread::get_id();
    });
    st5 = std::move(st6);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed
