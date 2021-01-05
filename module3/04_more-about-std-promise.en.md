<!-- .slide: data-background="#111111" -->

# More about `std::promise` and `std::future`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## More about `std::promise`

```c++
std::thread t(function, std::move(promise));
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code> and <code>std::future</code> can be moved only

```c++
std::future<int> future = promise.get_future();
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> returns a future associated with the promised result
* <!-- .element: class="fragment fade-in" --> second call will throw

___

### How to "set"?

* <!-- .element: class="fragment fade-in" --> set value

```c++
promise.set_value(10);
```
 <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> set exception

```c++
promise.set_exception(std::make_exception_ptr(e));
```
<!-- .element: class="fragment fade-in" -->
```c++
try {
    // ...
} catch(...) {
    promise.set_exception(std::current_exception());
}
```
<!-- .element: class="fragment fade-in" -->

<div style="background-color: #8B3536; padding: 3px 22px;">

<code>std::promise</code> can be "set" only once

</div> <!-- .element: class="fragment fade-in" -->

___

### More about `std::future`

* <!-- .element: class="fragment fade-in" --> <code>future.valid()</code>
  * <!-- .element: class="fragment fade-in" --> Checks if the future can be used
  * <!-- .element: class="fragment fade-in" --> Using invalid future cause Undefined Behavior
* <!-- .element: class="fragment fade-in" --> <code>future.wait()</code>
  * <!-- .element: class="fragment fade-in" --> waits for the result to become available

___

### How to "get"?

##### `future.get()` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <span style="color:#cf802a;">waits</span> for the results to become available and <span style="color:#cf802a;">returns</span> the result
* <!-- .element: class="fragment fade-in" --> will automatically <span style="color:#cf802a;">throw</span> stored exception
* <!-- .element: class="fragment fade-in" --> will <span style="color:#cf802a;">invalidate</span> the future

<div style="background-color: #8B3536; padding: 3px 22px;">

`std::future` can be "get" only once

</div> <!-- .element: class="fragment fade-in" -->
