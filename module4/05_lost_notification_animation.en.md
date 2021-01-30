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

```cpp [9-15]
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

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el1" class="element">Element 1</div>
</div>

<div data-id="queue" class="queue">
    Queue

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

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp [11]
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

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el1" class="element">Element 1</div>
</div>

<div data-id="queue" class="queue">
    Queue

</div>
<div data-id="condvar" class="condvar">Condvar</div>

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

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp [12]
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

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="condvar" class="condvar">Condvar</div>

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

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp [13]
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

<div data-id="push" class="producer-active">
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

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp [14]
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

<div data-id="push" class="producer-active">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="condvar" class="condvar-notified">Condvar</div>
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

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp [16-22]
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

## Lost notification

<div style="display: flex;">

<div style="width: 60%;">

```cpp [17]
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

## Lost notification

<div style="display: flex;">

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

## Lost notification

<div style="display: flex;">

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

### deadlock...

#### or...
<!-- .element: class="fragment fade-in" -->
### spurious wake-up to the rescue!
<!-- .element: class="fragment fade-in" -->

</div>

</div>

___

## `wait()` with predicate scenario

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [9-15]
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

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el1" class="element">Element 1</div>
</div>

<div data-id="queue" class="queue">
    Queue

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

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [11]
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

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el1" class="element">Element 1</div>
</div>

<div data-id="queue" class="queue">
    Queue

</div>
<div data-id="condvar" class="condvar">Condvar</div>

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

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [12]
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

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="condvar" class="condvar">Condvar</div>

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

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [13]
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

<div data-id="push" class="producer-active">
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

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [14]
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

<div data-id="push" class="producer-active">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el1" class="element">Element 1</div>
</div>
<div data-id="condvar" class="condvar-notified">Condvar</div>
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

## `wait()` with predicate scenario

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
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [16-23]
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

## `wait()` with predicate scenario

<div style="display: flex;">

<div style="width: 60%;">

```cpp [17]
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

## `wait()` with predicate scenario

<div style="display: flex;">

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

### `hasData == true`

### No waiting

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## `wait()` with predicate scenario

<div style="display: flex;">

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

## `wait()` with predicate scenario

<div style="display: flex;">

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

## Lost notification

* <!-- .element: class="fragment fade-in" --> If the notification was sent before the thread started waiting on the <code>condition_variable</code>, it is lost and will not wake the thread
* <!-- .element: class="fragment fade-in" --> A spurious wake-up may help to unlock such thread
* <!-- .element: class="fragment fade-in" --> If the thread was waiting on a <code>condition_variable</code> with a predicate, then the predicate must be met to resume the threads' job
* <!-- .element: class="fragment fade-in" --> In case the predicate is true, the receiver is able to continue its work independently of the notification of the sender
  * (condition == true) => notification was delivered

___

## Summary

* <!-- .element: class="fragment fade-in" --> Condition variables are used to sychronize threads multiple times. The future/promise mechanism is for one-time synchronization.
* <!-- .element: class="fragment fade-in" --> Condition variables are more difficult to use. There are many details that you need to be aware of.
* <!-- .element: class="fragment fade-in" --> Condition variables do not support exception forwarding. The future/promise mechanism allows it.
* <!-- .element: class="fragment fade-in" --> General advice - use future/promise whenever possible (better: async/future). Use <code>condition_variable</code> only if multiple synchronization is required.
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/condition-variables">Spurious wakeup and lost wakeup - article on modernescpp.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/thread-synchronization-with-condition-variables-or-tasks">Condition variables vs tasks - article on modernescpp.com</a>
