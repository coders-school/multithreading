<!-- .slide: data-background="#111111" -->

# RAII

___

## RAII

* <!-- .element: class="fragment fade-in" --> Resource Acquisition Is Initialization
* <!-- .element: class="fragment fade-in" --> C++ language idiom (pattern) that guarantees security of resource handling
* <!-- .element: class="fragment fade-in" --> Acquire the resource in the constructor
* <!-- .element: class="fragment fade-in" --> Release the resource in the destructor
* <!-- .element: class="fragment fade-in" --> Automatic release of the resource when an exception occurs, thanks to the stack unwinding mechanism
* <!-- .element: class="fragment fade-in" --> Known classes implementing RAII:
  * <!-- .element: class="fragment fade-in" --> <code>unique_ptr</code> - wrapper for a raw pointer
  * <!-- .element: class="fragment fade-in" --> <code>shared_ptr</code> - wrapper for a raw pointer
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock</code> - wrapper for a <code>mutex</code>
  * <!-- .element: class="fragment fade-in" --> <code>fstream</code> - wrapper for a file
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> does not implement RAII 😔
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> has a copy operation disabled
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> can be moved like <code>unique_ptr</code> (move semantics, <code>std::move</code>)

___
<!-- .slide: style="font-size: 0.8em" -->

## Task 4: `scoped_thread`

```cpp
#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    // your implementation goes here
};

void do_sth(int) {
    this_thread::sleep_for(1s);
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    auto st3 = move(st);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed
```

___

## Exercise 4: scoped_thread

* <!-- .element: class="fragment fade-in" --> Write RAII mechanism for thread - <code>scoped_thread</code>
* <!-- .element: class="fragment fade-in" --> What operations should be included?
  * <!-- .element: class="fragment fade-in" --> constructor takes a resource - <code>std::thread</code>
  * <!-- .element: class="fragment fade-in" --> the constructor prevents us from creating an object if we pass an empty thread
  * <!-- .element: class="fragment fade-in" --> the destructor is calling <code>join()</code>
  * <!-- .element: class="fragment fade-in" --> copying is prohibited
  * <!-- .element: class="fragment fade-in" --> moving is allowed
* <!-- .element: class="fragment fade-in" --> Copying threads is a deleted operation, the compiler will not allow it
* <!-- .element: class="fragment fade-in" --> Moving threads is allowed

___
<!-- .slide: style="font-size: 0.8em" -->

## Task 4 - solution

```cpp
class scoped_thread {
    std::thread t_;
public:
    explicit scoped_thread(std::thread t)
        : t_{std::move(t)}
    {
        if (not t_.joinable()) {
            throw std::logic_error("No thread");
        }
    }

    template<typename ...Args>
    explicit scoped_thread(Args&&... args)
        : t_{ std::forward<Args>(args)... }
    { }

    ~scoped_thread() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread(scoped_thread &&) = default;
    scoped_thread& operator=(const scoped_thread &) = delete;
    scoped_thread& operator=(scoped_thread &&) = default;
};

void do_sth(int) {
    this_thread::sleep_for(1s);
    cout << this_thread::get_id() << '\n';
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    [[maybe_unused]] auto st3 = move(st);
    scoped_thread st4(do_sth, 42);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << '\n';
    }
    return 0;
} // thread is safely destroyed
```
