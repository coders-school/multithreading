#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

// TODO: Get rid of possible deadlock

class X {
    mutable mutex mtx_;
    int value_ = 0;

public:
    explicit X(int v) : value_(v) {}

    bool operator<(const X & other) const {
        lock_guard<mutex> ownGuard(mtx_);
        lock_guard<mutex> otherGuard(other.mtx_);
        return value_ < other.value_;
    }
};

int main() {
    X x1(5);
    X x2(6);
    thread t1([&] {
        if (x1 < x2)
            cout << "x1 is less" << endl;
    });
    thread t2([&] {
        if (x2 < x1)
            cout << "x2 is less" << endl;
    });
    t1.join();
    t2.join();

    return 0;
}
