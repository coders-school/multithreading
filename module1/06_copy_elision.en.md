<!-- .slide: data-background="#111111" -->

# Copy elision (RVO)

___
<!-- .slide: style="font-size: 0.8em" -->

## Copy elision (RVO)

```cpp
#include <thread>

std::thread f() {
    return std::thread([]{});
}

int main() {
    auto t = f();           // copy elision - ok
    // auto t2 = t;         // copying not allowed
    auto t3 = std::move(t); // moving is ok
    t3.join();              // join on t3, not t
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Copying a thread is prohibited
* <!-- .element: class="fragment fade-in" --> Returning copies from functions is subject to the copy elision rules - the compiler optimizes the code by throwing away redundant copies
* <!-- .element: class="fragment fade-in" --> RVO (Return Value Optimization) is a special case of copy elision
* <!-- .element: class="fragment fade-in" --> If the local variable created in the function is returned by the copy RVO will happen
* <!-- .element: class="fragment fade-in" --> The variable will be immediately created in the appropriate place on the stack, where it can be accessed from the upper frame of the stack
* <!-- .element: class="fragment fade-in" --> With RVO, you can return threads from a function via a copy

___
<!-- .slide: style="font-size: 0.8em" -->

## RVO - code transformation

```cpp
#include <thread>

std::thread f() {
    return std::thread([]{});
}

int main() {
    auto t = f();           // copy elision - ok
    t.join();
    return 0;
}
```

```cpp
#include <thread>

void f(std::thread& t) {
    t{[]{}};
}

int main() {
    std::thread t;
    f(t);
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->
