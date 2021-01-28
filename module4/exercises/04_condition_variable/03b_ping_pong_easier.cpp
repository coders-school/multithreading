#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <atomic>
using namespace std;

class PingPong {
    mutex m_;
    condition_variable opponentsTurn_;
    bool isPingTurn_ = true;
    int repetitions_;
    atomic<bool> play_{true};

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        thread_local int reps = 0;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            // TODO: wait to be used here + printing, reps incrementation, chainging turn to pong
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_)
        {
            // TOOD: only print message here
        }
    }

    void pong() {
        thread_local int reps = 0;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            // TODO: wait to be used here + printing, reps incrementation, chainging turn to ping
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_) {
            // TODO:  set play_ to false, display message, notify others to avoid deadlocks
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        // TODO: wait_for to be used here. Check for a return value and set play_ to false       

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
