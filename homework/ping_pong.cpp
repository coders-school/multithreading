#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

using namespace std::chrono_literals;

constexpr std::chrono::milliseconds service_time = 500ms;
constexpr std::chrono::milliseconds thread_sleep_time = 100ms;

enum EToken
{ NONE = 0,
  PING,
  PONG
};

class PingPong {
    std::atomic<int> repetitions_;
    std::atomic<int> service = 1;
    std::atomic<bool> endTime = false;
    std::atomic<EToken> token = EToken::PING;
    std::condition_variable cvPingPong;
    std::mutex mPingPong;
public:

    PingPong(int repetitions)
        : repetitions_(repetitions)
    {}

    bool IsTokenNone() { return token == NONE; }

    void ping() {
        while ( repetitions_ > 0 && !endTime )
        {
            std::unique_lock<mutex> lock( mPingPong );
            cvPingPong.wait_for( lock, thread_sleep_time );
            if ( token == PING && !endTime)
            {
                std::cout << "ping " << service << std::endl;
                std::this_thread::sleep_for( service_time );

                token = PONG;
                NotifyAll();
            }
        }
    }

    void pong() {
        while ( repetitions_ > 0 && !endTime)
        {
            std::unique_lock<mutex> lock( mPingPong );
            cvPingPong.wait_for( lock, thread_sleep_time );
            if ( token == PONG )
            {
                std::cout << "pong " << service << std::endl;
                std::this_thread::sleep_for( service_time );

                service++;
                repetitions_--;
                token = ( repetitions_ > 0 ) ? PING : NONE;

                NotifyAll();
            }
        }
        NotifyAll();
    }

    void stop([[maybe_unused]] chrono::seconds timeout) 
    {
        auto stopTime = std::chrono::steady_clock::now() + timeout;

        while( true )
        {
            std::unique_lock<mutex> lock(mPingPong);
            cvPingPong.wait_for( lock, thread_sleep_time );
            {
                if ( std::chrono::steady_clock::now() > stopTime )
                {
                    endTime = true;
                    NotifyAll();
                    break;
                }

                if (token == NONE)
                {
                    break;
                }
            }

            NotifyAll();
        }
    }

    void NotifyAll()
    {
        cvPingPong.notify_all();
    }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Not enough parameters." << std::endl;
        std::cout << "Proper usage:" << std::endl;
        std::cout << "ping_pong <repetitions> <number_of_seconds>" << std::endl;
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

    if (pp.IsTokenNone())
    {
        std::cout << "End game." << std::endl;
    }
    else
    {
        std::cout << "Timeout." << std::endl;
    }

    return 0;
}
