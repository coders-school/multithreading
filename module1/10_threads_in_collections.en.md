<!-- .slide: data-background="#111111" -->

# Threads in collections

___

## Task: threads in collections

* <!-- .element: class="fragment fade-in" --> Write a short program in which 20 threads are held in a vector.
* <!-- .element: class="fragment fade-in" --> Each thread has to wait 1 second, then display its number, which it takes as a parameter and a newline character.
* <!-- .element: class="fragment fade-in" --> Creating threads and joining them should be done in 2 separate loops
* <!-- .element: class="fragment fade-in" --> Run the program several times and observe the results

___

## Task 5 - solution

```cpp
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

void do_work(int id) {
    this_thread::sleep_for(1s);
    cout << id << endl;
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```

___

## Task 5 - possible results

```output
$> ./a.out
0 1 3 2 5 6 8 10 12 7 11 9 13 4 14 15 17 16 19 18

$> ./a.out
4 5 7 6 14 15 16 18 13 11 9 2 0 8 10 17 12 1 19 3

$> ./a.out
18 016 2 3 4 5 615 8 9 10 11 12 13 14 19 1 7 17

$> ./a.out
0 1 16 3 199 815 11 5 18 17 4 12 13 7 102 6 14
```

___

## Task 5 - possible results

* <!-- .element: class="fragment fade-in" --> Why is this happening?
* <!-- .element: class="fragment fade-in" --> The output stream <code>cout</code> is only one. It is a shared resource that is shared between threads
* <!-- .element: class="fragment fade-in" --> There may be interlaces when accessing the stream (one thread will start typing something and will not finish, and the second one will interrupt it and enter its number)
* <!-- .element: class="fragment fade-in" --> Sharing resources is a common threading problem
* <!-- .element: class="fragment fade-in" --> How to deal with this? This is a topic for the next lesson 🙂
