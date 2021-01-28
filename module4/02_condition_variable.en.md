<!-- .slide: data-background="#111111" -->

# Condition variable

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Condition variable

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;condition_variable&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable</code>
* <!-- .element: class="fragment fade-in" --> Key operations
  * <!-- .element: class="fragment fade-in" --> <code>wait()</code> - waiting for a change - blocks the current thread until it wakes up
  * <!-- .element: class="fragment fade-in" --> <code>notify_one()</code> - wakes up one of the threads waiting for change. We have no control over which thread is notified.
  * <!-- .element: class="fragment fade-in" --> <code>notify_all()</code> - awakens all threads waiting for change. These threads may compete for resources.

___
<!-- .slide: style="font-size: .9em" -->

## Exercise: Arctic friendly FIFO queue

Correct code from file `02_wait_queue.cpp` so that it uses a conditional variable instead of busy waiting:
<!-- .element: class="fragment fade-in" -->

```c++
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
```
<!-- .element: class="fragment fade-in" -->

___

## Exercise - solution

<div style="display: flex;">

<div style="width: 50%; font-size: .8em;">

```c++
// includes
template <typename T>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    condition_variable nonEmpty_;
    using Lock = unique_lock<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
        nonEmpty_.notify_all();
    }
    T pop() {
        Lock l(m_);
        auto hasData = [&]{ return not queue_.empty(); };
        nonEmpty_.wait(l, hasData);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
};
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; font-size: .8em;">

```c++
using StringQueue = WaitQueue<string>;

void provideData(StringQueue & sq) {
    string txt;
    while (getline(cin, txt))
        sq.push(txt);
}

void saveToFile(StringQueue & sq) {
    ofstream file("/tmp/sth.txt");
    while (file)
        file << sq.pop() << endl;
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " +
                to_string(i));
}

int main() {
    // without changes
}
```

</div> <!-- .element: class="fragment fade-in" -->

</div>

___

## Condition variables make seals happy 🙂

<img data-src="img/foczka.jpg" alt="foczka" class="plain">

___

## Condition variable - details

* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable</code> only works with exclusive locks (<code>unique_lock</code>)
* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable_any</code> works with any type of lock (<code>shared_lock</code>)
* <!-- .element: class="fragment fade-in" --> They cannot be copied, so they are mostly class fields to avoid passing them
* <!-- .element: class="fragment fade-in" --> Method <code>wait()</code> takes a lock and optionally a predicate, so only those threads for which the condition is met will be woken up
* <!-- .element: class="fragment fade-in" --> All threads waiting for the condition variable must have the same mutex locked. Otherwise, the behavior is undefined.
* <!-- .element: class="fragment fade-in" --> Methods <code>wait_for()</code> and <code>wait_until()</code> take a point in time or a period of time, respectively, until which the threads wait to wake up. After that, the threads will wake up.
* <!-- .element: class="fragment fade-in" --> If several threads are waiting on the condition variable and each has a different predicate, using  <code>notify_one()</code> may cause a jam. A thread for which the condition has not been met may wake up, and if no other thread calls <code>nofity_one()</code> or <code>notify_all()</code> they will all be waiting.

___

## Homework: ping-pong

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> First thread prints "ping" and the next number
* <!-- .element: class="fragment fade-in" --> Second thread prints "pong" and the next number
* <!-- .element: class="fragment fade-in" --> Ping thread starts and the pong thread always ends. Threads must work in turns. There may not be 2 pings or pongs in succession. The program cannot end with a ping that will not be answered with pong.
* <!-- .element: class="fragment fade-in" --> The program is to terminate either after the number of hits or after the time limit, whichever occurs first. The reason for termination should be displayed on the screen
* <!-- .element: class="fragment fade-in" --> Program parameters:
  * <!-- .element: class="fragment fade-in" --> number of bounces
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
* <!-- .element: class="fragment fade-in" --> Use an additional <code>bool</code> variable which will tell all threads to end, when the required conditions are met. Use <code>atomic<bool></code> type here (we'll talk later about it 🙂)
* <!-- .element: class="fragment fade-in" --> Ping and pong threads should be using <code>wait()</code> to check if it's their turn to work. Use an additional <code>bool</code> variable that will be used in the predicate passed to <code>wait()</code>.
* <!-- .element: class="fragment fade-in" --> The pong thread should end the program after reaching the bounce limit

___

## Condition variable - threats

* <!-- .element: class="fragment fade-in" --> Spurious wakeup
  * <!-- .element: class="fragment fade-in" --> The thread waiting for a variable condition cyclically wakes up from time to time and checks whether notification has come or not
  * <!-- .element: class="fragment fade-in" --> At least a <code>unique_lock</code> is required to wait on a condition variable because when sleeping, the thread unlocks it, and when it wakes up to check the notification, it locks it again for a while, then unlocks it again and continues to sleep
  * <!-- .element: class="fragment fade-in" --> Predicate added to the <code>wait()</code> function prevents false awakenings as it adds an additional condition that must be met for the thread to wake up
* <!-- .element: class="fragment fade-in" --> Lost wakeup
  * <!-- .element: class="fragment fade-in" --> If the notification was sent before the thread waited for the variable, it is lost and will not wake the thread
  * <!-- .element: class="fragment fade-in" --> A workaround can be done when a false wakeup occurs
  * <!-- .element: class="fragment fade-in" --> If the thread was waiting on a condition variable with a predicate, then the predicate must be true, otherwise no false wake will occur
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/condition-variables">Spurious wakeup and lost wakeup - article on modernescpp.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/thread-synchronization-with-condition-variables-or-tasks">Condition variables vs tasks - article on modernescpp.com</a>

___

## Summary

* <!-- .element: class="fragment fade-in" --> Condition variables are used to sychronize threads multiple times. The future and promise mechanism is for one-time synchronization.
* <!-- .element: class="fragment fade-in" --> Condition variables are much more difficult to use. There are many details that need to be protected against.
* <!-- .element: class="fragment fade-in" --> Condition variables do not support exception throwing. The future/promise mechanism allows it.
* <!-- .element: class="fragment fade-in" --> General advice - use future/promise whenever possible (better: async/future). Only use condition_variable if multiple synchronization is required.
