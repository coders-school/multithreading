#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;

enum PingPongStatus {
    PING, PONG, TIMEOUT, REPETITIONS_END
};

class PingPong {
    int repetitions_;
    mutex coutMutex_;
    condition_variable pingOrPong_;
    PingPongStatus whoseTurn_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions), whoseTurn_(PING)
    {}

    void safePongExit(stringstream& ss) {
        lock_guard<mutex> lockCout(coutMutex_);
        ss << "Repetitions end!\n";
        cout << ss.rdbuf();
        
        whoseTurn_ = REPETITIONS_END;
        pingOrPong_.notify_all();
    }
    
    void safeStopExit() {
        unique_lock<mutex> lockCout(coutMutex_);
        pingOrPong_.wait(lockCout, [&]{return whoseTurn_ == PING;});
        stringstream ss("Timeout!\n");
        cout << ss.rdbuf();
        
        whoseTurn_ = TIMEOUT;
        pingOrPong_.notify_all();
    }

    void ping() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            unique_lock<mutex> lockCout(coutMutex_);
            pingOrPong_.wait(lockCout, [&]{return (whoseTurn_ == PING
                                                    || whoseTurn_ == TIMEOUT);});
            if(whoseTurn_ == TIMEOUT)
                return;
            
            ss << "ping" << i << ' ';
            cout << ss.rdbuf();
            
            whoseTurn_ = PONG;
            pingOrPong_.notify_all();
        }
    }

    void pong() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            unique_lock<mutex> lockCout(coutMutex_);
            pingOrPong_.wait(lockCout, [&]{return (whoseTurn_ == PONG
                                                    || whoseTurn_ == TIMEOUT);});
            if(whoseTurn_ == TIMEOUT)
                return;
            
            ss << "pong" << i << '\n';
            cout << ss.rdbuf();
            
            whoseTurn_ = PING;
            pingOrPong_.notify_all();
        }
        
        safePongExit(ss);
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        auto startCountTime = chrono::steady_clock::now();
        
        for(;;) {
            if(whoseTurn_ == REPETITIONS_END)
                return;
            
            auto currentTime = chrono::steady_clock::now();
            
            if(chrono::duration_cast<chrono::seconds>(currentTime - startCountTime)
                >= timeout) {
                safeStopExit();
                return;
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
