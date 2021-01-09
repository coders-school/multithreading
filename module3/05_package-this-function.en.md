<!-- .slide: data-background="#111111" -->
# Exercises

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Task #1

Implement `get_number_async()` function. It should call `get_number()` asynchronously on another thread and return `std::future` which will hold the result.

```cpp
int get_number() {
    return 10;
}

int main() {
    auto future = get_number_async();
    return future.get();
}
```

### [Example in slide 3.3](#/promise-future-example)

___

## Solution

```c++
std::future<int> get_number_async() {
    std::promise<int> p;
    std::future<int> f = p.get_future();
    auto wrapped_func = [] (std::promise<int> p) {
        p.set_value(get_number());
    };
    std::thread t(wrapped_func, std::move(p));
    t.detach();
    return f;
}
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .85em" -->

## Task 2

Implement a `schedule()` function. It should be able to take a function like `get_number()` as a parameter and call it asynchronously on another thread. It should return `std::future` which will hold the result.

```c++
int get_number() {
  return 10;
}

int main() {
  auto future = schedule(get_number);
  return future.get();
}
```

Test it on a below function
<!-- .element: class="fragment fade-in" -->

```cpp
int throw_sth() {
    throw std::runtime_error{"Sorry"};
}
```
<!-- .element: class="fragment fade-in" -->

___

## Solution

```cpp
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
<!-- .element: class="fragment fade-in" -->

___

## Better solution

* <!-- .element: class="fragment fade-in" --> Use template for function
* <!-- .element: class="fragment fade-in" --> Use variadic templates for function arguments
* <!-- .element: class="fragment fade-in" --> Use <code>std::invoke_result_t</code> to get return type
* <!-- .element: class="fragment fade-in" --> Use perfect forwarding
* <!-- .element: class="fragment fade-in" --> Do not pass arguments by ref into lambda - lifetime issues

It has more to do with templates rather than multithreading, so this is your homework.
<!-- .element: class="fragment fade-in" -->

## Homework
<!-- .element: class="fragment fade-in" -->

Modify the <code>schedule()</code> function, so it can take a function of any type, and behave similarly to <code>std::async()</code>
<!-- .element: class="fragment fade-in" -->

Take an examples from: <a href="https://en.cppreference.com/w/cpp/thread/async"> cppreference</a>
<!-- .element: class="fragment fade-in" -->
