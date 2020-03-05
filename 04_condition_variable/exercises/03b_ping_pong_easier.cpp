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
            opponentsTurn_.wait(l, [&](){
                return isPingTurn_;
            });
            std::cout << "ping " <<reps+1 <<std::endl;
            reps++;
            isPingTurn_ = false;
            l.unlock();
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_)
        {
            // TOOD: only print message here
            std::cout << "Ping repetition limit." << std::endl;
        }
    }

    void pong() {
        thread_local int reps = 0;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            // TODO: wait to be used here + printing, reps incrementation, chainging turn to ping
            opponentsTurn_.wait(l, [&](){
                return not isPingTurn_;
            });
            std::cout << "pong " <<reps+1 << std::endl;
            reps++;
            isPingTurn_ = true;
            l.unlock();
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_) {
            // TODO:  set play_ to false, display message, notify others to avoid deadlocks
            play_ = false;
            std::cout << "Pong repetition limit. \nEnd of program" << std::endl;
            opponentsTurn_.notify_all();
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        // TODO: wait_for to be used here. Check for a return value and set play_ to false       
       // auto timeStop = opponentsTurn_.wait_for(l, timeout,[&](){return not play_;});
        if( not opponentsTurn_.wait_for(l, timeout,[&](){return not play_;})){
            play_ = false;
            std::cout << "End od program because of TIMEOUT." << std::endl;
        }
        opponentsTurn_.notify_all();

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
