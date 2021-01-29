#include <thread>
using namespace std;

int main() {
    volatile bool stop = false;

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

