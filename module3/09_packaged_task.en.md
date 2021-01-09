<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### `std::packaged_task`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### `std::packaged_task`

<div style="display: flex;">

<div style="width: 60%; font-size: .63em;">

```c++
#include <iostream>
#include <string>
#include <cmath>
#include <future>
#include <chrono>

auto globalLambda = [](int a, int b) {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return std::pow(a, b);
};

void localPackagedTask() {
  std::packaged_task<int(int, int)> task(globalLambda);
  auto result = task.get_future();
  task(2, 9);
  std::cout << "getting result:\t" << result.get() << '\n';
}

void remotePachagedTask() {
  std::packaged_task<int(int, int)> task(globalLambda);
  auto result = task.get_future();
  std::thread t(std::move(task), 2, 9);
  t.detach();
  std::cout << "getting result:\t" << result.get() << '\n';
}

void remoteAsync() {
  auto result = std::async(std::launch::async,
  globalLambda, 2, 9);
  std::cout << "getting result:\t" << result.get() << '\n';
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .75em;">

* <!-- .element: class="fragment fade-in" --> <code>#include <future></code>
* <!-- .element: class="fragment fade-in" --> <code>std::packaged_task</code>
* <!-- .element: class="fragment fade-in" --> Auxiliary object through which the <code>std::async()</code> function is implemented
<!-- Pozwoliłem sobie na próbę usunięcia powtórzenia pomocniczy/pomocy. Oryginalnie podpunkt brzmiał: "Obiekt pomocniczy, przy pomocy którego zaimplementowana..." -->
* <!-- .element: class="fragment fade-in" --> Wraps a function that can be called asynchronously
* <!-- .element: class="fragment fade-in" --> Returns the appropriate <code>std::future<T></code>
* <!-- .element: class="fragment fade-in" --> Handles exceptions via <code>std::promise</code>/<code>std::future</code>
* <!-- .element: class="fragment fade-in" --> Does not start automatically
* <!-- .element: class="fragment fade-in" --> Requires explicit calling
* <!-- .element: class="fragment fade-in" --> The call can be forwarded to another thread

```bash
$> ./07_packaged_task
```
<!-- .element: class="fragment fade-in" style="font-size: .6em" -->
</div> <!-- .element: class="fragment fade-in" -->

<div>

___

Your code is high-level if you use only std::async and std::future object.

Raw std::thread, std::promise or std::packaged_task objects means that it uses lower abstraction level, which is more complicated to understand.

___

### Useful links

* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/async">std::async on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/packaged_task">std::packaged_task on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="http://scottmeyers.blogspot.com/2013/03/stdfutures-from-stdasync-arent-special.html">std::futures from std::async aren't special! - Scott Meyers</a>
* <!-- .element: class="fragment fade-in" --> The difference between std::async and std::packaged_task
<!-- Do ostatniego linku trzeba dodać link, tak żeby faktycznie był klikalny. Nie wiem jakie było źródło, bo niestety ale nie ma podanego odnośnika :( -->