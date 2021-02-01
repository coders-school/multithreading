<!-- .slide: data-background="#111111" -->

# Spurious wake-up

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

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
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [19]
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

<div data-id="pop1" class="consumer-active">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [18-19]
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

<div data-id="pop1" class="consumer-active">
    Consumer A
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>
<div data-id="pop1" class="consumer">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [20-22]
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
</div>
<div data-id="condvar" class="condvar">Condvar</div>

<div data-id="pop1" class="consumer-active">
    Consumer A
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="pop1" class="consumer">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [23]
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
<!-- .element data-id="wait_code" -->

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">Condvar</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer">
    Consumer A
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="pop1" class="consumer">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [18]
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
        nonEmpty_.wait(l);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
};
```
<!-- .element data-id="wait_code" -->

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">Condvar</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer">
    Consumer A
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [17-18]
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
        nonEmpty_.wait(l);
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
</div>
<div data-id="condvar" class="condvar">Condvar</div>

<div data-id="pop1" class="consumer">
    Consumer A
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [19-21]
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
        nonEmpty_.wait(l);
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
</div>
<div data-id="condvar" class="condvar">Condvar</div>

<div data-id="pop1" class="consumer">
    Consumer A
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el2" class="element-screwed">???</div>
</div>

### Undefined Behavior

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## Spurious wake-up

<div class="multicolumn">

<div style="width: 60%;">

```cpp [22]
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
        nonEmpty_.wait(l);
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
</div>
<div data-id="condvar" class="condvar">Condvar</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer">
    Consumer A
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el2" class="element-screwed">???</div>
</div>

### Undefined Behavior

</div>

</div>

___

## Spurious wake-up

* <!-- .element: class="fragment fade-in" --> The thread waiting on the <code>condition_variable</code> periodically wakes up from time to time.
* <!-- .element: class="fragment fade-in" --> A <code>unique_lock</code> is required to wait on a condition variable. During sleep, the thread unlocks it, and when it wakes up, it locks it again.
* <!-- .element: class="fragment fade-in" --> Predicate added to the <code>wait()</code> function makes spurious wake-ups not dangerous.
  * <!-- .element: class="fragment fade-in" --> Only when the condition is met, the thread will continue its work.
  * <!-- .element: class="fragment fade-in" --> If the condition is not met, the thread releases the lock and goes to sleep again.
* <!-- .element: class="fragment fade-in" --> <b>ALWAYS</b> use <code>wait()</code> with predicate to avoid potential problems.