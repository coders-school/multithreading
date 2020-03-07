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
                // TODO: wait to be used here + printing, reps incrementation, chainging turn to pong
                opponentsTurn_.wait(l, [&](){
                    return (isPingTurn_ and (repsPing_ == repsPong_));
                });
                std::stringstream print;
                print << "ping " <<repsPong_+1 << std::endl;
                std::cout << print.str();

                repsPing_++;
                isPingTurn_ = false;
                this_thread::sleep_for(500ms);

            }

            else{
                std::stringstream print;
                print << "Ping repetition limit." << std::endl;
                std::cout << print.str();
            }

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
                // TODO: wait to be used here + printing, reps incrementation, chainging turn to ping
                opponentsTurn_.wait(l, [&](){
                    return ((not isPingTurn_) and(repsPong_ == repsPing_ -1));
                });

                std::stringstream print;
                print << "pong " <<repsPong_+1 << std::endl;
                std::cout << print.str();

                repsPong_++;
                isPingTurn_ = true;
                
            }

            else{
                std::stringstream print;
                print << "Pong repetition limit. \nEnd of program." << std::endl;
                std::cout << print.str();

                play_ = false;
            }

            opponentsTurn_.notify_all();
            this_thread::sleep_for(500ms);
        }
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        unique_lock<mutex> l(m_);
        // TODO: wait_for to be used here. Check for a return value and set play_ to false       
       auto timeStoper = [&](){
           return ((not play_) and (not isPingTurn_) and (repsPing_ != repsPong_));
       };
       
        if( not opponentsTurn_.wait_for(l, timeout, timeStoper)){
            std::stringstream print;
            print << "End od program because of TIMEOUT" << std::endl;
            std::cout << print.str();
            
            play_ = false;
            opponentsTurn_.notify_all();
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
