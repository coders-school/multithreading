#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <fstream>
using namespace std;

template <typename T, std::size_t N>
class WaitQueue {
    array<T, N> queue_;
    mutable mutex m_;
    condition_variable nonEmpty_;
    condition_variable nonFull_;
    size_t read {0};
    size_t write {0};

    using Lock = unique_lock<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        auto isFull = [&]{ return (write + 1) % N != read; };
        nonFull_.wait(l, isFull);
        queue_[write] = element;
        // write++ % N;
        write = (write + 1) % N;
        nonEmpty_.notify_all();
    }
    T pop() {
        Lock l(m_);
        auto hasData = [&]{ return read != write; };
        nonEmpty_.wait(l, hasData);
        auto top = queue_[read];
        // read++ % N;
        read = (read + 1) % N;
        nonFull_.notify_all();
        return top;
    }
};

using StringQueue = WaitQueue<string, 5>;

void saveToScreen(StringQueue & sq) {
    while (true) {
        cout << sq.pop() << endl;
        this_thread::sleep_for(1s);
    }
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++) {
        sq.push("This is random text number " + to_string(i));
        cout << "Pushed " << i << '\n';
    }
}

int main() {
    StringQueue sq;
    thread textProducer(produceText, ref(sq), 10);
    thread consumer(saveToScreen, ref(sq));
    textProducer.join();
    consumer.join();
    return 0;
}
