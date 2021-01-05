<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### More about `std::promise`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### More about `std::promise`

```c++
std::thread t(function, std::move(promise));
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> can be only moved

```c++
std::future<int> future = promise.get_future();
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> returns a future associated with the promised result
* <!-- .element: class="fragment fade-in" --> second call will throw

___

### How to "set"?

<div style="display:flex;">

<div style="width: 35%;">

* <!-- .element: class="fragment fade-in" --> set value

```c++
promise.set_value(10);
```
 <!-- .element: class="fragment fade-in" -->
</div>
<div style="width: 65%;">

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> are used to create <span style="color:#AD5758;">one-way communication channel</span>

```c++
promise.set_exception(std::make_exception_ptr(e));
```
<!-- .element: class="fragment fade-in" -->
```c++
try {
    // ...
} catch {
    promise.set_ecxeption(std::current_exception());
}
```
<!-- .element: class="fragment fade-in" -->
</div>

</div>

<div style="background-color: #8B3536; padding: 3px 22px;">

<code>std::promise</code> can be "set" only once

</div> <!-- .element: class="fragment fade-in" -->

___

### More about `std::future`

##### `future.valid();` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> checks if the future can be <span style="color:#AD5758;">used</span>

##### `future.wait;` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <span style="color:#AD5758;">waits</span> for the result to become available

<div style="background-color: #8B3536; padding: 3px 22px;">

do not use "invalid" future

</div> <!-- .element: class="fragment fade-in" -->

___

### How to "get"?

##### `future.get();` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <span style="color:#AD5758;">waits</span> for the results to become available and <span style="color:#AD5758;">returns</span> the result
* <!-- .element: class="fragment fade-in" --> will automatically <span style="color:#AD5758;">throw</span> stored exception
* <!-- .element: class="fragment fade-in" --> will <span style="color:#AD5758;">invalidate</span> the future

<div style="background-color: #8B3536; padding: 3px 22px;">

`std::future` can be "get" only once

</div> <!-- .element: class="fragment fade-in" -->