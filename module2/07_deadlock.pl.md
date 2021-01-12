
## Jaki jest problem z poniższym kodem?

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

<div class="fragment fade-in" style="font-size: 0.55em">

| wątek t1                                                                            | wątek t2                                                                              |
| ----------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
| x1 < x2 <!-- .element: class="fragment fade-in" -->                                 | x2 < x1 <!-- .element: class="fragment fade-in" -->                                   |
| x1: mtx_.lock() by ownGuard <!-- .element: class="fragment fade-in" -->             | x2: mtx_.lock() by ownGuard <!-- .element: class="fragment fade-in" -->               |
| x2: mtx_.lock() by otherGuard - waiting <!-- .element: class="fragment fade-in" --> | x1: mtx_.lock() by otherGuard - waiting <!-- .element: class="fragment fade-in" --> |

</div>

### DEADLOCK
<!-- .element: class="fragment fade-in" -->

Zakleszczenie występuje losowo przy niektórych uruchomieniach
<!-- .element: class="fragment fade-in" -->

</div>

</div>

___
<!-- .slide: data-background="img/deadlockbg.png" -->

# Zakleszczenie (deadlock)
<!-- .element style="text-shadow: -2px 2px 0 #000, 2px 2px 0 #000, 2px -2px 0 #000, -2px -2px 0 #000;" -->

<div style="background-color: rgba(139, 53, 54, .85); margin: 400px 0 0 650px; padding: 0px 10px;">

Sytuacja, w której co najmniej dwa różne wątki czekają na siebie nawzajem, więc żaden nie może się zakończyć

</div>

___

## Zadanie: zakleszczenie w porównaniu

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

* <!-- .element: class="fragment fade-in" --> Zakleszczenie występuje losowo przy niektórych uruchomieniach
* <!-- .element: class="fragment fade-in" --> Użyj <code>std::scoped_lock</code> do rozwiązania problemu zakleszczenia

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

### Rozwiązania: Zakleszczenie w porównaniu

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

#### scoped lock – preferred solution

```cpp
bool operator<(const X & other) const {
    scoped_lock bothLock(mtx_, other.mtx_);
    return value_ < other.value_;
}
```
<!-- .element: style="width: 90%; font-size: .5em" -->
</div>

<div>

#### `std::scoped_lock` (C++17)
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> wymaga przekazania mutexów w konstruktorze, które od razu próbuje zablokować
* <!-- .element: class="fragment fade-in" --> nie wymaga tworzenia dodatkowych obiektów blokad

</div><!-- .element: style="font-size: .98em" -->

</div>

___

### Rozwiązania: Zakleszczenie w porównaniu

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

* <!-- .element: class="fragment fade-in" --> gwarantuje zablokowanie wszystkich mutexów bez zakleszczenia niezależnie od kolejności ich pozyskiwania
* <!-- .element: class="fragment fade-in" --> wymaga przekazania jako parametrów opóźnionych blokad <code>(defer_lock)</code> typu <code>std::unique_lock</code>
* <!-- .element: class="fragment fade-in" --> alternatywnie wymaga przekazania mutexów, a następnie utworzenia zablokowanych blokad <code>(adopt_lock)</code> typu <code>std::lock_guard</code>
* <!-- .element: class="fragment fade-in" --> l1 i l2 nie blokują mutexów w konstruktorach, robi to funkcja <code>std::lock()</code>

</div>

</div>

___

### Zakleszczenie

* <!-- .element: class="fragment fade-in" --> Może wystąpić gdy mamy 2 lub więcej mutexów blokowanych w różnych kolejnościach
* <!-- .element: class="fragment fade-in" --> Blokuj najpierw mutexy wszędzie w tej samej kolejności, ale ręczna zmiana kolejności <code>lock_guard</code>'ów nie zawsze naprawi program (jak na przykładzie z operatorem <code><</code>)
* <!-- .element: class="fragment fade-in" --> Używaj <code>std::scoped_lock</code> - blokady, która przyjmuje kilka mutexów i blokuje je zawsze w określonej (prawidłowej) kolejności
