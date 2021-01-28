#include <thread>
#include <atomic>
using namespace std;

int main() {
    atomic<bool> stop{false};

    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}

