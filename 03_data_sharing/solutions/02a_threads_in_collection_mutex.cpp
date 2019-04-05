#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
using namespace std;

mutex m;

void do_work(int id) {
    this_thread::sleep_for(100ms);
    m.lock();
    cout << "Thread [" << id << "]: " << "Job done!" << endl;
    m.unlock();
}
 
int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
