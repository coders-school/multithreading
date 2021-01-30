<!-- .slide: data-background="#111111" -->

# How exactly does it work?

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [14-21]
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

<div data-id="pop1" class="consumer-active">
    Consumer A
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [15]
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
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [16-17]
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
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [15]
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [16-17]
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
    <div data-id="elB" class="element">B</div>
</div>
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

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [9-13]
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el1" class="element">Element 1</div>
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
    <div data-id="elB" class="element">B</div>
</div>

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

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [10]
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
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
    <div data-id="elB" class="element">B</div>
</div>

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

## How exactly does it work?

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
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
    <div data-id="elB" class="element">B</div>
</div>

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

## How exactly does it work?

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
<div data-id="condvar" class="condvar-notified">
    Condvar
    <div data-id="elA" class="element">A</div>
    <div data-id="elB" class="element">B</div>
</div>

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

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [15,17]
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
<div data-id="condvar" class="condvar">
    Condvar
</div>

<div data-id="pop1" class="consumer-active">
    Consumer A
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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
<div data-id="pop1" class="consumer-active">
    Consumer B
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [18-20]
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
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [10]
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el2" class="element">Element 2</div>
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">Condvar</div>

<div data-id="pop1" class="consumer-active">
    Consumer A
</div>
<div data-id="pop1" class="consumer-active">
    Consumer B
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [21]
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el2" class="element">Element 2</div>
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">Condvar</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer-active">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el2" class="element">Element 2</div>
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">Condvar</div>

<div data-id="pop1" class="consumer-active">
    Consumer A
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="el2" class="element">Element 2</div>
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

<div style="display: flex;">

<div style="width: 60%;">

```cpp [10]
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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
    <div data-id="el2" class="element">Element 2</div>
</div>

<div data-id="queue" class="queue">
    Queue
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el2" class="element">Element 2</div>
</div>
<div data-id="condvar" class="condvar">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer-active">
    Producer
    <div data-id="mutex" class="mutex-locked">Mutex</div>
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el2" class="element">Element 2</div>
</div>
<div data-id="condvar" class="condvar-notified">
    Condvar
    <div data-id="elA" class="element">A</div>
</div>

<div data-id="pop1" class="consumer">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___
<!-- .slide: style="font-size: .75em" data-auto-animate -->

## How exactly does it work?

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

</div>

<div id="animation" style="width: 40%; padding: 15px;">

<div data-id="push" class="producer">
    Producer
</div>

<div data-id="queue" class="queue">
    Queue
    <div data-id="el2" class="element">Element 2</div>
</div>
<div data-id="condvar" class="condvar">Condvar</div>
<div data-id="mutex" class="mutex">Mutex</div>

<div data-id="pop1" class="consumer-active">
    Consumer A
</div>
<div data-id="pop1" class="consumer">
    Consumer B
    <div data-id="el1" class="element">Element 1</div>
</div>

</div>

</div>

___

## Condition variable - details

* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable</code> works only with exclusive locks (<code>unique_lock</code>)
* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable_any</code> works with any type of lock (eg. <code>shared_lock</code>)
* <!-- .element: class="fragment fade-in" --> They are non-copyable, but movable
* <!-- .element: class="fragment fade-in" --> The <code>wait()</code> method takes a lock and optionally a predicate.
* <!-- .element: class="fragment fade-in" --> All threads waiting on the condition variable must have the same mutex locked. Otherwise, the behavior is undefined.
* <!-- .element: class="fragment fade-in" --> The <code>wait_for()</code> and <code>wait_until()</code> methods take respectively a period of time or a point in time, until which the threads wait to wake up. After that, threads will wake up.
* <!-- .element: class="fragment fade-in" --> If several threads are waiting on the condition variable and each has a different predicate, using <code>notify_one()</code> may cause a deadlock. A thread for which the condition has not been met may wake up, and if no other thread calls <code>notify_one()</code> or <code>notify_all()</code> they all will be waiting.
