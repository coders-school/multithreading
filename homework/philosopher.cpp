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
#include <vector>

constexpr size_t itemsCnt = 5;       // number of items (philosophers, forks)
constexpr size_t actionCnt = 10;     // number of action to take by philosopher
constexpr bool action_debug = false; // additional debugs

std::mutex forks[ itemsCnt ];
std::thread philosopher[itemsCnt];

std::mutex displayMutex;
std::mutex updateMutex;
std::atomic<bool> philCanEat[itemsCnt];   // philosopher can eat
std::atomic<bool> philEat[itemsCnt];      // philosopher is eating

using namespace std::chrono_literals;

struct report_item
{
    int iPhilosopherID;
    bool bEat;
    decltype(std::chrono::steady_clock::now()) time;
};

std::vector<report_item> resport_events(itemsCnt * actionCnt * 2);
int resport_events_idx = 0;

void displayState()
{
    std::stringstream ss;

    std::lock_guard<std::mutex> lock( displayMutex );

    for ( int i = 0; i < itemsCnt; ++i )
    {
        if ( philEat[i] )
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
        if (philCanEat[idx] == true)
        {
            std::scoped_lock lock( forks[left_fork], forks[right_fork] );
            
            // NOTE: Here is a small race condition on philEat[idx] variable and display_state() function.
            // Proper results are shown in post processing. See show events in main function().

            displayMutex.lock();
            philEat[idx] = true;
            resport_events[resport_events_idx++] = {idx, true, std::chrono::steady_clock::now()};
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
            philEat[idx] = false;
            resport_events[resport_events_idx++] = {idx, false, std::chrono::steady_clock::now()};
            displayMutex.unlock();
            
            philCanEat[idx] = false;
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
                if ( philCanEat[i] )
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
                    philCanEat[i] = true;
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
        philCanEat[i] = true;
        philEat[i] = false;
    }
    std::srand(std::time(nullptr));
    auto start = std::chrono::steady_clock::now();

    // start threads
    for (int i = 0; i < itemsCnt; ++i)
    {
       philosopher[i] = std::thread( action, i ); 
    }
    
    // join threads
    std::for_each(&(philosopher[0]), &(philosopher[itemsCnt]), std::mem_fn(&std::thread::join));
    
    std::cout << "------------------------------------" << std::endl;

    for( std::atomic<bool>& a : philEat )
    {
        a = false;
    }

    // sort report events
    std::sort(resport_events.begin(), resport_events.end(), [](report_item a, report_item b){ return a.time < b.time;});

    // show events
    std::cout << "time\tID\teat\tstatus" << std::endl;
    for( report_item ri : resport_events)
    {
        philEat[ri.iPhilosopherID] = ri.bEat;

        std::cout <<  std::chrono::duration_cast<std::chrono::microseconds>(ri.time - start).count() << "\t" 
            << "F" << ri.iPhilosopherID << "\t" << ri.bEat << "\t";
        displayState();
    }

    return 0;
}