<!-- .slide: data-background="#111111" -->

# Asynchronous tasks

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## promise/future vs async approach

```c++
void promise_future_approach() {
  std::promise<int> promise;
  std::future<int> future = promise.get_future();
  auto function = [] (std::promise<int> promise) {
    // ...
    promise.set_value(10);
  };
  std::thread t(function, std::move(promise));
  t.detach();
  std::cout << future.get() << std::endl;
}

void async_approach () {
  auto function = [] () {
    // ...
    return 20;
  };
  std::future<int> future = std::async(function);
  std::cout << future.get() << std::endl;
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

### `std::async`

* <!-- .element: class="fragment fade-in" --> <code>std::async</code> is a high-level solution (finally!) that automatically manages asynchronous calls with basic synchronization mechanisms
* <!-- .element: class="fragment fade-in" --> The most convenient form of launching tasks:
  * <!-- .element: class="fragment fade-in" --> handling return values
  * <!-- .element: class="fragment fade-in" --> exception handling
  * <!-- .element: class="fragment fade-in" --> synchronization (blocking <code>get()</code> and <code>wait()</code> on <code>std::future</code>)
  * <!-- .element: class="fragment fade-in" --> scheduler - automatic queuing of tasks performed by implementation of the standard library
  * <!-- .element: class="fragment fade-in" --> ability to manually select the type of launch (immediate, asynchronous <code>async</code>, synchronous <code>deferred</code>)

___

## Launch policies - example

<div style="display: flex;">

<div style="width: 45%; font-size: .9em;">

```c++
#include <chrono>
#include <future>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    auto f1 = async([]{
        cout << "f1 started\n";
        this_thread::sleep_for(1s);
        return 42;
    });
    cout << "f1 spawned\n";

    auto f2 = async(launch::async, []{
        cout << "f2 started\n";
        this_thread::sleep_for(1s);
        return 2 * 42;
    });
    cout << "f2 spawned\n";

    auto f3 = async(launch::deferred, []{
        cout << "f3 started\n";
        this_thread::sleep_for(1s);
        return 3 * 42;
    });
    cout << "f3 spawned\n";

    cout << "Getting f1 result\n";
    auto v1 = f1.get();
    cout << "Got f1 result\n";

    cout << "Getting f2 result\n";
    auto v2 = f2.get();
    cout << "Got f2 result\n";

    cout << "Getting f3 result\n";
    auto v3 = f3.get();
    cout << "Got f3 result\n";

    vector<int> numbers = {v1, v2, v3};
    for (const auto& item : numbers)
        cout << item << '\n';

    return 0;
}
```
<!-- .element: class="fragment fade-in" style="font-size: .5em;" -->

</div>

<div style="width: 55%; padding: 20px; font-size: .85em;">

* <!-- .element: class="fragment fade-in" --> Launch examples/04_async_policies
* <!-- .element: class="fragment fade-in" --> Look at the source code
* <!-- .element: class="fragment fade-in" --> Launch examples/05_async_ids
* <!-- .element: class="fragment fade-in" --> Experiment with launch policies settings
* <!-- .element: class="fragment fade-in" --> Observe how do programs work
* <!-- .element: class="fragment fade-in" --> Draw conclusions :)

```bash
$> ./04_async_policies
f1 spawned
f1 started
f2 spawned
f3 spawned
Getting f1 result
f2 started
Got f1 result
Getting f2 result
Got f2 result
Getting f3 result
f3 started
Got f3 result
42
84
126
```
<!-- .element: class="fragment fade-in" style="font-size: 0.5em;" -->
</div>

<div>

___

## Drawbacks of `async`

It doesn’t use a thread pool.

It may fail due to resource exhaustion, rather than queuing up tasks to be executed later.