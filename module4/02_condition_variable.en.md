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


```cpp []
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

```cpp
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

```cpp
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
