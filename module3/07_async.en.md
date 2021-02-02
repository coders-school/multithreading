<!-- .slide: data-background="#111111" -->

# Asynchronous tasks

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## promise/future vs async approach

```c++
void promise_future_approach() {
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();
  auto function = [] (std::promise<int> prom) {
    // ...
    prom.set_value(10);
  };
  std::thread t(function, std::move(prom));
  t.detach();
  std::cout << fut.get() << std::endl;
}
```

```cpp
void async_approach () {
  auto function = [] () {
    // ...
    return 20;
  };
  std::future<int> fut = std::async(function);
  std::cout << fut.get() << std::endl;
}
```
<!-- .element: class="fragment fade-in" -->

___

## `std::async`

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;future&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::async()</code>
* <!-- .element: class="fragment fade-in" --> Wraps a function that can be called asynchronously
* <!-- .element: class="fragment fade-in" --> Returns appropriate <code>std::future<T></code>
* <!-- .element: class="fragment fade-in" --> Handles exceptions through <code>std::promise</code>/<code>std::future</code>
* <!-- .element: class="fragment fade-in" --> Automatically throws exceptions as needed

```bash
$> ./02_async
10
20
```
<!-- .element: class="fragment fade-in" -->
</div> <!-- .element: class="fragment fade-in" -->

<div>

___

## `std::async`

* <!-- .element: class="fragment fade-in" --> <code>std::async</code> is a high-level solution (finally!) that automatically manages asynchronous calls with basic synchronization mechanisms
* <!-- .element: class="fragment fade-in" --> The most convenient form of launching tasks:
  * <!-- .element: class="fragment fade-in" --> handling return values
  * <!-- .element: class="fragment fade-in" --> exception handling
  * <!-- .element: class="fragment fade-in" --> synchronization (blocking <code>get()</code> and <code>wait()</code> on <code>std::future</code>)
  * <!-- .element: class="fragment fade-in" --> scheduler - automatic queuing of tasks performed by implementation of the standard library
  * <!-- .element: class="fragment fade-in" --> ability to manually select the type of launch (immediate, asynchronous <code>async</code>, synchronous <code>deferred</code>)

___

## Exercise

### `exercises/03_exceptions.cpp`

Simplify the code by using async instead of threads.

```cpp
#include <thread>
#include <iostream>
#include <string>
#include <random>
using namespace std;

std::random_device rd;

int main() {
    std::exception_ptr thread_exception = nullptr;
    std::string result;

    auto task = [](std::exception_ptr & te, std::string & result) {
        try {
            std::mt19937 gen(rd());
            std::bernoulli_distribution d(0.5);
            if (d(gen)) {
                throw std::runtime_error("WTF");
            } else {
                result = "success";
            }
        } catch (...) {
            te = std::current_exception();
        }
    };

    std::thread t(task, std::ref(thread_exception), std::ref(result));

    std::cout << "Some heave task on main thread\n";
    std::this_thread::sleep_for(1s);

    t.join();

    if (thread_exception) {
        try {
            std::rethrow_exception(thread_exception);
        } catch (const std::exception & ex) {
            std::cout << "Task exited with an exception: " << ex.what() << "\n";
        }
    } else {
        std::cout << "Task exited normally with result: " << result << '\n';
    }
    return 0;
}
```
<!-- .element: style="font-size: 0.4em" -->

___

## Solution

```cpp
#include <iostream>
#include <string>
#include <random>
#include <future>
using namespace std;

std::random_device rd;

int main() {
    auto task = []() {
        std::mt19937 gen(rd());
        std::bernoulli_distribution d(0.5);
        if (d(gen)) {
            throw std::runtime_error{"WTF"};
        } else {
            return "success";
        }
    };

    auto result = std::async(std::launch::async, task);

    std::cout << "Some heave task on main thread\n";
    std::this_thread::sleep_for(1s);

    try {
        auto value = result.get();
        std::cout << "Task exited normally with result: " << value << '\n';
    } catch (const std::exception & ex) {
        std::cout << "Task exited with an exception: " << ex.what() << "\n";
    }
    return 0;
}
```
<!-- .element: class="fragment fade-in" style="font-size: 0.4em" -->

___

## Drawbacks of `async`

It may fail due to resource exhaustion, rather than queuing up tasks to be executed later.
It does not use a thread pool to reschedule a failed task.
