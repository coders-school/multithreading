<!-- .slide: data-background="#111111" -->

# Recap

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### 1. What was the most surprising for you? 😲

### 2. What was the most obvious for you? 🥱

___
<!-- .slide: style="font-size: .81em" -->

## Points to remember

* <!-- .element: class="fragment fade-in" --> <code>condition_variable</code>
  * <!-- .element: class="fragment fade-in" --> Always use <code>wait()</code> with a predicate!
  * <!-- .element: class="fragment fade-in" --> Remember about spurious wake-ups and lost notifications
  * <!-- .element: class="fragment fade-in" --> Remember about a fight for locking a mutex when you use <code>notify_all()</code>
  * <!-- .element: class="fragment fade-in" --> You can't choose which thread should wake up when you use <code>notify_one()</code>
* <!-- .element: class="fragment fade-in" --> <code>atomic</code>
  * <!-- .element: class="fragment fade-in" --> Only a single operation on atomic type is atomic. Read + write may not be atomic, if you use it incorrectly
  * <!-- .element: class="fragment fade-in" --> You need to use proper operators to have atomic read + write, or use <code>fetch_*()</code> functions, <code>exchange()</code> or <code>compare_exchange_*()</code>
  * <!-- .element: class="fragment fade-in" --> The default memory order for <code>atomic</code> is sequential consistency. If you need some optimizations you have to loosen the memory order by specifying it manually.
  * <!-- .element: class="fragment fade-in" --> Memory order acquire is used with <code>load()</code> and release is used with <code>store</code>. If you exchange them, you have undefined behavior.
* <!-- .element: class="fragment fade-in" --> <code>call_once</code>
  * <!-- .element: class="fragment fade-in" --> Mind the <span style="text-decoration: line-through;">gap</span> bug in g++ and clang++ - exceptional call may hang the program
  * <!-- .element: class="fragment fade-in" --> If you use the same <code>once_flag</code> for different functions, you don't know which function will be called.

___
<!-- .slide: style="font-size: .65em" -->

## Pre-test 🤯

<div class="multicolumn">

<div style="width: 60%">

```cpp
// assume all necessary includes are here

int main() {
    std::mutex m;
    std::condition_variable cv;
    std::vector<int> v;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    auto consume = [&] {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul);
        std::cout << v.back();
        v.pop_back();
    };
    for (int i = 0; i < 10; i++) consumers.emplace_back(consume);

    auto produce = [&](int i) {
        {
            std::lock_guard<std::mutex> lg(m);
            v.push_back(i);
        }
        cv.notify_all();
    };
    for (int i = 0; i < 10; i++) producers.emplace_back(produce, i);

    for (auto && p : producers) p.join();
    for (auto && c : consumers) c.join();
}
```

</div>

<div class="col" style="margin-top: 20px">

1. <!-- .element: class="fragment highlight-green" --> there may be an Undefined Behavior in this code
2. <!-- .element: class="fragment highlight-red" --> the output is guaranteed to always be <code>0123456789</code>
3. <!-- .element: class="fragment highlight-red" --> <code>v</code> is always an empty vector at the end of this program
4. <!-- .element: class="fragment highlight-green" --> if some producers threads started before some consumers, we would have a deadlock because of lost notifications
5. <!-- .element: class="fragment highlight-green" --> a change from <code>notify_all()</code> to <code>notify_one()</code> guarantees that each consumer thread will receive a different number
6. <!-- .element: class="fragment highlight-green" --> this code can be improved by providing a predicate to <code>wait()</code> to disallow getting elements when the vector is empty

Note: 1, 4, 5, 6

</div>

</div>

___

## Post-work

* Ping-pong
  * difficult version - exercises/03a_ping_pong.cpp
  * easier version - exercises/03b_ping_pong.cpp
* [Post-test](https://forms.gle/dgw17Ac5vCT1YBAw7)
* [Training evaluation](https://forms.gle/ADXRttpAaZgW8KwM6)

___

## Homework: ping-pong

<div class="multicolumn">

<div style="width: 60%; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> Thread A prints "ping" and the consecutive number
* <!-- .element: class="fragment fade-in" --> Thread B prints "pong" and the consecutive number
* <!-- .element: class="fragment fade-in" --> Ping always starts. Pong always ends.
* <!-- .element: class="fragment fade-in" --> Threads must work in turns. There may not be 2 consecutive pings or pongs. The program cannot end with a ping without a respective pong.
* <!-- .element: class="fragment fade-in" --> The program must be terminated either after the given number of repetitions or after the time limit, whichever occurs first. The reason for termination should be displayed on the screen.
* <!-- .element: class="fragment fade-in" --> Program parameters:
  * <!-- .element: class="fragment fade-in" --> number of repetitions
  * <!-- .element: class="fragment fade-in" --> time limit (in seconds)

</div>

<div style="width: 40%; font-size: .85em;">

```bash
$> g++ 03_ping_pong.cpp -lpthread
-std=c++17 -fsanitize=thread
$> ./a.out 1 10
ping 0
pong 0
Ping reached repetitions limit
Pong reached repetitions limit
$> ./a.out 12 1
ping 0
pong 0
ping 1
pong 1
ping 2
pong 2
Timeout
```

</div> <!-- .element: class="fragment fade-in" -->

</div>

___

## Tips

If you got stuck:

* <!-- .element: class="fragment fade-in" --> You need a mutex and a condition variable in your <code>PingPong</code> class
* <!-- .element: class="fragment fade-in" --> Wait for a condition variable with <code>wait_for()</code> in <code>stop()</code> function
* <!-- .element: class="fragment fade-in" --> Check the number of repetitions in ping and pong threads
* <!-- .element: class="fragment fade-in" --> Use an additional <code>std::atomic<bool></code> variable which will tell all threads to end, when the required conditions are met.
* <!-- .element: class="fragment fade-in" --> Ping and pong threads should be using <code>wait()</code> to check if it's their turn to work. Use an additional variable that will be used in the predicate passed to <code>wait()</code>.
* <!-- .element: class="fragment fade-in" --> The pong thread should end the program after reaching the repetition limit

___

## Useful links

* <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2393.html">C++ Atomic Types and Operations (C++ Standard)</a>
* <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>
* <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">std::memory_order on cppreference.com</a>
* <a href="https://en.cppreference.com/w/cpp/thread/call_once">std::call_once on cppreference.com</a>
* <a href="https://en.cppreference.com/w/cpp/thread/once_flag">std::once_flag on cppreference.com</a>
  * <a href="https://github.com/PaddlePaddle/Paddle/issues/6681">STL bug in exception handling in call_once</a>
  * <a href="https://stackoverflow.com/questions/26985370/stdcall-once-vs-stdmutex-for-thread-safe-initialization">call_once vs mutex on stackoverflow</a>
* <a href="https://stackoverflow.com/questions/17712001/how-is-meyers-implementation-of-a-singleton-actually-a-singleton">Meyers Singleton on stackoverflow</a>
* [Atomic<> Weapons by Herb Sutter](https://www.youtube.com/watch?v=A8eCGOqgvH4)
