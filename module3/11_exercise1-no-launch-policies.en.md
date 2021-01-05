<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Exercise 1: no launch policies problem

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="transform: scale(.9)" -->

### Exercise 1: no launch policies problem

<div style="display: flex;">

<div style="width: 60%; font-size: .88em;">

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
  cout << "Finally...\n"
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .75em;">

* <!-- .element: class="fragment fade-in" --> Undefined Behavior?
* <!-- .element: class="fragment fade-in" --> If scheduler choose <code>std::launch::async</code> then everything is fine
* <!-- .element: class="fragment fade-in" --> If it choose <code>std::launch::deferred</code> than <code>future_status</code> will never get <code>ready</code> value which gives us infinite loop
* <!-- .element: class="fragment fade-in" --> The selected policy may depend on the current system load
* <!-- .element: class="fragment fade-in" --> Fix this code so that the program will always exit. Do it without specifying a policy.

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
### Exercise 1: solution

<div style="display: flex;">

<div style="width: 60%; font-size: .85em;">

```c++
#include <iostream>
#include <future>
using namespace std;

void f() {
  this_thread::sleep_for(1s);
}

int main() {
  auto fut = async(f);

  if (fut.wait_for(0s) == future_status::deffered) {
    cout << "Scheduled as deffered. "
    << "Calling wait() to enforce execution\n";
    fut.wait();
  } else {
    while (fut.wait_for(100ms) != future_status::ready) {
      cout << "Waiting...\n";
    }
    cout << "Finally...\n"
  }
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .75em;">

* <!-- .element: class="fragment fade-in" --> There is no direct way to check how <code>future</code> will be/was run, but...
* <!-- .element: class="fragment fade-in" --> <code>wait_for()</code> returns 1 of 3 statuses:
  * <!-- .element: class="fragment fade-in" --> <code>future_status::deferred</code>
  * <!-- .element: class="fragment fade-in" --> <code>future_status::ready</code>
  * <!-- .element: class="fragment fade-in" --> <code>future_status::timeout</code>
* <!-- .element: class="fragment fade-in" --> We do not want to wait for <code>wait_for()</code> so if for time 0 it returns <code>deferred</code>, and not <code>timeout</code> then the execution is deferred and will until <code>get()</code> or <code>wait()</code> gets called
* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/thread/future/wait_for>cppreference.com</a>

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
