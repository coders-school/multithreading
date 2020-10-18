<!-- .slide: data-background="#111111" -->

# Auxiliary functions

___

## Selection of the number of threads during implementation

* <!-- .element: class="fragment fade-in" --> Too many threads - the program runs slower,
* <!-- .element: class="fragment fade-in" --> Too few threads - no use of potential,
* <!-- .element: class="fragment fade-in" --> <code>std::thread::hardware_concurrency()</code> - returns the number of available concurrent threads. This function can return 0 if such information is not obtainable.

___

## Thread identification

* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::get_id()</code> - returns an object of <code>std::thread::id</code> type representing the id of the current thread

```cpp
std::thread::id master_thread;
void some_core_part_of_the_algorithm()
{
    if (std::this_thread::get_id() == master_thread) {
        do_master_thread_work();
    }
    do_common_work();
}
```
<!-- .element: class="fragment fade-in" -->

___

## Putting threads to sleep

* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::sleep_until(const chrono::time_point<Clock, Duration>& sleep_time)</code>
  * <!-- .element: class="fragment fade-in" -->blocks execution of the thread at least until the time point specified as a parameter
* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::sleep_for(const chrono::duration<Rep, Period>& sleep_duration)</code>
  * <!-- .element: class="fragment fade-in" --> suspends execution of the current thread for (at least) the specified time interval
* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::yield()</code>
  * <!-- .element: class="fragment fade-in" --> function to try to yield the current thread and allocate CPU time to another thread

___

## Putting threads to sleep

```cpp
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    using namespace std::chrono_literals;
    std::cout << "Hello waiter\n" << std::flush;

    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(2s);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Waited " << elapsed.count() << " ms\n";
}
```

Possible output:
<!-- .element: class="fragment fade-in" -->

```output
Hello waiter
Waited 2000.12 ms
```
<!-- .element: class="fragment fade-in" -->
