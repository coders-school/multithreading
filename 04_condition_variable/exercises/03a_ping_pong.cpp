#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;

class PingPong {
    int repetitions_;
    int repetitionsCounter_;
    mutex coutMutex_;
    condition_variable pingOrPong_;
    bool pingTurn_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions), repetitionsCounter_(0), pingTurn_(true)
    {}

    void safePongExit(stringstream& ss) {
        lock_guard<mutex> lockCout(coutMutex_);
        ss << "Repetitions end!\n";
        cout << ss.rdbuf();
        exit(0);
    }
    
    void safeStopExit() {
        lock_guard<mutex> lockCout(coutMutex_);
        stringstream ss;
        
        if(pingTurn_ == false) {
            ss << "pong" << repetitionsCounter_ - 1 << '\n';
            cout << ss.rdbuf();
        }

        ss << "Timeout!\n";
        cout << ss.rdbuf();
        exit(0);
    }

    void ping() {
        stringstream ss;

        for(; repetitionsCounter_ < repetitions_; ++repetitionsCounter_) {
            unique_lock<mutex> lockCout(coutMutex_);
            pingOrPong_.wait(lockCout, [&]{return pingTurn_;});
            
            ss << "ping" << repetitionsCounter_ << ' ';
            cout << ss.rdbuf();
            
            pingTurn_ = false;
            pingOrPong_.notify_all();
        }
    }

    void pong() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            unique_lock<mutex> lockCout(coutMutex_);
            pingOrPong_.wait(lockCout, [&]{return !pingTurn_;});
            
            ss << "pong" << i << '\n';
            cout << ss.rdbuf();
            
            pingTurn_ = true;
            pingOrPong_.notify_all();
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
