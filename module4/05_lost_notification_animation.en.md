<!-- .slide: data-background="#111111" -->

# Lost notification

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp []
template <typename T>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    condition_variable nonEmpty_;
    using Lock = unique_lock<mutex>;

public:
    void push(const T & element) {
        {
            Lock l(m_);
            queue_.push_front(element);
        }
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="condvar" class="condvar">Condvar</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
</div>

</div>

</div>

___

## Lost notification

* <!-- .element: class="fragment fade-in" --> If the notification was sent before the thread started waiting on the <code>condition_variable</code>, it is lost and will not wake the thread
* <!-- .element: class="fragment fade-in" --> A spurious wake-up may help to unlock the thread
* <!-- .element: class="fragment fade-in" --> If the thread was waiting on a <code>condition_variable</code> with a predicate, then the predicate must be met to resume the threads' job

___

## Summary

* <!-- .element: class="fragment fade-in" --> Condition variables are used to sychronize threads multiple times. The future/promise mechanism is for one-time synchronization.
* <!-- .element: class="fragment fade-in" --> Condition variables are more difficult to use. There are many details that you need to be aware of.
* <!-- .element: class="fragment fade-in" --> Condition variables do not support exception forwarding. The future/promise mechanism allows it.
* <!-- .element: class="fragment fade-in" --> General advice - use future/promise whenever possible (better: async/future). Only use condition_variable if multiple synchronization is required.
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/condition-variables">Spurious wakeup and lost wakeup - article on modernescpp.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/thread-synchronization-with-condition-variables-or-tasks">Condition variables vs tasks - article on modernescpp.com</a>
