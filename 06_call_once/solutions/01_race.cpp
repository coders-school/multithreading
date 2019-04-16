#include <mutex>
#include <vector>
#include <limits>
#include <functional>
#include <future>
#include <thread>
#include <iostream>
#include <sstream> 
#include <random>
#include <chrono>
using namespace std;

class Prize {
    once_flag once{};
    string winnerId{"unknown"};
    std::random_device dev{};
    std::mt19937 rng{dev()};  
    std::uniform_int_distribution<int> dist{10, 50};
    
public:
    void setWinner() {
        auto id = this_thread::get_id();
        auto sleepDuration = dist(rng);
        stringstream msg;
        msg << "Called " << __FUNCTION__ << "(" << id << "). Sleeping for " << sleepDuration << "ms\n";
        cout << msg.str(); // single operation on stream is atomic
        this_thread::sleep_for(chrono::milliseconds(sleepDuration));

        call_once(once, [&]{
            cout << "Call once for " << id << '\n';
            stringstream troublesomeConversion; 
            troublesomeConversion << id;  // no other way to convert thread::id :(
            winnerId = troublesomeConversion.str();
        });
    }
      
    void showWinner() {
        cout << ("And the winner is... " + winnerId + '\n');
        // this is single stream operation, which is atomic - no need for mutex
    }
};

int main() {
    Prize millionBucks;
    vector<thread> contestants;

    for (int i = 0; i < 10; i++)
        contestants.emplace_back(&Prize::setWinner, &millionBucks);

    for (auto && c: contestants)
        c.join();

    millionBucks.showWinner();
    return 0;
}
