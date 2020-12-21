<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Launch policies

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .96em" -->

### Launch policies

#### `async(std::launch policy, Function&& f, Args&&... args);` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::launch::async</code> - asynchronous call on a separate system thread
* <!-- .element: class="fragment fade-in" --> <code>std::launch::differed</code> - lazily executes the function <code>f</code> the first time method <code>get()</code> or <code>wait()</code> is called on the object <code>future</code> . Execution is synchronous, which means the caller waits for the function <code>f</code> to complete. If <code>get()</code> or <code>wait()</code> will not be called function <code>f</code> will not execute

#### `async(Function&& f, Args&&... args);` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> No policies - behaves the same as <code>async(std::launch::async | std::launch::differed, f, args...)</code>. Implications:
  * <!-- .element: class="fragment fade-in" --> it is not known whether the <code>f</code> will be executed concurrently
  * <!-- .element: class="fragment fade-in" --> it is not known whether the <code>f</code> will execute on another thread or on the same thread that calls <code>get()</code> or <code>wait()</code> on <code>future</code>
  * <!-- .element: class="fragment fade-in" --> it is impossible to predict whether the <code>f</code> will execute at all, because there may be paths in the code where <code>get()</code> or <code>wait()</code> will not be called (e.g. due to exceptions)
