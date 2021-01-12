<!-- .slide: data-background="#111111" -->

# `std::packaged_task`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## `std::packaged_task` - traits

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;future&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::packaged_task</code> is something between <code>std::async()</code> and <code>std::thread</code>.
* <!-- .element: class="fragment fade-in" --> It's a callable class (functor), not a function, like <code>std::async()</code>
* <!-- .element: class="fragment fade-in" --> Auxiliary object through which the <code>std::async()</code> can be implemented
* <!-- .element: class="fragment fade-in" --> Wraps a function that can be called asynchronously
* <!-- .element: class="fragment fade-in" --> <code>operator()</code> returns the appropriate <code>std::future&lt;T&gt;</code>
* <!-- .element: class="fragment fade-in" --> Handles exceptions via <code>std::promise</code>/<code>std::future</code>
* <!-- .element: class="fragment fade-in" --> Does not start automatically
* <!-- .element: class="fragment fade-in" --> Requires explicit calling
* <!-- .element: class="fragment fade-in" --> The call can be forwarded to another thread

```bash
$> ./07_packaged_task
```
<!-- .element: class="fragment fade-in" style="font-size: .6em" -->

___

## `std::packaged_task` - example

```c++
#include <iostream>
#include <cmath>
#include <future>

auto globalLambda = [](int a, int b) {
    return std::pow(a, b);
};

void remoteAsync() {
    auto result = std::async(std::launch::async, globalLambda, 2, 9);
    std::cout << "getting result:\t" << result.get() << '\n';
}

void localPackagedTask() {
    std::packaged_task<int(int, int)> task(globalLambda);
    auto result = task.get_future();
    task(2, 9);
    std::cout << "getting result:\t" << result.get() << '\n';
}

void remotePackagedTask() {
    std::packaged_task<int(int, int)> task(globalLambda);
    auto result = task.get_future();
    std::thread t(std::move(task), 2, 9);
    t.detach();
    std::cout << "getting result:\t" << result.get() << '\n';
}
```
<!-- .element: class="fragment fade-in" style="font-size: .45em" -->

