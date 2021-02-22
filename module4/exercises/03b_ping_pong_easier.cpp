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
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            opponentsTurn_.wait(l, [&] {return isPingTurn_; });
            cout << "Ping " << reps << endl;
            reps++;
            isPingTurn_ = false;
            opponentsTurn_.notify_all();
        }
        if (reps >= repetitions_)
        {
            std::stringstream notify;
            notify << "Ping is finishing game. Num reps has reached " << reps << endl;
            cout << notify.str();
        }
    }

    void pong() {
        thread_local int reps = 0;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            opponentsTurn_.wait(l,  [&] {return !isPingTurn_; });
            cout << "Pong " << reps  << endl;
            reps++;
            isPingTurn_ = true;
            if (reps >= repetitions_) {
                play_ = false;
            }
            opponentsTurn_.notify_all(); 
        }
        if (reps >= repetitions_) {
            std::stringstream notify;
            notify << "Pong is finishing game. Num reps has reached " << reps << endl;
            cout << notify.str();
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        // TODO: wait_for to be used here. Check for a return value and set play_ to false
        auto ret = opponentsTurn_.wait_for(l, timeout, [&] {return not play_.load(); });
        if (ret) {
            cout << "Game finished.\n";
        }
        else {
            cout << "Stop is finishing game - timeout.\n";
            play_ = false;
            opponentsTurn_.notify_one();
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
