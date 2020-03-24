#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include <chrono>
#include <sstream>
#include <mutex>

using namespace std::chrono_literals;

std::mutex mtx;

class CPingPong{

    std::string name_;
    std::chrono::high_resolution_clock::time_point startOfLive_;
    std::stringstream ss;

public:
    static std::atomic<int> counter;
    static std::string first_;
    static std::string second_;
    static std::atomic<bool> currentThread;
    CPingPong(std::string name) : name_(name), startOfLive_(std::chrono::high_resolution_clock::now()){
    }

    void act(){

        auto timeOfLive = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startOfLive_).count();

        while(timeOfLive < 150 or counter <= 4){
            ss << name_ << "     time of live: " << timeOfLive << "      counter: " << counter << "\n";
            std::lock_guard<std::mutex> lg(mtx);
            std::cout << ss.str();
            counter++;
        
            timeOfLive = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startOfLive_).count();
        }
    }

};

std::atomic<int> CPingPong::counter = 0;
std::string CPingPong::first_ = "ping";
std::string CPingPong::second_ = "pong";
std::atomic<bool> CPingPong::currentThread = true;

void play(){
    CPingPong ping("ping");
    CPingPong pong("pong");
    std::thread t1(&CPingPong::act, &ping);
    std::thread t2(&CPingPong::act, &pong);    

    t1.join();
    t2.join();
}

int main(){

    play();

    return 0;
}
