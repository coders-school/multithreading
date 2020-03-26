#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std;

class PingPong {
    int repetitions_;
    std::mutex coutMutex;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        std::stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            ss << "ping" << i << ' ';
            lock_guard<mutex> lockCout(coutMutex);
            std::cout << ss.rdbuf();
        }
    }

    void pong() {
        std::stringstream ss;

        for(int i = 0; i < repetitions_; ++i) {
            ss << "pong" << i << '\n';
            lock_guard<mutex> lockCout(coutMutex);
            std::cout << ss.rdbuf();
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
