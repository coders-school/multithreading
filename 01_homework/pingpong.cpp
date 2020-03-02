#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>

using namespace std;

class PingPong {
    long int repetitions_;
    atomic<bool> game_is_on_{true};
    atomic<bool> ping_turn_{true};
    mutex mutex_;
    condition_variable cv_ping_, cv_pong_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

        void ping() {
            long int ping_reps = 0;
            auto my_turn_check = [&]{return ping_turn_ == true;};
            unique_lock<mutex> ping_lock(mutex_);

            while (ping_reps < repetitions_) {
                cout << "ping ";
                ping_turn_.store(false);
                ping_lock.unlock();
                cv_pong_.notify_one();
                ping_reps++;

                this_thread::sleep_for(100ms);

                ping_lock.lock();
                cv_ping_.wait(ping_lock, my_turn_check);
            }
        }

        void pong() {
            long int pong_reps = 0;
            auto my_turn_check = [&]{return ping_turn_ == false;};
            unique_lock<mutex> pong_lock(mutex_, defer_lock);

            while (pong_reps < repetitions_) {
                pong_lock.lock();
                cv_pong_.wait(pong_lock, my_turn_check);
                cout << "pong : " << pong_reps << "\n";
                ping_turn_.store(true);
                pong_lock.unlock();
                cv_ping_.notify_all();
                pong_reps++;
            }
        }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        // TODO: should stop execution after timeout
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
