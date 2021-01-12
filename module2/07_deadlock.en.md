
## What is the problem with the code below?

<div style="display: flex;">

<div style="width: 90%">

```c++
#include <thread>
#include <mutex>
using namespace std;

class X {
    mutable mutex mtx_;
    int value_ = 0;
public:
    explicit X(int v) : value_(v) {}

    bool operator<(const X & other) const {
        lock_guard<mutex> ownGuard(mtx_);
        lock_guard<mutex> otherGuard(other.mtx_);
        return value_ < other.value_;
    }
};

int main() {
    X x1(5);
    X x2(6);
    thread t1([&](){ x1 < x2; });
    thread t2([&](){ x2 < x1; });
    t1.join();
    t2.join();
    return 0;
}
```
<!-- .element: style="width: 90%; font-size: .4em;" -->
</div>

<div>

<div class="fragment fade-in" style="font-size: 0.55em">

| t1 thread | t2 thread |
| ----------------------------------------------------------------------------------- | -------------------------------------------------- ------------------------------------- |
| `x1 < x2` <!-- .element: class="fragment fade-in" -->                               | `x2 < x1` <!-- .element: class="fragment fade-in" -->                                   |
| x1: mtx_.lock() by ownGuard <!-- .element: class="fragment fade-in" -->             | x2: mtx_.lock() by ownGuard <!-- .element: class="fragment fade-in" -->          |
| x2: mtx_.lock() by otherGuard - waiting <!-- .element: class="fragment fade-in" --> | x1: mtx_.lock() by otherGuard - waiting <!-- .element: class="fragment fade-in" --> |

</div>

### DEADLOCK
<!-- .element: class="fragment fade-in" -->

The deadlock occurs randomly on some executions
<!-- .element: class="fragment fade-in" -->

</div>

</div>

___
<!-- .slide: data-background="img/deadlockbg.png" -->

# Deadlock
<!-- .element style="text-shadow: -2px 2px 0 #000, 2px 2px 0 #000, 2px -2px 0 #000, -2px -2px 0 #000;" -->

<div style="background-color: rgba(139, 53, 54, .85); margin: 400px 0 0 650px; padding: 0px 10px;">

A situation where at least two different threads are waiting for each other, so neither can proceed

</div>

___

## Task: deadlock in comparison

<div style="display: flex;">

<div style="width: 90%">

```c++
#include <thread>
#include <mutex>
using namespace std;

class X {
    mutable mutex mtx_;
    int value_ = 0;
public:
    explicit X(int v) : value_(v) {}

    bool operator<(const X & other) const {
        lock_guard<mutex> ownGuard(mtx_);
        lock_guard<mutex> otherGuard(other.mtx_);
        return value_ < other.value_;
    }
};

int main() {
    X x1(5);
    X x2(6);
    thread t1([&](){ x1 < x2; });
    thread t2([&](){ x2 < x1; });
    t1.join();
    t2.join();
    return 0;
}
```
<!-- .element: style="width: 90%; font-size: .45em;" -->
</div>

<div>

* <!-- .element: class="fragment fade-in" --> The deadlock occurs randomly on some executions
* <!-- .element: class="fragment fade-in" --> Use <code>std::scoped_lock</code> to fix the deadlock

```bash
$> g++ 04_deadlock.cpp –lpthread -fsanitize=thread
$> ./a.out
...
WARNING: ThreadSanitizer: lock-order-inversion
(potential deadlock) (pid=5509)
Cycle in lock order graph: M6
(0x7fffac4d4430) => M7 (0x7fffac4d4460) => M6
...
```
<!-- .element: style="font-size: .4em" class="fragment fade-in" -->
</div>

</div>

___

### Solutions: Deadlock in comparison

<div style="display: flex;">

<div style="width: 100%">

#### original example with deadlock

```cpp
bool operator<(const X & other) const {
    lock_guard<mutex> ownGuard(mtx_);
    lock_guard<mutex> otherGuard(other.mtx_);
    return value_ < other.value_;
}
```
<!-- .element: style="width: 90%; font-size: .5em" -->

#### scoped lock - preferred solution

```cpp
bool operator<(const X & other) const {
    scoped_lock bothLock(mtx_, other.mtx_);
    return value_ < other.value_;
}
```
<!-- .element: style="width: 90%; font-size: .5em" -->
</div>

<div>

#### `std::scoped_lock` (C ++ 17)
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> requires passing mutexes in the constructor, which it tries to lock immediately
* <!-- .element: class="fragment fade-in" --> does not require creating additional lock objects

</div><!-- .element: style="font-size: .98em" -->

</div>

___

### Solutions: Deadlock in comparison

<div style="display: flex;">

<div style="width: 60%">

#### defer unique_lock + lock

```cpp
bool operator<(const X & other) const {
    unique_lock<mutex> l1(mtx_, defer_lock);
    unique_lock<mutex> l2(other.mtx_, defer_lock);
    lock(l1, l2);
    return value_ < other.value_;
}
```
<!-- .element: style="width: 90%; font-size: .5em" -->

#### lock + adopt lock_guard

```cpp
bool operator<(const X & other) const {
    lock(mtx_, other.mtx_);
    lock_guard<mutex> l1(mtx_, adopt_lock);
    lock_guard<mutex> l2(other.mtx_, adopt_lock);
    return value_ < other.value_;
}
```
<!-- .element: style="width: 90%; font-size: .5em" -->

</div>

<div style="width: 40%; font-size: 0.8em">

#### `std::lock()`
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> guarantees that all mutexes will be blocked without deadlock, regardless of the order in which they are obtained
* <!-- .element: class="fragment fade-in" --> requires that delayed locks are passed as parameters - <code>std::unique_lock</code> with <code>defer_lock</code>
* <!-- .element: class="fragment fade-in" --> alternatively it requires passing mutexes and then creating locked locks <code>std::lock_guard</code> with <code>adopt_lock</code>
* <!-- .element: class="fragment fade-in" --> l1 and l2 do not block mutexes in constructors, the <code>std::lock()</code> function does block

</div>

</div>

___

### Deadlock

* <!-- .element: class="fragment fade-in" --> Can occur when we have 2 or more mutexes blocked in different orders
* <!-- .element: class="fragment fade-in" --> Block mutexes everywhere in the same order, but manual reordering does not always fix the problem (as in the example with the <code>operator<</code>)
* <!-- .element: class="fragment fade-in" --> Use <code>std::scoped_lock</code> - a lock that takes multiple mutexes and always locks them in a certain (correct) order
