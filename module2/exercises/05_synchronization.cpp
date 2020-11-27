// use store() /  load()
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

// * Use proper synchronization mechanisms
//   * std::mutex + locks
//   * std::atomic
// * Choose proper places for locking
// * Avoid unnecessary locking

vector<int> generateContainer() {
    const vector<int> input = {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
    vector<int> output;
    vector<thread> threads;
    mutex m;
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back([&, i] {
            lock_guard l{m};
            output.push_back(input[i]);
        });
    }
    for (auto&& t : threads) {
        t.join();
    }
    return output;
}

vector<int> generateOtherContainer() {
    atomic<int> start = 5;
    atomic<int> add = 1;
    vector<int> output;
    vector<thread> threads;
    mutex m;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&, i] {
            if (add % 2) {
                lock_guard l{m};
                output.push_back(start += i);
            } else {
                lock_guard l{m};
                output.push_back(start -= i);
            }
            add++;
        });
    }
    for (auto&& t : threads) {
        t.join();
    }
    return output;
}

void powerContainer(vector<int>& input) {
    vector<thread> threads;
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back([&, i] { input[i] *= input[i]; });
    }
    for (auto&& t : threads) {
        t.join();
    }
}

void printContainer(const vector<int>& c) {
    for (const auto& value : c)
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
