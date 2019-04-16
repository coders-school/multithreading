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
            opponentsTurn_.wait(l, [&] { return isPingTurn_; });
            // play checking cannot be in wait, because stop set it to false
            // and we will never wake up to finish job after timeout
            cout << "ping " << reps << endl;
            reps++;
            isPingTurn_ = false;
            l.unlock();
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_)
        {
            cout << "Ping reached repetitions limit" << endl;
        }
    }

    void pong() {
        thread_local int reps = 0;
        while (reps < repetitions_ and play_)
        {
            unique_lock<mutex> l(m_);
            opponentsTurn_.wait(l, [&] { return not isPingTurn_; });
            // play checking cannot be in wait, because stop set it to false
            // and we will never wake up to finish job after timeout
            cout << "pong " << reps << endl;
            reps++;
            isPingTurn_ = true;
            l.unlock();
            this_thread::sleep_for(500ms);
        }
        if (reps >= repetitions_) {
            cout << "Pong reached repetitions limit" << endl;
            play_ = false;
            opponentsTurn_.notify_all();
        }
    }

    void stop(chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        auto wasTimeout = opponentsTurn_.wait_for(l, timeout, [&]{ return not play_; });
        if (not wasTimeout) {
            play_ = false;
            cout << "Timeout" << endl;
        }
        opponentsTurn_.notify_all();
    }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage " << argv[0] << " repetition timeout" << '\n';
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
