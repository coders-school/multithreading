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

* <!-- .element: class="fragment fade-in" -->A
* <!-- .element: class="fragment fade-in" -->B

___

<!-- .slide: style="font-size: 0.7em" -->

## Pre-test answers

```cpp []
#include <future>
#include <iostream>

int main() {
    int x = 0;
    auto f = std::async(std::launch::deferred, [&x]{
        x = 1;
    });

    x = 2;
    f.get();
    x = 3;
    std::cout << x;
    return 0;
}
```

1. <!-- .element: class="fragment highlight-red" --> the type of f is <code>promise&lt;int&gt;</code>
1. <!-- .element: class="fragment highlight-green" --> the type of f is <code>future&lt;void&gt;</code>
1. <!-- .element: class="fragment highlight-green" --> <code>async()</code> without a launch policy may never be called
1. <!-- .element: class="fragment highlight-green" --> this program always prints 3
1. <!-- .element: class="fragment highlight-red" -->  <code>x = 2</code> assignment cause a data race
1. <!-- .element: class="fragment highlight-green" --> if async was run with <code>std::launch::async</code>, there would be a data race
1. <!-- .element: class="fragment highlight-green" --> <code>x = 3</code> assignment is safe, because it happens after synchronization with async task
1. <!-- .element: class="fragment highlight-green" --> <code>future&lt;void&gt;</code> may be used to synchronize tasks

Note: 2, 3, 4, 6, 7, 8

___

## Post-work

* Ping-pong
  * difficult version - exercises/03a_ping_pong.cpp
  * easier version - exercises/03b_ping_pong.cpp
* [Post-test](https://forms.gle/tF9a3Q2gATwnUmWU8)
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
