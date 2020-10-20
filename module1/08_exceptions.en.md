<!-- .slide: data-background="#111111" -->

# Exceptions in threads

___
<!-- .slide: style="font-size: 0.9em" -->

## Problem

```cpp
#include <thread>
#include <iostream>

int main() {
    try {
        std::thread t1([]{
            throw std::runtime_error("WTF - What a Terrible Failure");
        });
        t1.join();
    } catch (const std::exception & ex) {
        std::cout << "Thread exited with exception: " << ex.what() << "\n";
    }
    return 0;
}
```

```output
$> g++ 09_exceptions_not_working.cpp –lpthread
$> ./a.out
terminate called after throwing an instance of 'std::runtime_error'
    what():  WTF - What a Terrible Failure
Aborted (core dumped)
```
<!-- .element: class="fragment fade-in" -->

How to handle an exception from a different thread?
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: 0.85em" -->

## Problem - exceptions in threads

```cpp
int main() {
    std::exception_ptr thread_exception = nullptr;
    std::thread t([](std::exception_ptr & te) {
        try {
            throw std::runtime_error("WTF");
        } catch (...) {
            te = std::current_exception();
        }
    }, std::ref(thread_exception));
    t.join();
    if (thread_exception) {
        try {
            std::rethrow_exception(thread_exception);
        } catch (const std::exception & ex) {
            std::cout << "Thread exited with an exception: "
            << ex.what() << "\n";
        }
    }
    return 0;
}
```

```output
$> g++ 10_exceptions_working.cpp –lpthread
$> ./a.out
Thread exited with an exception: WTF
```
<!-- .element: class="fragment fade-in" -->

___

## Problem - exceptions in threads

* <!-- .element: class="fragment fade-in" --> Each thread has its own stack so the stack unwinding mechanism works only on its stack
* <!-- .element: class="fragment fade-in" --> You cannot normally catch exceptions from a thread other than the thread that raised the exception
* <!-- .element: class="fragment fade-in" --> To catch an exception thrown from another thread use the exception pointer <code>-std::exception_ptr</code>
* <!-- .element: class="fragment fade-in" --> The thread throwing the exception should assign the current exception to the exception pointer via <code>std::current_exception()</code>
* <!-- .element: class="fragment fade-in" --> The thread that wants to catch an exception should check if <code>std::exception_ptr</code> has been set and if it is, throw that exception again with <code>std::rethrow_exception()</code>
* <!-- .element: class="fragment fade-in" --> It is useful to use <code>noexcept</code> functions in threads to make sure no exceptions are thrown
