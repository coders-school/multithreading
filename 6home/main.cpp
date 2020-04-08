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



std::mutex m;
std::condition_variable cv;
bool canPingWrite {true};
std::atomic<bool> gameOn {true};
std::atomic<int> counter {0};

void timer(const int &time)
{
    while(gameOn)
    {
        std::this_thread::sleep_for(std::chrono::seconds(time));
        gameOn = false;
        std::cout << "Timeout ... " << std::endl;
    }
}

void ping(const int &rebounds)
{
    while(counter < rebounds and gameOn)
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&rebounds](){return canPingWrite;});
        if(gameOn)
        {
            ++counter;
            std::cout << counter << " Ping\n";
            canPingWrite = false;
        }
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(333));
    }
}

void pong(const int &rebounds)
{
    while(gameOn and counter <= rebounds)
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&](){return !canPingWrite;});

        std::cout << counter << " Pong\n";
        canPingWrite = true;

        if(counter >= rebounds)
        { 
            gameOn = false;
            std::cout << "Out of rebounds..." << std::endl;
        }

        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(333));

    }
}


int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cerr << "Wrong usage\n ./main <time> <rebounds>\n";
        exit(0);
    }


    int time = std::stoi(argv[1]);
    int rebounds = std::stoi(argv[2]); 
    std::cout << "time : " << time << " rebounds " << rebounds << std::endl;
    std::thread timerThread(timer, std::cref(time));
    std::thread pingThread(ping, std::cref(rebounds));
    std::thread pongThread(pong, std::cref(rebounds));

    pingThread.join();
    pongThread.join();
    timerThread.detach();
    return 0;
}