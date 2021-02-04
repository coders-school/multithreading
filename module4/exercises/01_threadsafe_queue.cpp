#include <iostream>
#include <deque>
#include <thread>
#include <string>
#include <fstream>
#include <mutex>
using namespace std;

template <typename T>
class ThreadsafeQueue {
    deque<T> queue_;
    mutable mutex m_;
    // TODO: Make it thread-safe :)

public:
    void push(const T & element) {
        lock_guard _{m_};
        queue_.push_front(element);
    }
    T pop() {
        lock_guard _{m_};
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        lock_guard _{m_};
        return queue_.empty();
    }
};

using StringQueue = ThreadsafeQueue<string>;

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
