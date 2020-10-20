<!-- .slide: data-background="#111111" -->

# Dangling reference

___
<!-- .slide: style="font-size: 0.72em" -->

## Problem

```cpp
#include <thread>

void do_sth(int i) { /* ... */ }

struct A {
    int& ref_;
    A(int& a) : ref_(a) {}
    void operator()() {
        do_sth(ref_); // potential access to
                      // a dangling reference
    }
};

std::thread create_thread() {
    int local = 0;
    A worker(local);
    std::thread t(worker);
    return t;
} // local is destroyed, reference in worker is dangling

int main() {
    auto t = create_thread();  // Undefined Behavior
    auto t2 = create_thread(); // Undefined Behavior
    t.join();
    t2.join();
    return 0;
}
```

___

## Dangling reference/pointer

* <!-- .element: class="fragment fade-in" --> You have to ensure that the thread has correct access to the resources it uses during its lifetime, i.e. something is not deleted sooner. This should not be a surprise, because even in a single-threaded application you have to take care of it, otherwise we have Undefined Behavior (UB).
* <!-- .element: class="fragment fade-in" --> This is the case when the thread is holding pointers or references to local objects and the thread is still alive when we exit the local function.
* <!-- .element: class="fragment fade-in" --> Copying data to a thread is safe. Always prefer copying.
* <!-- .element: class="fragment fade-in" --> See <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp31-pass-small-amounts-of-data-between-threads-by-value-rather-than-by-reference-or-pointer>C++ Core Guidelines [CP.31]</a>
