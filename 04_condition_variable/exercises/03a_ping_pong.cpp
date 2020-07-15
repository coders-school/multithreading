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
    
    bool timeoutForPingPong() const {return whoseTurn_ == TIMEOUT;}
    
    bool timeoutForStop(chrono::time_point<chrono::steady_clock>& current,
                        chrono::time_point<chrono::steady_clock>& start,
                        chrono::seconds& timeout) const {
        return chrono::duration_cast<chrono::seconds>(current - start) >= timeout;
    }
    
    bool repetitionsEnd() const {return whoseTurn_ == REPETITIONS_END;}
    
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
    
    void safePrintPing(stringstream& ss, int& i) {
        unique_lock<mutex> lockCout(coutMutex_);
        pingOrPong_.wait(lockCout, [&]{return (whoseTurn_ == PING
                                                || whoseTurn_ == TIMEOUT);});
        if(timeoutForPingPong())
            return;
        
        ss << "ping" << i << ' ';
        cout << ss.rdbuf();
        
        whoseTurn_ = PONG;
        pingOrPong_.notify_all();
    }
    
    void safePrintPong(stringstream& ss, int& i) {
        unique_lock<mutex> lockCout(coutMutex_);
        pingOrPong_.wait(lockCout, [&]{return (whoseTurn_ == PONG
                                                || whoseTurn_ == TIMEOUT);});
        if(timeoutForPingPong())
            return;
        
        ss << "pong" << i << '\n';
        cout << ss.rdbuf();
        
        whoseTurn_ = PING;
        pingOrPong_.notify_all();
    }
    
    void safeCountTime(chrono::time_point<chrono::steady_clock>& startCountTime,
                       chrono::seconds& timeout) {
        unique_lock<mutex> lock_cout(coutMutex_);
        pingOrPong_.wait(lock_cout, [&]{
            auto currentTime = chrono::steady_clock::now();
            return chrono::duration_cast<chrono::seconds>(currentTime - startCountTime) >= timeout || whoseTurn_ == REPETITIONS_END;
            });
    }

public:
    PingPong(int repetitions)
        : repetitions_(repetitions), whoseTurn_(PING)
    {}

    void ping() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i)
            safePrintPing(ss, i);
    }

    void pong() {
        stringstream ss;

        for(int i = 0; i < repetitions_; ++i)
            safePrintPong(ss, i);
        
        if(timeoutForPingPong())
            return;
        else safePongExit(ss);
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        auto startCountTime = chrono::steady_clock::now();
        
        safeCountTime(startCountTime, timeout);

        if(repetitionsEnd())
            return;
        else safeStopExit();
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
