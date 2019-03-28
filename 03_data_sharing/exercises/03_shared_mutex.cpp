#include <vector>
#include <iostream>
#include <thread>
using namespace std;

vector<int> numbers = {};

int getNextValue() { 
    static int i = 0;
    return i+=10;
}

void read(int index) {
    int value = numbers[index];
    cout << value << " ";
}

void write() {
    int newValue = getNextValue();
    numbers.emplace_back(newValue);
}

int main() {
    vector<thread> writers;
    for(int i = 0; i < 10; i++)
        writers.emplace_back(write);
    for(auto && writer : writers)
    	writer.join();

    cout << "Writers produced: ";
    for(const auto & n : numbers)
        cout << n << " ";
    cout << endl;

    cout << "Readers consumed: ";
    vector<thread> readers;
    for(int i = 0; i < 10; i++)
        readers.emplace_back(read, i);
    for(auto && reader : readers)
        reader.join();

    cout << endl;
    return 0;
}
