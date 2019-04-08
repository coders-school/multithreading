#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <iostream>
using namespace std;


vector<int> generateContainer() {
    vector<int> input = {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
    // no need to synchronize input - it's read only
    vector<int> output;
    output.reserve(input.size());
    vector<thread> threads;
    mutex outputMutex;
    
    auto pushOperation = [&](auto i){
        // lambda - i must be taken by copy
        lock_guard<mutex> lg(outputMutex);
        output.push_back(input[i]);
    };
    
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back(pushOperation, i);
        // no need to synchronize threads vector
        // emplacing is done sequentialy in a loop in main thread only.
    }
    for (auto && t : threads) {
        t.join();
    }
    return output;
}

vector<int> generateOtherContainer() {
    atomic<int> start{1};
    atomic<bool> add{true};
    vector<int> output;
    vector<thread> threads;
    mutex outputMutex;
    auto threadFunction = [&](auto i){
        // lambda - i variable must be passed by copy
        if (add) {
            lock_guard<mutex> lg(outputMutex);
            output.push_back(start+=i);
            // start is protected by mutex in both cases
            // so it even doesn't need to be atomic
        } else {
            lock_guard<mutex> lg(outputMutex);
            output.push_back(start-=i);
        }
        add = !add; // problematic use of atomic
                    // it doesn't protect from data races here
                    // reading and writing are 2 separate operations and value 
                    // can change between them. Only the lone read or write is safe
    };

    for (int i = 0; i < 10; i++) {
        threads.emplace_back(threadFunction, i);
        // no need to synchronize threads vector
        // emplacing is done sequentialy in a loop in main thread only.
    }
    for (auto && t : threads) {
        t.join();
    }
    return output;
}

void powerContainer(vector<int>& input) {
    vector<thread> threads;
    auto powerElement = [&](auto i){ input[i]*=input[i]; };
    // no need to synchronize on input - every thread writes to a different memory region
    // lambda takes `i` by copy 
    
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back(powerElement, i);
    }
    for (auto && t : threads) {
        t.join();
    }
}

void printContainer(const vector<int>& c) {
    for (const auto & value : c)
        cout << value << " ";
    cout << endl;
}

int main() {
    auto container1 = generateContainer();
    printContainer(container1);
    powerContainer(container1);
    printContainer(container1);

    auto container2 = generateOtherContainer();
    printContainer(container2);
    powerContainer(container2);
    printContainer(container2);
    return 0;
}

