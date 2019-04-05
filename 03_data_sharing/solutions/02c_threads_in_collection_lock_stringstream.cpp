#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
using namespace std;

void do_work(int id, mutex & m) {
    this_thread::sleep_for(100ms);
    stringstream ss;
    ss << "Thread [" << id << "]: " << "Job done!" << endl;
    lock_guard<mutex> lock(m);
    cout << ss.rdbuf();
}
 
int main() {
    mutex m;
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i, ref(m)));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
