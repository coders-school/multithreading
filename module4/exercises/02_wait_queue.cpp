#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <condition_variable>
using namespace std;

template <typename T>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    condition_variable cv_;
    using Lock = unique_lock<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
        cv_.notify_one();
    }
    T pop() {
        Lock l(m_);
        cv_.wait(l, [&] { return !queue_.empty(); });
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
};

using StringQueue = WaitQueue<string>;

void provideData(StringQueue & sq) {
    string txt;
    while (getline(cin, txt))
        sq.push(txt);
}

void saveToFile(StringQueue & sq) {
    ofstream file("/tmp/sth.txt");
    while (file) {
        file << sq.pop() << endl;
    }
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " + to_string(i));
}

int main() {
    StringQueue sq;
    thread userDataProducer(provideData, ref(sq));
    thread textProducer(produceText, ref(sq), 10);
    thread consumer(saveToFile, ref(sq));
    userDataProducer.join();
    textProducer.join();
    consumer.join();
    return 0;
}
