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
    int repsPing_ = 0;
    int repsPong_ = 0;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        while (repsPing_ <= repetitions_ and play_)
        {
            if(not play_){
                break;
            }
            
            if(repsPing_ < repetitions_){
                unique_lock<mutex> l(m_);
                opponentsTurn_.wait(l, [&](){
                    return (isPingTurn_ and (repsPing_ == repsPong_));
                });
                printingFunction("ping", (repsPing_+1));
                repsPing_++;
                isPingTurn_ = false;
                this_thread::sleep_for(500ms);
            }

            else
                printingFunction("Ping repetition limit");

            opponentsTurn_.notify_all();
            this_thread::sleep_for(500ms);
        }
    }

    void pong() {
        while (repsPong_ <= repetitions_ and play_)
        {
            if(not play_){
                break;
            }
            if(repsPong_ < repetitions_){
                unique_lock<mutex> l(m_);
                opponentsTurn_.wait(l, [&](){
                    return ((not isPingTurn_) and(repsPong_ == repsPing_ -1));
                });

                printingFunction("pong", (repsPong_+1));
                repsPong_++;
                isPingTurn_ = true;             
            }

            else{
                printingFunction("Pong repetition limit.\nEnd of program");
                play_ = false;
            }

            opponentsTurn_.notify_all();
            this_thread::sleep_for(500ms);
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        auto timeStoper = [&](){
           return ((not play_) and (not isPingTurn_) and (repsPing_ != repsPong_));
       };
       
        if( not opponentsTurn_.wait_for(l, timeout, timeStoper)){
            printingFunction("End of program because of TIMEOUT ");
            play_ = false;
            opponentsTurn_.notify_all();
        }
    }

    void printingFunction (string first){
        stringstream print;
        print << first << endl;
        cout << print.str();
    }

    void printingFunction (string first, int second){
        stringstream print;
        print << first << " " << second << endl;
        cout << print.str();
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
