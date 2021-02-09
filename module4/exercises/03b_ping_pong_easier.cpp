#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <sstream>
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
        cout << "Ready to ping" << endl;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            // TODO: wait to be used here + printing, reps incrementation, chainging turn to pong
            auto myTurn = [&] {return isPingTurn_ ;};//&& play_;};
            opponentsTurn_.wait(l, myTurn);
            cout << "ping..." << reps << endl;
            reps++;
            isPingTurn_ = false;
            opponentsTurn_.notify_all();
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_)
        {
            // TOOD: only print message here
            unique_lock<mutex> l(m_);
            auto myTurn = [&] {return isPingTurn_ ;};//&& play_;};
            opponentsTurn_.wait(l, myTurn);
            cout << "Ping Repetitions Reached" << endl;
            isPingTurn_ = false;
            opponentsTurn_.notify_all();
        }
    }

    void pong() {
        thread_local int reps = 0;
        //cout << "Ready to pong" << endl;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            // TODO: wait to be used here + printing, reps incrementation, chainging turn to ping
            auto myTurn = [&] {return !isPingTurn_ ;};//&& play_;};
            opponentsTurn_.wait(l, myTurn);
            cout << reps << "...pong"  << endl;
            reps++;
            isPingTurn_ = true;
            this_thread::sleep_for(500ms);
            opponentsTurn_.notify_all();
        }
        if (reps >= repetitions_) {
            // TODO:  set play_ to false, display message, notify others to avoid deadlocks
            unique_lock<mutex> l(m_);
            auto myTurn = [&] {return !isPingTurn_ ;};//&& play_;};
            opponentsTurn_.wait(l, myTurn);
            opponentsTurn_.notify_all();
            play_ = false;
            cout << "Pong Repetitions Reached" << endl;
            
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        // TODO: wait_for to be used here. Check for a return value and set play_ to false
        //cout << "I am about to wait" << endl;
        auto finishedPonging = [&] {return !play_;};
        auto cvstat = opponentsTurn_.wait_for(l, timeout, finishedPonging);
        //if (cvstat == cv_status::no_timeout){
        if (cvstat == false){
            auto myTurn = [&] {return !isPingTurn_ ;};
            opponentsTurn_.wait(l, myTurn);
            play_ = false;
            cout << "Timeout reached" << endl;
            
            //opponentsTurn_.notify_all();
        }
        //cout << "Finished waiting" << endl;

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
