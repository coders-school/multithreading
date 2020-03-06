#include <iostream>
#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

constexpr chrono::milliseconds flightTime = 300ms;
constexpr chrono::milliseconds clockTick = 200ms;

enum gameState {
    FINISH = 0, PING, PONG
};

class PingPong {
    mutex ballLock;

    atomic<int> counter;
    atomic<int> turnNumber = 1;
    atomic<bool> timerOut = false;

    atomic<gameState> status = gameState::PING;

    condition_variable condition;
public:

    explicit PingPong(int turns) : counter(turns) {}

    void ping() {
        while (counter && !timerOut) {
            unique_lock<mutex> lock(ballLock);
            condition.wait_for(lock, clockTick);
            if (status == PING && !timerOut) {
                cout << "ping " << turnNumber << endl;
                this_thread::sleep_for(flightTime);
                status = PONG;
                condition.notify_all();
            }
        }
    }

    void pong() {
        while (counter && !timerOut) {
            unique_lock<mutex> lock(ballLock);
            condition.wait_for(lock, clockTick);
            if (status == PONG) {
                cout << "pong " << turnNumber++ << endl;
                this_thread::sleep_for(flightTime);
                status = (--counter) ? PING : FINISH;
                condition.notify_all();
            }
        }
        condition.notify_all();
    }

    void stop(chrono::seconds timeout) {
        auto stopTime = chrono::steady_clock::now() + timeout;
        while (true) {
            unique_lock<mutex> lock(ballLock);
            condition.wait_for(lock, clockTick);
            {
                if (chrono::steady_clock::now() > stopTime) {
                    timerOut = true;
                    condition.notify_all();
                    cout << "Timeout\n";
                    break;
                }
                if (status == FINISH) {
                    cout << "End of the game\n";
                    break;
                }
            }
            condition.notify_all();
        }
    }
};

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Please use as follows:" << endl
             << "pingpong turns timeout_in_seconds" << endl;
        return 100;
    }

    auto turns = stoi(argv[1]);
    auto timeout = stoi(argv[2]);

    PingPong table(turns);

    thread t1(&PingPong::ping, &table);
    thread t2(&PingPong::pong, &table);
    thread t3(&PingPong::stop, &table, chrono::seconds(timeout));

    t1.join();
    t2.join();
    t3.join();

    return 0;
}