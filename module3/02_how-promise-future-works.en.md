<!-- .slide: data-background="#111111" -->

# One way communication

## `std::promise`

## `std::future`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- TODO: Update to 4.1.0 and use auto-animate -->

### How `std::promise`/`std::future` works

<img data-src="img/how_promise_future_works.gif" class="plain" style="
    position: relative;
    left: 50%;
    transform: translateX(-50%);
">

___

### Basic `std::promise`/`std::future` usage

```c++
std::promise<int> promise;
std::future<int> future = promise.get_future();     // connected pair
auto function = [] (std::promise<int> promise) {
    // ...
    promise.set_value(10);
};
std::thread t(function, std::move(promise));
std::cout << future.get() << std::endl;
t.join();
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> pair is used to create <span style="color:#cf802a;">one-way communication channel</span>
* <!-- .element: class="fragment fade-in" --> <code>std::promise</code> is used for <span style="color:#cf802a;">setting value</span>
* <!-- .element: class="fragment fade-in" --> <code>std::future</code> is used for <span style="color:#cf802a;">getting value</span>
* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> pair can be used only once
