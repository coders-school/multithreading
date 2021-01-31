<!-- .slide: data-background="#111111" -->

# `std::call_once`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## `std::call_once`

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;mutex&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::call_once</code>
* <!-- .element: class="fragment fade-in" --> Wraps a function that must be executed only once
* <!-- .element: class="fragment fade-in" --> Guarantees one call, even when called concurrently by several threads
* <!-- .element: class="fragment fade-in" --> Calls the passed function on its own thread (doesn't create a new one)
* <!-- .element: class="fragment fade-in" --> It needs <code>std::once_flag</code> flag

___
<!-- .slide: style="font-size: .8em" -->

## `std::call_once`

```cpp []
#include <iostream>
#include <thread>
#include <mutex>

std::once_flag flag;

void do_once() {
    std::call_once(flag, [] {
        std::cout << "Called once!\n";
    });
}

int main() {
    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

```bash
$> g++ 01_call_once.cpp -lpthread -fsanitize=thread
$> ./a.out
Called once!
```
<!-- .element: class="fragment fade-in" -->

___

## once_flag

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;mutex&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::once_flag</code>
* <!-- .element: class="fragment fade-in" --> An auxiliary structure for use with <code>std::call_once</code>
* <!-- .element: class="fragment fade-in" --> Neither copyable nor movable
* <!-- .element: class="fragment fade-in" --> Contains information whether the function has already been called
* <!-- .element: class="fragment fade-in" --> The constructor sets the state to not called

___

## `call_once` way of operation

* <!-- .element: class="fragment fade-in" --> If <code>once_flag</code> is in the "called" state, <code>call_once</code> immediately returns (passive call)
* <!-- .element: class="fragment fade-in" --> If <code>once_flag</code> is in the "not called" state, <code>call_once</code> executes the passed function, passing further arguments to it (active call)
  * <!-- .element: class="fragment fade-in" --> If the function throws an exception, it is propagated on, and <code>once_flag</code> is not set to "called state" (exceptional call), so different <code>call_once</code> can be called <span style="color: #f33">(at least in theory 🙂)</span>
  * <!-- .element: class="fragment fade-in" --> If the function ends normally, <code>once_flag</code> is set to the "called" state. It is guaranteed that all other calls will be passive.
* <!-- .element: class="fragment fade-in" --> Multiple active calls on the same <code>once_flag</code> are queued.
* <!-- .element: class="fragment fade-in" --> If the same flag is used for concurrent calls to different functions, it is not specified which function will be called.

___
<!-- .slide: style="font-size: .78em" -->

### Exercise: rat race

<div class="multicolumn">

<div style="width: 60%;">

```cpp []
void setWinner() {
    auto id = this_thread::get_id();
    auto sleepDuration = dist(rng);
    stringstream msg;
    msg << "Called " << __FUNCTION__
        << "(" << id << "). Chasing time: "
        << sleepDuration << "ms\n";
    cout << msg.str();
    this_thread::sleep_for(chrono::milliseconds(sleepDuration)

    // TODO: set me as a winner but don't let others overwrite this!
}

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; padding: 20px;">

#### `exercises/05_race.cpp`

* <!-- .element: class="fragment fade-in" --> 10 contestants (threads) are racing for $1 million
* <!-- .element: class="fragment fade-in" --> Only the first player wins the prize, the rest will get nothing
* <!-- .element: class="fragment fade-in" --> Implement the function  <code>setWinner()</code> so that the winning thread sets itself as the winner and does not allow others to override this value.

</div>

</div>

```bash
$> g++ 01_race.cpp -lpthread -fsanitize=thread
$> ./a.out
Called setWinner(139887531521792). Sleeping for 15ms
Called setWinner(139887523129088). Sleeping for 35ms
Called setWinner(139887497950976). Sleeping for 31ms
Call once for 139887531521792
Called setWinner(139887489558272). Sleeping for 16ms
Called setWinner(139887481165568). Sleeping for 14ms
Called setWinner(139887453927168). Sleeping for 35ms
And the winner is... 139887531521792
```
<!-- .element: class="fragment fade-in" -->

___

## Exercise - solution

```cpp []
void setWinner() {
    auto id = this_thread::get_id();
    auto sleepDuration = dist(rng);
    stringstream msg;
    msg << "Called " << __FUNCTION__
        << "(" << id << "). Chasing time: "
        << sleepDuration << "ms\n";
    cout << msg.str();
    this_thread::sleep_for(chrono::milliseconds(sleepDuration));

    call_once(once, [&]{
        cout << "Call once for " << id << '\n';
        std::stringstream troublesomeConversion;
        troublesomeConversion << id;
        winnerId = troublesomeConversion.str();
    });
}
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .85em" -->

### Exercise: mutually exclusive calls

#### `exercises/06_exclusive_calls.cpp`

<div class="multicolumn">

<div style="width: 60%;">

```cpp []
class X {
    vector<double> values;

    void initializeOne() { values = {1.0}; }
    void initializeTwo() { values = {1.0, 2.0}; }
    void initializeThree() { values = {1.0, 2.0, 3.0}; }

public:
    explicit X(int i) noexcept {
        switch (i) {
        case 2: // top priority
            initializeTwo();
            [[fallthrough]];
        case 3:
            initializeThree();
            [[fallthrough]];
        default: // least priority
            initializeOne();
        }
    }
    // ...
};

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%;">

<div style="padding: 20px;">

* <!-- .element: class="fragment fade-in" --> Add the appropriate <code>call_once</code> and messages so that the output appears as below
* <!-- .element: class="fragment fade-in" --> Do not modify the constructor 😉

</div>

```bash
$> g++ 02_exclusive_calls.cpp
-lpthread -fsanitize=thread
$> ./a.out
initializeTwo
Call once initializeTwo
initializeThree
initializeOne
1 2
initializeThree
Call once initializeThree
initializeOne
1 2 3
initializeOne
Call once initializeOne
1
```
<!-- .element: class="fragment fade-in" -->

</div>

___
<!-- .slide: style="font-size: .88em" -->

## Exercise - solution

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

    void initializeTwo() {
        cout << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeTwo\n";
            values = {1.0, 2.0};
        });
    }

    void initializeThree() {
        cout << __FUNCTION__ << '\n';
        call_once(once, [&]{
            cout << "Call once initializeThree\n";
            values = {1.0, 2.0, 3.0};
        });
    }
    // ...
};
```
<!-- .element: class="fragment fade-in" -->
