#include <iostream>
#include <thread>

void do_something(int data) {
    std::cout << "Data: " << data << "\n" << std::flush;
}

struct Foo {
public:
    Foo(int& i): i_(i) {}
    void operator()(){
        for (int k = 0 ; k < 10 ; ++k) {
            do_something(++i_);
        }
    }
private:
    int& i_;
};

void bar() {throw std::runtime_error("Error"); }

void oops() {
    int some_local_state = 0;
    Foo foo(some_local_state);
    std::thread t(foo);
    try {
        bar();
    } catch (...) {
        std::cout << "ERROR" << std::endl;
        t.join();
        throw;
    }
    t.join();
}

int main()
{
    oops();
    return 0;
}