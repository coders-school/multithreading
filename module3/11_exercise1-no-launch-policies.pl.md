<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Zadanie 1: problem braku polityki

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="transform: scale(.9)" -->

### Zadanie 1: problem braku polityki

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
* <!-- .element: class="fragment fade-in" --> Jeśli scheduler wybierze <code>std::launch::async</code> to wszystko jest w porządku
* <!-- .element: class="fragment fade-in" --> Jeśli wybierze <code>std::launch::deferred</code> to <code>future_status</code> nigdy nie będzie miał wartości <code>ready</code> i mamy nieskończoną pętlę
* <!-- .element: class="fragment fade-in" --> Wybrana polityka może zależeć od obecnego obciążenia systemu
* <!-- .element: class="fragment fade-in" --> Napraw ten kod tak, aby program zawsze się zakończył. Zrób to bez specyfikowania polityki.

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
### Zadanie 1: rozwiązanie

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

* <!-- .element: class="fragment fade-in" --> Nie ma bezpośredniego sposobu sprawdzenia na <code>future</code> w jaki sposób zostanie/zostało uruchomione, ale...
* <!-- .element: class="fragment fade-in" --> <code>wait_for()</code> zwraca 1 z 3 statusów:
  * <!-- .element: class="fragment fade-in" --> <code>future_status::deferred</code>
  * <!-- .element: class="fragment fade-in" --> <code>future_status::ready</code>
  * <!-- .element: class="fragment fade-in" --> <code>future_status::timeout</code>
* <!-- .element: class="fragment fade-in" --> Nie chcemy czekać na <code>wait_for()</code>, więc jeśli dla czasu 0 zwraca <code>deferred</code>, a nie <code>timeout</code> to uruchomienie jest odroczone i zaczeka na wywołanie <code>get()</code> lub <code>wait()</code>
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
