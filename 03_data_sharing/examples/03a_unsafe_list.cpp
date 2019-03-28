#include <list>
#include <thread>
#include <algorithm>
#include <iostream>
using namespace std;

void addToList(list<int> & l, int value) {
    l.push_back(value);
}

void listContains(list<int> & l, int value, bool & result) {
    result = find(l.begin(), l.end(), value) != l.end();
}

int main() {
    list<int> someList{};
    thread t1(addToList, ref(someList), 5);
    thread t2(addToList, ref(someList), 10);
    thread t3(addToList, ref(someList), 15);
    bool has15{};
    thread t4(listContains, ref(someList), 15, ref(has15));
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    for(const auto & element : someList) {
        cout << element << " ";
    }

    cout << boolalpha << has15 << endl;
}
