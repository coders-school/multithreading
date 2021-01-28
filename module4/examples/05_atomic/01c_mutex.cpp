#include <thread>
#include <mutex>
using namespace std;

int main() {
    bool flag = false;
    mutex m;
    auto stop = [&] {
        lock_guard<mutex> lg(m);
        return flag;
    };
    auto f = [&] {
        while (not stop()) {
            /* do sth... */
        }
    };
    thread t(f);
    {
        lock_guard<mutex> lg(m);
        flag = true;
    }
    t.join();
    return 0;
}

