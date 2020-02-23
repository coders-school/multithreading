// (zadanie 03_ping_pong.cpp z rozdziału o zmiennych warunku

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

class PingPong {
    int repetitions_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        // TODO: implement me :)
    }

    void pong() {
        // TODO: implement me :)
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
