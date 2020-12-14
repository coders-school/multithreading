<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### `std::async`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### `std::async`

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
void promise_future_approach() {
  std::promise<int> promise;
  std::future<int> future = promise.get_future();
  auto function = [] (std::promise<int> promise) {
    // ...
    promise.set_value(10);
  };
  std::thread t(function, std::move(promise));
  std::cout << future.get() << std::endl;
  t.join();
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
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> <code>#include <future></code>
* <!-- .element: class="fragment fade-in" --> <code>std::async()</code>
* <!-- .element: class="fragment fade-in" --> Wraps a function that can be called asynchronously
* <!-- .element: class="fragment fade-in" --> Returns appropriate <code>std::future<T></code>
* <!-- .element: class="fragment fade-in" --> Handles exceptions through <code>std::promise</code>/<code>std::future</code>
* <!-- .element: class="fragment fade-in" --> Automatically creates exceptions as needed
* <!-- .element: class="fragment fade-in" --> You can choose the type of behavior via the <code>policy</code> (<code>async</code>, <code>differed</code>) parameter

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
  * <!-- .element: class="fragment fade-in" --> scheduler - automatic queuing of tasks performed by implementing the standard library
  * <!-- .element: class="fragment fade-in" --> ability to manually select the type of launch (immediate, asynchronous <code>async</code>, synchronous <code>differed</code>)