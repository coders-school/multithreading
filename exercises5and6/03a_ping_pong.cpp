#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <shared_mutex>

using namespace std;
using namespace std::literals::chrono_literals;

using lockMutex = std::unique_lock<std::mutex>;

class PingPong {
    int repetitions_;
    int counter_ = 0;
    bool close_ = false;
    std::mutex guard_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    void ping() {
        do 
        {
            while(counter_%2 != 0) {};
            
            std::lock_guard<std::mutex> lock(guard_);
            if(close_) { break; };
            std::cout << counter_ << " " << "ping" << std::endl;    
            ++counter_;
        
        }while (!close_ and (counter_ < repetitions_*2 -1));
    }

    void pong() {    
        do
        {
            while(counter_%2 == 0) {};

            std::lock_guard<std::mutex> lock(guard_);
            if(counter_ > repetitions_*2) { break; }; 
            std::cout << counter_ << " " << "pong" << std::endl;
            ++counter_;
        
        }while (!close_ and (counter_ < repetitions_*2));
    }

    void stop([[maybe_unused]] chrono::seconds timeout) {
        auto start = std::chrono::system_clock::now();
        do
        {
            std::this_thread::sleep_for(1s);
            auto current = std::chrono::duration_cast<std::chrono::seconds>(
                                std::chrono::system_clock::now() - start).count();
            close_ = current > timeout.count();

        }while(!close_ and (counter_ < repetitions_*2));

        
        while (counter_%2 == 0){};
        std::lock_guard<std::mutex> lock(guard_);
        counter_ %2 != 0 ? repetitions_ = counter_/2 + 1 : 0;
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
