#include <algorithm>    // std::for_each()
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <functional>   // std::mem_fn() 
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <thread>

constexpr size_t itemsCnt = 5;       // number of items (philosophers, forks)
constexpr size_t actionCnt = 10;     // number of action to take by philosopher
constexpr bool action_debug = false; // additional debugs

std::mutex forks[ itemsCnt ];
std::thread philosopher[itemsCnt];

std::mutex displayMutex;
std::mutex updateMutex;
std::atomic<bool> canEat[itemsCnt];
std::atomic<bool> eat[itemsCnt];

using namespace std::chrono_literals;

void displayState()
{
    std::stringstream ss;

    std::lock_guard<std::mutex> lock( displayMutex );

    for ( int i = 0; i < itemsCnt; ++i )
    {
        if ( eat[i] )
        {
            ss << "-F" << i << "- ";
        }
        else
        {
            ss << " F" << i << "  ";
        }
    }
    ss << std::endl;
    std::cout << ss.str();
}

void action( int idx )
{
    int left_fork = idx;
    int right_fork = ( left_fork + 1 ) % 5;
    int iActionStep = 0;

    // std::cout << "action idx " << idx << " l " << left_fork << " r " << right_fork << std::endl; 

    while ( iActionStep < actionCnt )
    {
        if (canEat[idx] == true)
        {
            std::scoped_lock lock( forks[left_fork], forks[right_fork] );
            
            // NOTE: Here is a race condition on eat[idx] variable and display_state.
            // Race can be solved by using i.e. queue.

            displayMutex.lock();
            eat[idx] = true;
            displayMutex.unlock();

            if ( action_debug )
            {
                std::stringstream ss;
                ss << "[" << iActionStep << "] " 
                    <<"F" << idx 
                    << " gained forks: " << left_fork << " : " << right_fork << std::endl;
                std::cout << ss.str();
            }

            displayState();
            
            displayMutex.lock();
            eat[idx] = false;
            displayMutex.unlock();
            
            canEat[idx] = false;
            iActionStep++;

            if ( action_debug )
            {
                std::stringstream ss2;
                ss2 <<"F" << idx << " end lock" << std::endl;
                std::cout << ss2.str();
            }
            
        }
        else
        {
            std::lock_guard<std::mutex> lock(updateMutex);

            // std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 100));

            // check round
            bool allEaten = true;
            for (int i = 0; i < itemsCnt; ++i)
            {
                if ( canEat[i] )
                {
                    allEaten = false;
                    break;
                }
            }

            // next round ?
            if (allEaten)
            {
                for (int i = 0; i < itemsCnt; ++i)
                {
                    canEat[i] = true;
                }
            }
        }
    }
}

int main() 
{
    // init
    for (int i = 0; i < itemsCnt; ++i)
    {
        canEat[i] = true;
        eat[i] = false;
    }
    std::srand(std::time(nullptr));

    // start threads
    for (int i = 0; i < itemsCnt; ++i)
    {
       philosopher[i] = std::thread( action, i ); 
    }
    
    // join threads
    std::for_each(&(philosopher[0]), &(philosopher[itemsCnt]), std::mem_fn(&std::thread::join));
    
    return 0;
}