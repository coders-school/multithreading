#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;

class PingPong {
    int repetitions_;
    mutable mutex coutMutex_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}
    
    void safeCout(stringstream& ss) const {
        lock_guard<mutex> lockCout(coutMutex_);
        cout << ss.rdbuf();
    }
    
    void safePongExit(stringstream& ss) {
        lock_guard<mutex> lockCout(coutMutex_);
        ss << "Repetitions end!\n";
        cout << ss.rdbuf();
        exit(0);
    }
    
    void safeStopExit() const {
        lock_guard<mutex> lockCout(coutMutex_);
        stringstream ss("Timeout!\n");
        cout << ss.rdbuf();
        exit(0);
    }

    void ping() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            ss << "ping" << i << ' ';
            safeCout(ss);
        }
    }

    void pong() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            ss << "pong" << i << '\n';
            safeCout(ss);
        }
        
        safePongExit(ss);
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        auto startCountTime = chrono::steady_clock::now();
        
        for(;;) {
            auto currentTime = chrono::steady_clock::now();
            
            if(chrono::duration_cast<chrono::seconds>(currentTime - startCountTime)
                >= timeout) {
                    safeStopExit();
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
