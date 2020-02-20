#include <shared_mutex>
#include <mutex>
#include <vector>
#include <iostream>
#include <thread>
using namespace std;

vector<int> numbers = {};
shared_mutex numbersMtx;
mutex coutMtx;

int getNextValue() { 
    static int i = 0;
    return i+=10;
}

void read(int index) {
    for (int i = 0; i < 10; i++) {
        shared_lock<shared_mutex> lock(numbersMtx);
        int value = numbers[index];
        lock.unlock();
        lock_guard<mutex> coutLock(coutMtx);
        cout << value << " ";
    }
}

void write() {
    for (int i = 0; i < 3; i++) {
        lock_guard<shared_mutex> lock(numbersMtx);
        int newValue = getNextValue();
        numbers.emplace_back(newValue);
    }
}

int main() {
    vector<thread> writers;
    for(int i = 0; i < 10; i++)
        writers.emplace_back(write);

    cout << "Writers produced: ";
    for(const auto & n : numbers)
        cout << n << " ";
    cout << endl;

    cout << "Readers consumed: ";
    vector<thread> readers;
    for(int i = 0; i < 10; i++)
        readers.emplace_back(read, i);

    for(auto && writer : writers)
        writer.join();
    for(auto && reader : readers)
        reader.join();

    cout << endl;
    return 0;
}