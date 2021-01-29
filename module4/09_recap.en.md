<!-- .slide: data-background="#111111" -->

# Recap

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## 1. Describe one thing from today's session

## 2. What was the most obvious for you? 🙂

___
<!-- .slide: style="font-size: .81em" -->

## Points to remember

* <!-- .element: class="fragment fade-in" --> Your code is <span style="color: #cf802a">high-level</span> if you use only <code>std::async</code> and <code>std::future</code> object. Raw <code>std::thread</code>, <code>std::promise</code> or <code>std::packaged_task</code> objects means that it uses lower abstraction level, which is more complicated to understand.
* <!-- .element: class="fragment fade-in" --> Calling <code>std::async</code> without a policy may cause unexpected behavior, like task not being called at all.
* <!-- .element: class="fragment fade-in" --> <code>std::promise</code> can be set <span style="color: #cf802a">only once</span>
* <!-- .element: class="fragment fade-in" --> <code>std::future</code> can be get <span style="color: #cf802a">only once</span>
* <!-- .element: class="fragment fade-in" --> There is a <code style="color: limegreen">std::shared_future</code>, but there is no <code style="text-decoration: line-through; color: red">std::shared_promise</code>
* <!-- .element: class="fragment fade-in" --> <code>std::async</code> <span style="color: #cf802a">does NOT</span> always spawn a new thread. It may use a thread pool or run the task synchronously on the thread that called <code>get()</code>
* <!-- .element: class="fragment fade-in" --> Creating <code>std::future</code> via the default constructor <span style="color: #cf802a">does not make any sense</span>. It will be always invalid.

___

<!-- .slide: style="font-size: 0.7em" -->

## Pre-test answers

```cpp
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
* [Post-test](https://forms.gle/tF9a3Q2gATwnUmWU8)
* [Training evaluation](https://forms.gle/ADXRttpAaZgW8KwM6)

___

## Homework: ping-pong

<div style="display: flex;">

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

<div style="width: 40%; font-size: .9em;">

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

If you get stuck:

* <!-- .element: class="fragment fade-in" --> You need a mutex and a condition variable in your <code>PingPong</code> class
* <!-- .element: class="fragment fade-in" --> Wait for a condition variable with <code>wait_for()</code> in <code>stop()</code> function
* <!-- .element: class="fragment fade-in" --> Check the number of repetitions in ping and pong threads
* <!-- .element: class="fragment fade-in" --> Use an additional <code>std::atomic<bool></code> variable which will tell all threads to end, when the required conditions are met.
* <!-- .element: class="fragment fade-in" --> Ping and pong threads should be using <code>wait()</code> to check if it's their turn to work. Use an additional variable that will be used in the predicate passed to <code>wait()</code>.
* <!-- .element: class="fragment fade-in" --> The pong thread should end the program after reaching the repetition limit

___

### Useful links

* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/async">std::async on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/packaged_task">std::packaged_task on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="http://scottmeyers.blogspot.com/2013/03/stdfutures-from-stdasync-arent-special.html">std::futures from std::async aren't special! - Scott Meyers</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://stackoverflow.com/questions/18143661/what-is-the-difference-between-packaged-task-and-async">The difference between std::async and std::packaged_task</a>
