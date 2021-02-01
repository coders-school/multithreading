<!-- .slide: data-background="#111111" -->

# Exceptions in call_once

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .8em" -->

## Exercise: exceptions in call_once

### `exercises/07_exceptional_exclusive_calls.cpp`

<div class="multicolumn">

<div style="width: 56%;">

```cpp []
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
<!-- .element: style="font-size: 1.05rem" class="fragment fade-in" -->
</div>

<div style="width: 44%;">

<div style="padding: 20px;">

* <!-- .element: class="fragment fade-in" --> Try to fix the problem with throwing exceptions in call_once
* <!-- .element: class="fragment fade-in" --> According to <a href="https://en.cppreference.com/w/cpp/thread/call_once">cppreference.com</a>:
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
<!-- .element: style="font-size: 1.1rem" class="fragment fade-in" -->

</div>

___
<!-- .slide: style="font-size: .85em" -->

## solution

<div class="multicolumn">

<div style="width: 57%;">

```cpp []
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
<!-- .element: style="font-size: 1.05rem" class="fragment fade-in" -->
</div>

<div style="width: 43%; padding: 20px;">

* Try to fix the problem with throwing exceptions in call_once
* According to <a href="https://en.cppreference.com/w/cpp/thread/call_once">cppreference.com</a>:
  * <i>If that invocation throws an exception, it is propagated to the caller of call_once, and the flag is not flipped so that another call will be attempted (such call to call_once is known as exceptional)</i>

### NOT POSSIBLE?
<!-- .element: class="fragment fade-in" -->
#### (At least for g++7.4.0 🙂)
<!-- .element: class="fragment fade-in" -->

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
<!-- .element: style="font-size: 1.0rem" class="fragment fade-in" -->

</div>

</div>


___
<!-- .slide: style="color: #555" -->

## Exceptions in call_once - bug in standard library implementation

* If once_flag is in the "called" state, call_once immediately returns - return (passive call)
* <!-- .element: style="color: #fff" --> If once_flag is in the "not called" state, call_once executes the passed function, passing further arguments to it (active call)
  * If the function throws an exception, it is propagated on, and `once_flag` is not set to "called state" (exceptional call), so different `call_once` can be called <span style="color: #f33">(at least in theory 🙂) - [implementation bug](https://github.com/PaddlePaddle/Paddle/issues/6681), [example at cppreference.com](https://en.cppreference.com/w/cpp/thread/call_once#Example) also doesn't work. Supposedly works in MSVC (Visual Studio Compiler) and new versions of Apple Clang</span>
  * <!-- .element: style="color: #555" --> If the function ends normally, once_flag is set to the "called" state. It is guaranteed that all other calls will be passive.
* Multiple active calls on the same once_flag are queued.
* If the same flag is used for concurrent calls to different functions, it is not specified which function will be called.
