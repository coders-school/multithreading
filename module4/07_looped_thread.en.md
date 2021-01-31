<!-- .slide: data-background="#111111" -->

# Looped thread

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .85em" -->

## How to stop a looped thread?

<div class="multicolumn">

<div style="width: 70%;">

```cpp []
#include <thread>
using namespace std;

int main() {
    bool stop = false;
    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

</div>

<div>

<img data-src="img/epic_fail.jpg" alt="epic fail" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

</div>

```bash
$> g++ 01_stop.cpp -lpthread -fsanitize=thread
$> ./a.out
WARNING: ThreadSanitizer: data race (pid=10179)
...
$> g++ 01_stop.cpp -lpthread -fsanitize=thread -03
$> ./a.out
deadlock
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .85em" -->

## How to stop a looped thread?

### volatile?

<div class="multicolumn">

<div style="width: 70%;">

```cpp []
#include <thread>
using namespace std;

int main() {
    volatile bool stop = false;
    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

</div>

<div>

<img data-src="img/fail.jpg" alt="fail" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

</div>

```bash
$> g++ 01b_volatile.cpp -lpthread -fsanitize=thread
$> ./a.out
WARNING: ThreadSanitizer: data race (pid=10179)
...
$> g++ 01b_volatile.cpp -lpthread -fsanitize=thread -03
$> ./a.out
WARNING: ThreadSanitizer: data race (pid=10179)
...
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .7em" -->

## How to stop a looped thread?

### Variable with mutex?

<div class="multicolumn">

<div style="width: 50%;">

```cpp []
#include <thread>
#include <mutex>

using namespace std;

int main() {
    bool flag = false;
    mutex m;

    auto stop = [&] {
        lock_guard<mutex> lg(m);
        return flag;
    };

    auto f = [&] {
        while (not stop()) {
            /* do sth... */
        }
    };
    thread t(f);
    {
        lock_guard<mutex> lg(m);
        flag = true;
    }
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

</div>

<div style="width: 50%;">

<img data-src="img/spider_pig.jpg" alt="spider_pig" class="plain" style="width: 80%">
<!-- .element: class="fragment fade-in" -->


```bash
$> g++ 01c_mutex.cpp -lpthread -fsanitize=thread
$> ./a.out
$> g++ 01c_mutex.cpp -lpthread -fsanitize=thread -03
$> ./a.out
```
<!-- .element: class="fragment fade-in" -->

</div>

</div>

___
<!-- .slide: style="font-size: .9em" -->

## How to stop a looped thread?

### atomic variable

<div class="multicolumn">

<div style="width: 50%;">

```cpp []
#include <thread>
#include <atomic>
using namespace std;

int main() {
    atomic<bool> stop{false};
    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

</div>

<div style="width: 50%;">

<img height="400px" data-src="img/perfect.jpeg" alt="perfect" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

</div>

```bash
$> g++ 01d_atomic.cpp -lpthread -fsanitize=thread -03
$> ./a.out
```
<!-- .element: class="fragment fade-in" -->
