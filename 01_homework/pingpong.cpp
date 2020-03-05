#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>

using namespace std;
constexpr auto step_time = std::chrono::milliseconds(10000);

class PingPong {
    long int repetitions_;
    atomic<bool> game_is_on_{true};
    atomic<bool> ping_turn_{true};
    mutex mutex_, to_mutex_;
    condition_variable cv_step_;

    void end_message() {
        if(game_is_on_) {
            game_is_on_.store(false);
            cout << "Max repetitions reached\n";
            cv_step_.notify_all();
        } else {
            cout << "Timeout reached\n";
        }
    }

    void pingponging(string text, bool is_ping) {
        long int ping_reps = 0;
        unique_lock<mutex> ping_lock(mutex_, defer_lock);
        unique_lock<mutex> to_lock(to_mutex_, defer_lock);
        auto ping_turn_check = [&]{return ping_turn_.load() == is_ping;};
        auto check_timeout = [&]{return not game_is_on_.load();};

        while (game_is_on_) {
            ping_lock.lock();
            cv_step_.wait(ping_lock, ping_turn_check);

            cout << text << " " << ping_reps << "\n";
            ping_turn_.store(not is_ping);
            ping_lock.unlock();
            cv_step_.notify_all();

            ping_reps++;
            if(ping_reps >= repetitions_) break;

            to_lock.lock();
            cv_step_.wait_for(to_lock, step_time, check_timeout);
            to_lock.unlock();
        }

        if(is_ping)
            cv_step_.notify_all();
        else
            end_message();
    }

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        pingponging("ping", true);
    }

    void pong() {
        pingponging("pong", false);
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        auto check_max_reps = [&]{return not game_is_on_.load();};

        unique_lock<mutex> stop_lock(mutex_);
        if(not cv_step_.wait_for(stop_lock, timeout, check_max_reps)) {
            game_is_on_.store(false);
            stop_lock.unlock();
            cv_step_.notify_all();
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
