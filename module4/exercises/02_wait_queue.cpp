#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
using namespace std;

template <typename T>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    using Lock = lock_guard<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
    }
    T pop() {
        Lock l(m_);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        Lock l(m_);
        return queue_.empty();
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
        while (sq.empty()) { /* nop */ }
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
