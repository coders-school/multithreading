<!-- .slide: data-background="#111111" -->

# Exceptions in call_once

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .83em" -->

## Exercise: exceptions in call_once

<div style="display: flex;">

<div style="width: 56%;">

```c++
class X {
    once_flag once;
    vector<double> values;

    void initializeOne() {
        cout << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeOne\n";
            values = {1.0};
        });
    }

    // ...

    void initializePierdyliard() {
        cout << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializePierdyliard\n";
            throw std::bad_alloc{};
            // TODO: Can you fix me?
        });
    }
};
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 44%;">

<div style="padding: 20px;">

* <!-- .element: class="fragment fade-in" --> Try to fix the problem with throwing exceptions in call_once
* <!-- .element: class="fragment fade-in" --> According to <a href="https://en.cppreference.com/w/cpp/thread/call_once">CPP reference.com</a>:
  * <!-- .element: class="fragment fade-in" --> <i>If that invocation throws an exception, it is propagated to the caller of call_once, and the flag is not flipped so that another call will be attempted (such call to call_once is known as exceptional)</i>

</div>

```bash
$> g++ 03_exceptional_exclusive_calls.cpp
-lpthread -fsanitize=thread
$> ./a.out
...
initializePierdyliard
Call once initializePierdyliard
terminate called after throwing an instance
of 'std::bad_alloc'
what(): std::bad_alloc
Aborted (core dumped)
```
<!-- .element: class="fragment fade-in" -->

</div>

___
<!-- .slide: style="font-size: .85em" -->

## Exercise - solution

<div style="display: flex;">

<div style="width: 57%;">

```c++
class X {
    once_flag once;
    vector<double> values;
    
    void initializeOne() {
        cout << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeOne\n";
            values = {1.0};
        });
    }
    
    // ...
    
    void initializePierdyliard() try {
        cout << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializePierdyliard\n";
            throw std::bad_alloc{};
        });
    } catch (...) { /* ignore exceptions */ }
};
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 43%;">

<div style="padding: 20px;">

* <!-- .element: class="fragment fade-in" --> Try to fix the problem with throwing exceptions in call_once
* <!-- .element: class="fragment fade-in" --> According to <a href="https://en.cppreference.com/w/cpp/thread/call_once">CPP reference.com</a>:
* <!-- .element: class="fragment fade-in" --> If that invocation throws an exception, it is propagated to the caller of call_once, and the flag is not flipped so that another call will be attempted (such call to call_once is known as exceptional)
* <!-- .element: class="fragment fade-in" --> <b>NOT POSSIBLE! (At least for me 🙂)</b>

</div>

```bash
$> g++ 03_exceptional_exclusive_calls.cpp
-lpthread -fsanitize=thread
$> ./a.out
...
initializePierdyliard
Call once initializePierdyliard
initializeOne
(hang up)
```
<!-- .element: class="fragment fade-in" -->

</div>

___
<!-- .slide: style="color: #555" -->

## Exceptions in call_once - bug in standard library implementation

* If once_flag is in the "called" state, call_once immediately returns - return (passive call)
* <!-- .element: style="color: #fff" --> If once_flag is in the "not called" state, call_once executes the passed function, passing further arguments to it (active call)
  * If the function throws an exception, it is propagated on, and `once_flag` is not set to "called state" (exceptional call), so different `call_once` can be called <span style="color: #f33">(at least in theory 🙂) - [implementation bug](https://github.com/PaddlePaddle/Paddle/issues/6681), [example at cppreference.com](https://en.cppreference.com/w/cpp/thread/call_once#Example) also doesn't work, supposedly works in MSVC (Visual Studio Compiler)</span>
  * <!-- .element: style="color: #555" --> If the function ends normally, once_flag is set to the "called" state. It is guaranteed that all other calls will be passive.
* Multiple active calls on the same once_flag are queued.
* If the same flag is used for concurrent calls to different functions, it is not specified which function will be called.

___
<!-- .slide: style="color: #bbb" -->

## Useful links

* <!-- .element: class="fragment fade-in" --> <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2393.html">C++ Atomic Types and Operations (C++ Standard)</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">std::memory_order on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/call_once">std::call_once on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/once_flag">std::once_flag on cppreference.com</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://github.com/PaddlePaddle/Paddle/issues/6681">STL bug in exception handling in call_once</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://stackoverflow.com/questions/26985370/stdcall-once-vs-stdmutex-for-thread-safe-initialization">call_once vs mutex on stackoverflow</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://stackoverflow.com/questions/17712001/how-is-meyers-implementation-of-a-singleton-actually-a-singleton">Meyers Singleton on stackoverflow</a>
