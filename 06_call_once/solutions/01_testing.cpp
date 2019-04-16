#include <mutex>
#include <vector>
#include <limits>
#include <functional>
#include <future>
#include <thread>
#include <iostream>
using namespace std;

class X {
    once_flag once;
    vector<double> values;
    int number;
  
public:
    void initializeFive() {
        cout << "In function " << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeFive\n";
            number = 5;
            values = {1.0, 2.0, 3.0, 4.0, 5.0};
        });
    }

    void initializeTen() {
        cout << "In function " << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeTen\n";
            number = 10;
            values = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
        });
    }
    
    void initializeEmpty() {
        cout << "In function " << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeEmpty\n";
            number = 0;
        });
    }
      
    void print() {
        cout << "number: " << number << ", values: ";
        for (const auto & value : values)
            cout << value << " ";
        cout << '\n';
    }
};

void runTest(function<void(X*)> f, X* object) try {
    cout << "===== Running a new test =====\n";
    f(object);
    cout << "--- FINISHED ---\n";
} catch (...) {
    cout << "--- EXCEPTION ---\n";
}

int main() {
    auto x = new X{};
    thread t1(runTest, &X::initializeFive, ref(x));
    thread t2(runTest, &X::initializeTen, ref(x));
    thread t3(runTest, &X::initializeEmpty, ref(x));
    t1.join();
    t2.join();
    t3.join();
    x->print();
    return 0;
}
