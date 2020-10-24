<!-- .slide: data-background="#111111"-->
# `std::thread`

___

## Thread - `std::thread`

* <!-- .element: class="fragment fade-in" --> The thread is an object
* <!-- .element: class="fragment fade-in" --> <code>#include &lt;thread&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code>
* <!-- .element: class="fragment fade-in" --> The most important operations:
  * <!-- .element: class="fragment fade-in" --> constructor - starts a thread
  * <!-- .element: class="fragment fade-in" --> <code>get_id()</code>
  * <!-- .element: class="fragment fade-in" --> <code>join()</code>
  * <!-- .element: class="fragment fade-in" --> <code>detach()</code>
  * <!-- .element: class="fragment fade-in" --> <code>joinable()</code>

<!-- w polskiej wersji byly krotkie opisy tych funkcji (2-4 slowa), ale tutaj nazwy wg mnie mowia same za siebie -->
___

## Basic usage

```cpp
#include <thread>
#include <iostream>
using namespace std;

void action() {
    cout << "Hello ";
    cout << this_thread::get_id();
    cout << " thread" << endl;
}

int main() {
    thread t(action);
    // can do other stuff here
    t.join();
    return 0;
}
```

```output
$> g++ 01_hello.cpp –lpthread
$> ./a.out
Hello 47082117789440 thread
```
<!-- .element: class="fragment fade-in" -->

___

## What to pass to `std::thread`?

Any callable, like:
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Function
* <!-- .element: class="fragment fade-in" --> Functor (function object) - an object with <code>operator()</code>
* <!-- .element: class="fragment fade-in" --> Lambda expression
* <!-- .element: class="fragment fade-in" --> <code>std::function</code>
* <!-- .element: class="fragment fade-in" --> Pointer to a function or pointer to a member function

The callable is <strong>copied</strong> to the memory area belonging to the newly created thread
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: 0.9em" -->

## What to pass to `std::thread`?

<div class="multicolumn">
<div class="col">

```cpp
struct Bar {
    void operator()() {
        std::cout << "Hello world\n";
    }
}

void foo() {
    std::cout << "Hello world\n";
}

int main() {
    std::thread t1(foo);
    Bar bar;
    std::thread t2(bar);
    std::thread t3([]() {
        std::cout << "Hello world\n";
    });
    t1.join();
    t2.join();
    t3.join();
}
```

</div>
<div class="col">

```cpp
void foo() {
    std::cout << "Hello world\n";
}

class Bar {
public:
    void foo() {
        std::cout << "Hello world\n";
    }
};

int main() {
    std::thread t(&foo);
    t.join();

    Bar bar;
    std::thread t1(&Bar::foo, &bar);
    t1.join();
    return 0;
}
```

</div>
</div>

___

## Empty threads (Not-A-Thread)

* <!-- .element: class="fragment fade-in" --> Threads are started immediately after their creation, as long as we pass the so-called thread of execution or callable (function, functor, lambda). They are related to system threads.
* <!-- .element: class="fragment fade-in" --> Threads are pinned to their variable in the thread that created it. Creating an empty thread <code>std::thread t;</code> does not start anything.
* <!-- .element: class="fragment fade-in" --> An empty thread (Not-A-Thread) is not associated with any system thread and <code>join()</code> or <code>detach()</code> can't be called on it

___

## Empty threads (Not-A-Thread)

```cpp
#include <thread>
#include <iostream>
using namespace std;

int main() {
    thread t;
    t.join();   // not allowed on an empty thread
    t.detach(); // not allowed on an empty thread
    return 0;
}
```

```output
$> g++ 03_join_empty_thread.cpp –lpthread
$> ./a.out
terminate called after throwing an instance of 'std::system_error'
    what():  Invalid argument
Aborted (core dumped)
```
<!-- .element: class="fragment fade-in" -->
