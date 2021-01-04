<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Przykład: polityki uruchamiania

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Przykład: polityki uruchamiania

<div style="display: flex;">

<div style="width: 70%; font-size: .9em;">

<div style="display: flex;">

<div style="width: 50%; font-size: .85em;">

```c++
#include <future>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;

int main () {
  auto f1 = async([] {
    cout << "f1 started\n";
    this_thread::sleep_for(1s);
    return 42;
  });
  cout << "f1 spawned\n";

  auto f2 = async(
    launch::async, [] {
      cout << "f2 started\n";
      this_thread::sleep_for(1s);
      return 2 * 42;
  });
  cout << "f2 spawned\n";

  auto f3 = async(
    launch::differed, [] {
      cout << "f3 started\n";
```

</div>

<div style="width: 50%; font-size: .85em;">

```c++
      this_thread::sleep_for(1s);
      return 3 * 42;
  });
  cout << "f3 spawned\n";

  cout << "Getting f1 result\n";
  auto v1 = f1.get();
  cout << "Got f1 result\n";

  cout << "Getting f2 result\n";
  auto v1 = f2.get();
  cout << "Got f2 result\n";

  cout << "Getting f3 result\n";
  auto v1 = f3.get();
  cout << "Got f3 result\n";

  vector<int> numbers = {
    v1, v2, v3
  };
  for (const auto & item : numbers)
    cout << item << '\n';
  
  return 0;
}
```

</div>

</div>
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 30%; background-color: #8B3536; padding: 5px 10px; font-size: .7em;">

* <!-- .element: class="fragment fade-in" --> Uruchom examples/04_async_policies
* <!-- .element: class="fragment fade-in" --> Spójrz na kod źródłowy
* <!-- .element: class="fragment fade-in" --> Uruchom examples/05_async_ids
* <!-- .element: class="fragment fade-in" --> Poeksperymentuj z ustawieniami polityk
* <!-- .element: class="fragment fade-in" --> Zaobserwuj działania programów
* <!-- .element: class="fragment fade-in" --> Wyciągnij wnioski :)

```bash
$> ./04_async_policies
f1 spawned
f1 started
f2 spawned
f3 spawned
Getting f1 result
f2 started
Got f1 result
Getting f2 result
Got f2 result
Getting f3 result
f3 started
Got f3 result
42
84
126
```
<!-- .element: class="fragment fade-in" style="font-size: 0.6em;" -->
</div> <!-- .element: class="fragment fade-in" -->

<div>