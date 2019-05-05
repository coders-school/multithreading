#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <fstream>
using namespace std;

template <typename T, int N>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    condition_variable nonEmpty_;
    condition_variable nonFull_;
    using Lock = unique_lock<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        auto hasSpace = [&]{ return queue_.size() < N; };
        nonFull_.wait(l, hasSpace);
        queue_.push_front(element);
        nonEmpty_.notify_all();
    }
    T pop() {
        Lock l(m_);
        auto hasData = [&]{ return not queue_.empty(); };
        nonEmpty_.wait(l, hasData);
        auto top = queue_.back();
        queue_.pop_back();
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
        cout << "Produced " << i << '\n';
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
