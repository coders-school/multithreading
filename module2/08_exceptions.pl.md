<!-- .slide: data-background="#111111" -->

# Wyjątki w wątkach

___

## Problem - wyjątki w wątkach

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

___
<!-- .slide: style="font-size: 0.85em" -->

## Problem - wyjątki w wątkach

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

## Problem - wyjątki w wątkach

* <!-- .element: class="fragment fade-in" --> Nie można standardowo złapać wyjątków w innym wątku niż tym, który rzucił wyjątek
* <!-- .element: class="fragment fade-in" --> Aby przechwycić wyjątek rzucony z innego wątku należy użyć wskaźnika na wyjątek <code>-std::exception_ptr</code>
* <!-- .element: class="fragment fade-in" --> Wątek rzucający wyjątek powinien przypisać do wskaźnika na wyjątek obecny wyjątek za pomocą <code>std::current_exception()</code>
* <!-- .element: class="fragment fade-in" --> Wątek, który chce złapać wyjątek powinien sprawdzić, czy <code>std::exception_ptr</code> został ustawiony i jeśli tak jest rzucić ten wyjątek ponownie poprzez <code>std::rethrow_exception()</code>
* <!-- .element: class="fragment fade-in" --> Warto używać w wyjątkach funkcji <code>noexcept</code>, aby mieć pewność, że wyjątki nie będą rzucane
