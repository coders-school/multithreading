#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

class PingPong
{
private:
    int repetitions_;
    std::atomic<int> index_ {0};
    int indexPings_ = 0;
    int indexPongs_ = 0;
    std::atomic<bool> continuePlay {true};
    std::mutex m_;
    std::condition_variable cv_;

public:
    PingPong(int repetitions)
        : repetitions_(repetitions) { }

    void ping()
    {
        while (index_ <= repetitions_ && continuePlay)
        {
            auto shouldIcontinuePing = [&]{ return ((indexPings_ == indexPongs_) and (index_ <= repetitions_)); };
            std::unique_lock<std::mutex> l(m_);
            cv_.wait(l, shouldIcontinuePing);

            if (!continuePlay)
                break;

            if (index_ < repetitions_)
            {
                std::stringstream ss;
                ss << "Ping " << index_ << std::endl;
                std::cout << ss.str();

                indexPings_++;
            }
            else
            {
                std::cout << "Ping reached repetitions limit\n";
                indexPings_++;
            }

            cv_.notify_all();
            std::this_thread::sleep_for(250ms);
        }
    }

    void pong()
    {
        while (index_ <= repetitions_)
        {
            auto shouldIcontinuePong = [&](){ return indexPongs_ < indexPings_; };
            std::unique_lock<std::mutex> l(m_);
            cv_.wait(l, shouldIcontinuePong);

            if (index_ < repetitions_)
            {
                std::stringstream ss;
                ss << "Pong " << index_ << std::endl;
                std::cout << ss.str();

                indexPongs_++;
                index_++;
            }
            else
            {
                std::cout << "Pong reached repetitions limit\n";

                continuePlay = false;
                indexPongs_++;
            }
            
            cv_.notify_all();
            std::this_thread::sleep_for(250ms);
        }

    }

    void stop([[maybe_unused]] std::chrono::seconds timeout)
    {
        auto start = std::chrono::steady_clock::now();

        std::unique_lock<std::mutex> l(m_);
        cv_.wait(l, [&](){
            auto now = std::chrono::steady_clock::now();
            auto diff = (std::chrono::duration_cast<std::chrono::seconds>(now - start)).count();
            return (indexPongs_ == indexPings_) && (!continuePlay || (diff >= timeout.count()));
            });
        
        if (continuePlay)
        {
            std::stringstream ss;
            ss << "Timeout" << std::endl;
            std::cout << ss.str();

            continuePlay = false;
            cv_.notify_all();
        }
        
        cv_.notify_all();
    }
};

int main(int argc, char** argv)
{
    if (argc != 3)
        return -1;

    int repetitions = std::stoi(argv[1]);
    int timeout = std::stoi(argv[2]);

    PingPong pp(repetitions);

    std::thread t1(&PingPong::ping, &pp);
    std::thread t2(&PingPong::pong, &pp);
    std::thread t3(&PingPong::stop, &pp, std::chrono::seconds(timeout));

    t1.join();
    t2.join();
    t3.join();

    return 0;
}