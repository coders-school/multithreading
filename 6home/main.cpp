#include <thread>
#include <algorithm>
#include <iostream>
#include <vector>
#include <mutex>
#include <utility>
#include <cstdlib>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <sstream>


constexpr std::chrono::milliseconds waitTime = std::chrono::milliseconds(350);

class PingPong
{
    public:
        std::mutex gameStateMutex;
        std::mutex m;
        std::condition_variable cv;
        std::atomic<bool> gameOn {true};
        std::atomic<int> counter {0};
        bool canPingWrite {true};
        int rebounds;

        PingPong(const int &rebounds_) 
                : rebounds(rebounds_) {};
    
        void timer(const std::chrono::seconds &gameTime)
        {
            auto stopAt = std::chrono::steady_clock::now() + gameTime;
            std::unique_lock<std::mutex> lock(m);
            cv.wait_until(lock, stopAt, [&]{ return !gameOn; });
            if (std::chrono::steady_clock::now() >= stopAt)
            {
                gameOn = false;
                cv.notify_all();
                std::cout << "Timeout ...\n";
            }
            if (!gameOn)
            {
                cv.notify_all();
            }
        }

        void ping()
        {
            while(counter < rebounds and gameOn)
            {
                std::unique_lock<std::mutex> lock(m);
                cv.wait(lock, [&](){return canPingWrite;});
                if(gameOn)
                {
                    std::cout << ++counter << " Ping\n";
                    std::this_thread::sleep_for(waitTime);
                    canPingWrite = false;
                    cv.notify_all();
                }
            }
        }

        void pong()
        {
            while(counter <= rebounds and gameOn)
            {
                std::unique_lock<std::mutex> lock(m);
                cv.wait(lock, [&](){return !canPingWrite or !gameOn;});
                
                if(counter >= rebounds and gameOn)
                { 
                    gameOn = false;
                    std::cout << counter << " Pong\n";
                    std::cout << "Out of rebounds..." << std::endl;
                }
                else
                {
                    std::cout << counter << " Pong\n";
                    std::this_thread::sleep_for(waitTime);
                    canPingWrite = true;
                }

                cv.notify_all();
            }
        }
};

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Wrong usage\n ./main <time_seconds> <rebounds>\n";
        exit(0);
    }


    std::chrono::seconds time =std::chrono::seconds(std::stoi(argv[1]));
    int rebounds = std::stoi(argv[2]); 
    PingPong pp{rebounds};
    std::cout << "time_seconds : " << std::chrono::duration_cast<std::chrono::seconds>(time).count() << "\nrebounds: " << rebounds << std::endl;
    std::thread timerThread(&PingPong::timer, &pp, std::cref(time));
    std::thread pingThread(&PingPong::ping, &pp);
    std::thread pongThread(&PingPong::pong, &pp);

    timerThread.join();
    pingThread.join();
    pongThread.join();
    return 0;
}