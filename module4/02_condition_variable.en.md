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
  * <!-- .element: class="fragment fade-in" --> <code>wait()</code> - waiting for a change - blocks the current thread until it receives a notification
  * <!-- .element: class="fragment fade-in" --> <code>notify_one()</code> - awakens one of the threads waiting to change. We have no control over which thread is notified.
  * <!-- .element: class="fragment fade-in" --> <code>notify_all()</code> - awakens all threads waiting for change. These threads may compete for resources.

___
<!-- .slide: style="font-size: .85em" -->

## Exercise: Arctic friendly FIFO queue

Correct the code from `exercises/02_wait_queue.cpp` file. A condition variable should be used instead of busy waiting.


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
<!-- .slide: data-background="img/foczka.jpg" data-background-opacity="0.5" -->

## Seals are happy 🦭

### Thanks condvar!

___

## Condition variable - details

* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable</code> works only with exclusive locks (<code>unique_lock</code>)
* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable_any</code> works with any type of lock (eg. <code>shared_lock</code>)
* <!-- .element: class="fragment fade-in" --> They are non-copyable, but movable
* <!-- .element: class="fragment fade-in" --> The <code>wait()</code> method takes a lock and optionally a predicate, so only threads for which the condition is met will be woken up
* <!-- .element: class="fragment fade-in" --> All threads waiting on the condition variable must have the same mutex locked. Otherwise, the behavior is undefined.
* <!-- .element: class="fragment fade-in" --> The <code>wait_for()</code> and <code>wait_until()</code> methods take respectively a period of time or a point in time, until which the threads wait to wake up. After that, the threads will wake up.
* <!-- .element: class="fragment fade-in" --> If several threads are waiting on the condition variable and each has a different predicate, using <code>notify_one()</code> may cause a deadlock. A thread for which the condition has not been met may wake up, and if no other thread calls <code>notify_one()</code> or <code>notify_all()</code> they all will be waiting.

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

* <!-- .element: class="fragment fade-in" --> Condition variables are used to sychronize threads multiple times. The future/promise mechanism is for one-time synchronization.
* <!-- .element: class="fragment fade-in" --> Condition variables are more difficult to use. There are many details that you need to be aware of.
* <!-- .element: class="fragment fade-in" --> Condition variables do not support exception forwarding. The future/promise mechanism allows it.
* <!-- .element: class="fragment fade-in" --> General advice - use future/promise whenever possible (better: async/future). Only use condition_variable if multiple synchronization is required.
