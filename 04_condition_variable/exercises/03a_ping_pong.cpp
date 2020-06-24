#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;

class PingPong {
    int repetitions_;
    mutex coutMutex_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            ss << "ping" << i << ' ';
            lock_guard<mutex> lockCout(coutMutex_);
            cout << ss.rdbuf();
        }
    }

    void pong() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            ss << "pong" << i << '\n';
            lock_guard<mutex> lockCout(coutMutex_);
            cout << ss.rdbuf();
        }
        
        lock_guard<mutex> lockCout(coutMutex_);
        ss << "Repetitions end!\n";
        cout << ss.rdbuf();
        exit(0);
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        auto startCountTime = chrono::steady_clock::now();
        
        for(;;) {
            auto currentTime = chrono::steady_clock::now();
            
            if(chrono::duration_cast<chrono::seconds>(currentTime - startCountTime)
                >= timeout) {
                lock_guard<mutex> lockCout(coutMutex_);
                stringstream ss("Timeout!\n");
                cout << ss.rdbuf();
                exit(0);
            }
        }
    }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        return -1;
    }
    int repetitions = stoi(argv[1]);
    int timeout = stoi(argv[2]);
    PingPong pp(repetitions);
    thread t1(&PingPong::ping, &pp);
    thread t2(&PingPong::pong, &pp);
    thread t3(&PingPong::stop, &pp, chrono::seconds(timeout));
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
