<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Homework for volunteers: generalisation of a `schedule` function (level hard)

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Homework for volunteers: generalisation of a `schedule` function (level hard)

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
std::future<int> schedule(std::function<int()> func) {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  auto wrapped_func = [func] (std::promise<int> p) {
    try {
      p.set_value(func());
    } catch(...) {
      p.set_exception(std::current_exception());
    }
  };
  std::thread t(wrapped_func, std::move(p));
  t.detach();
  return f;
}
```

</div><!-- .element: class="fragment fade-in" -->

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> Task 1. Modify <code>schedule()</code> function, so it can assume function of any type, and thus behave similarly to <code>std::async()</code>
* <!-- .element: class="fragment fade-in" --> Task 2. Add an additional parameter - <code>std::launch policy</code>, that will specify whether the function should be executed immediately or only on request
* <!-- .element: class="fragment fade-in" --> Take an example from: <a href="https://en.cppreference.com/w/cpp/thread/async"> cppreference-async</a>

</div><!-- .element: class="fragment fade-in" -->

<div>