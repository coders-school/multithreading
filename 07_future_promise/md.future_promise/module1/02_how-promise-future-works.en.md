<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### How `std::promise`/`std::future` works

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### How `std::promise`/`std::future` works

<img data-src="img/how_promise_future_works.gif" class="plain" style="
    position: relative;
    left: 50%;
    transform: translateX(-50%);
">

___

### Basic `std::promise`/`std::future` usage

<div style="display:flex;">

<div style="width: 59%;">

```c++
std::promise<int> promise;
std::future<int> future = promise.get_future();
auto function = [] (std::promise<int> promise) {
    // ...
    promise.set_value(10);
};
std::thread t(function, std::move(promise));
std::cout << future.get() << std::endl;
t.join();
```
<!-- .element: class="fragment fade-in" -->
</div>
<div style="width: 41%;">

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> are used to create <span style="color:#AD5758;">one-way communication channel</span>
* <!-- .element: class="fragment fade-in" --> <code>std::promise</code> is used for <span style="color:#AD5758;">setting value</span>
* <!-- .element: class="fragment fade-in" --> <code>std::future</code> is used for <span style="color:#AD5758;">getting value</span>
</div>

</div>

<div style="background-color: #8B3536; padding: 3px 22px;">

<code>std::promise</code>/<code>std::future</code> can be used only once

</div> <!-- .element: class="fragment fade-in" -->