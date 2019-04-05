#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

class X {
    mutable mutex mtx_;
    int value_ = 0;

public:
    explicit X(int v) : value_(v) {}

    bool operator<(const X & other) const {
        lock(mtx_, other.mtx_);
        unique_lock<mutex> ownLock(mtx_, adopt_lock);
        unique_lock<mutex> otherLock(other.mtx_, adopt_lock);
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
