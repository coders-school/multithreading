#include <future>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;

int main()
{
    auto f1 = async([] {
        cout << "f1 started\n";
        this_thread::sleep_for(1s);
        return 42;
    });
    cout << "f1 spawned\n";
    
    auto f2 = async(launch::async, []{
        cout << "f2 started\n";
        this_thread::sleep_for(1s);
        return 2 * 42;
    });
    cout << "f2 spawned\n";
    
    auto f3 = async(launch::deferred, []{
        cout << "f3 started\n";
        this_thread::sleep_for(1s);
        return 3 * 42;
    });
    cout << "f3 spawned\n";

    cout << "Getting f1 result\n";
    auto v1 = f1.get();
    cout << "Got f1 result\n";
    
    cout << "Getting f2 result\n";
    auto v2 = f2.get();
    cout << "Got f2 result\n";
    
    cout << "Getting f3 result\n";
    auto v3 = f3.get();
    cout << "Got f3 result\n";
    
    vector<int> numbers = { v1, v2, v3 };
    for (const auto & item : numbers)
        cout << item << '\n';
    
    return 0;
}
