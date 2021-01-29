<!-- .slide: data-background="#111111" -->

# Thread-safe queue

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Exercise: FIFO queue

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
template <typename T>
class ThreadsafeQueue {
    deque<T> queue_;
    // TODO: Make it thread-safe :)

public:
    void push(const T & element) {
        queue_.push_front(element);
    }
    T pop() {
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        return queue_.empty();
    }
};

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; padding: 20px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> In the file <code>01_threadsafe_queue.cpp</code> there is a missing FIFO queue implementation
* <!-- .element: class="fragment fade-in" --> Fix the <code>textProducer</code> thread to generate correct strings:
  * <!-- .element: class="fragment fade-in" --> This is random text number 0
  * <!-- .element: class="fragment fade-in" --> This is random text number 1
  * <!-- .element: class="fragment fade-in" --> …
  * <!-- .element: class="fragment fade-in" --> This is random text number n
  * <!-- .element: class="fragment fade-in" --> Prevent queue operations from being accessed by multiple threads (make it thread-safe 🙂)
  * <!-- .element: class="fragment fade-in" --> What problem do you see?

</div>

</div>

___

## Exercise - solution

```c++
void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number "
                + i);
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " +
                to_string(i));
}

```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .9em" -->

## Exercise - solution: thread safe queue

```c++
template <typename T>
class ThreadsafeQueue {
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

## Problem - CPU heating and the greenhouse effect

<img data-src="img/efekt_cieplarniany.jpg" alt="efekt cieplarniany" class="plain">

___

## Busy waiting

```c++
void saveToFile(StringQueue & sq) {
    ofstream file("/tmp/sth.txt");
    while (file) {
        while (sq.empty()) { /* nop */ }
        file << sq.pop() << endl;
    }
}
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Busy waiting is a state in which the thread is constantly checking if a certain condition has been met
* <!-- .element: class="fragment fade-in" --> Another name for this problem is spinlock
* <!-- .element: class="fragment fade-in" --> The problem is solved by the condition variable

<img data-src="img/aktywne_czekanie.png" alt="aktywne_czekanie" class="plain">
<!-- .element: class="fragment fade-in" -->
