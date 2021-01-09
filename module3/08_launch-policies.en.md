<!-- .slide: data-background="#111111" -->

# Launch policies

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .90em" -->

## Launch policies

```cpp
async(std::launch policy, Function&& f, Args&&... args);
```

* <!-- .element: class="fragment fade-in" --> <code>launch::async</code>
  * <!-- .element: class="fragment fade-in" --> Asynchronous call on a separate system thread
* <!-- .element: class="fragment fade-in" --> <code>launch::deferred</code>
  * <!-- .element: class="fragment fade-in" --> Lazily executes the function <code>f</code> the first time methods <code>get()</code> or <code>wait()</code> are called on the <code>future</code>.
  * <!-- .element: class="fragment fade-in" --> Execution is synchronous - the caller waits for the function <code>f</code> to complete.
  * <!-- .element: class="fragment fade-in" --> If <code>get()</code> or <code>wait()</code> is not called, the function <code>f</code> will not be executed.
* <!-- .element: class="fragment fade-in" --> Both <code>launch::async | launch::deferred</code> (default)
  * <!-- .element: class="fragment fade-in" --> Asynchronous execution or lazy evaluation (up to the implementation)
  * <!-- .element: class="fragment fade-in" --> It is not known whether the <code>f</code> will be executed concurrently
  * <!-- .element: class="fragment fade-in" --> It is not known whether the <code>f</code> will be executed on another thread or on the same thread that calls <code>get()</code> or <code>wait()</code> on <code>future</code>
  * <!-- .element: class="fragment fade-in" --> It is impossible to predict whether the <code>f</code> will execute at all, because there may be paths in the code where <code>get()</code> or <code>wait()</code> will not be called (e.g. due to exceptions)
* <!-- .element: class="fragment fade-in" --> Neither <code>launch::async</code> or <code>launch::deferred</code>
  * <!-- .element: class="fragment fade-in" --> Undefined Behavior
* <!-- .element: class="fragment fade-in" --> There are also additional, implementation defined policies allowed

___
<!-- .slide: style="transform: scale(.9)" -->

### Exercise: no launch policies problem

<div style="display: flex;">

<div style="width: 60%; font-size: .8em;">

```c++
#include <iostream>
#include <future>
using namespace std;

void f() {
    this_thread::sleep_for(1s);
}

int main() {
    auto fut = async(f);

    while (fut.wait_for(100ms) != future_status::ready) {
        // loop until f has finished running...
        // ... which may never happen!
        cout << "Waiting...\n";
    }
    cout << "Finally...\n";
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; padding: 5px 10px; font-size: .75em;">

* <!-- .element: class="fragment fade-in" --> Undefined Behavior?
* <!-- .element: class="fragment fade-in" --> If the scheduler choose <code>std::launch::async</code> then everything is fine
* <!-- .element: class="fragment fade-in" --> If it choose <code>std::launch::deferred</code> then <code>future_status</code> will never get <code>ready</code> value which gives us infinite loop
* <!-- .element: class="fragment fade-in" --> The selected policy may depend on the current system load
* <!-- .element: class="fragment fade-in" --> Can you fix this code without specifying a policy?

```bash
$> ./04_async_never_called
Waiting...
Waiting...
Waiting...
Waiting...
Waiting...
Waiting...
Waiting...
Waiting...
Waiting...
Finally...
```
<!-- .element: class="fragment fade-in" style="font-size: .6em" -->
</div> <!-- .element: class="fragment fade-in" -->

<div>

___
<!-- .slide: style="transform: scale(.9)" -->
### Exercise: solution

<div style="display: flex;">

<div style="width: 60%; font-size: .7em;">

```c++
#include <iostream>
#include <future>
using namespace std;

void f() {
    this_thread::sleep_for(1s);
}

int main() {
    auto fut = async(f);

    if (fut.wait_for(0s) == future_status::deferred) {
        cout << "Scheduled as deferred. "
             << "Calling wait() to enforce execution\n";
        fut.wait();
    } else {
        while (fut.wait_for(100ms) != future_status::ready) {
            cout << "Waiting...\n";
        }
    }
    cout << "Finally...\n"
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; padding: 10px; font-size: .75em;">

* <!-- .element: class="fragment fade-in" --> There is no direct way to check how <code>future</code> will be/was run, but...
* <!-- .element: class="fragment fade-in" --> <code>wait_for()</code> returns 1 of 3 statuses:
  * <!-- .element: class="fragment fade-in" --> <code>future_status::deferred</code>
  * <!-- .element: class="fragment fade-in" --> <code>future_status::ready</code>
  * <!-- .element: class="fragment fade-in" --> <code>future_status::timeout</code>
* <!-- .element: class="fragment fade-in" --> <code>wait_for()</code> called with 0 time returns immediately
  * <!-- .element: class="fragment fade-in" --> <code>future_status::deferred</code> means that <code>deferred</code> was chosen
  * <!-- .element: class="fragment fade-in" --> <code>future_status::timeout</code> means that <code>async</code> was chosen
* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/thread/future/wait_for>cppreference.com</a>

</div>

<div>

___

### The riddle

```c++
#include <iostream>
#include <string>
#include <future>

int main() {
    std::string x = "x";
    std::async(std::launch::async, [&x](){
        x = "y";
    });
    std::async(std::launch::async, [&x](){
        x = "z";
    });
    std::cout << x;
}
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> What will be displayed on screen?
  * <!-- .element: class="fragment fade-in" --> x
  * <!-- .element: class="fragment fade-in" --> y
  * <!-- .element: class="fragment fade-in" --> z
  * <!-- .element: class="fragment fade-in" --> It depends (on what?)

___

### Answer

```bash
$> ./06_riddle
z
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Explanation:
  * <!-- .element: class="fragment fade-in" --> if the future object is temporary, it waits in the destructor until the task is over. So the second task will be run after the first one and even though they will be in other threads, their execution will be synchronized
  * <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/future/~future">cppreference.com ~future</a>
  * <!-- .element: class="fragment fade-in" --> <a href="http://cppquiz.org/quiz/question/48">Source</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://scottmeyers.blogspot.com/2013/03/stdfutures-from-stdasync-arent-special.html">std::futures from std::async aren't special! - Scott Meyers</a>
* <!-- .element: class="fragment fade-in" --> Conclusions:
  * <!-- .element: class="fragment fade-in" --> If you want to have asynchronous calls, you have to save the result in the <code>std::future</code> variable
* <!-- .element: class="fragment fade-in" --> C++20 - changes:
  * <!-- .element: class="fragment fade-in" --> <code>std::async</code> has marked it's return type with <code>[[nodiscard]] </code> attribute. Compiler will emit a warning when it is not assigned to a local variable.
