<!-- .slide: data-background="#111111" -->

# Multithreading

## Moduł 4

### Jednokrotne wywołania

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .9em" -->

### call_once

<div style="display: flex;">

<div style="width: 60%; font-size: .85em;">

```c++
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
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> <code>#include <mutex></code>
* <!-- .element: class="fragment fade-in" --> <code>std::call_once</code>
* <!-- .element: class="fragment fade-in" --> Opakowuje funkcję, która zostanie wykonana tylko raz
* <!-- .element: class="fragment fade-in" --> Gwarantuje jednokrotne wywołanie nawet w przypadku jej współbieżnego wywołania przez kilka wątków
* <!-- .element: class="fragment fade-in" --> Wywołuje przekazaną funkcję w swoim wątku (nie tworzy nowego)
* <!-- .element: class="fragment fade-in" --> Potrzebuje flagi <code>std::once_flag</code>

</div> <!-- .element: class="fragment fade-in" -->

</div>

```bash
$> g++ 01_call_once.cpp -lpthread -fsanitize=thread
$> ./a.out
Called once!
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .9em" -->

### once_flag

<div style="display: flex;">

<div style="width: 60%; font-size: .8em;">

```c++
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
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: 1em;">

* <!-- .element: class="fragment fade-in" --> <code>#include <mutex></code>
* <!-- .element: class="fragment fade-in" --> <code>std::once_flag</code>
* <!-- .element: class="fragment fade-in" --> Pomocnicza struktura do użytku z <code>std::call_once</code>
* <!-- .element: class="fragment fade-in" --> Brak kopiowania i przenoszenia
* <!-- .element: class="fragment fade-in" --> Zawiera informację, czy funkcja z jej użyciem została już wywołana
* <!-- .element: class="fragment fade-in" --> Konstruktor ustawia stan na niewywołany

</div> <!-- .element: class="fragment fade-in" -->

</div>

```bash
$> g++ 01_call_once.cpp -lpthread -fsanitize=thread
$> ./a.out
Called once!
```
<!-- .element: class="fragment fade-in" -->

___

### Zasada działania `call_once`

* <!-- .element: class="fragment fade-in" --> Jeśli <code>once_flag</code> jest w stanie "wywołana", <code>call_once</code> natychmiast zwraca - return (passive call)
* <!-- .element: class="fragment fade-in" --> Jeśli <code>once_flag</code> jest w stanie "nie wywołana", <code>call_once</code> wykonuje przekazaną funkcję, przekazując do niej dalsze argumenty (active call)
  * <!-- .element: class="fragment fade-in" --> Jeśli funkcja rzuci wyjątkiem to jest on propagowany dalej, a <code>once_flag</code> nie zostaje ustawiona w stanie "wywołana" (exceptional call), więc inny <code>call_once</code> może zostać wywołany <span style="color: #f33">(przynajmniej w teorii 🙂)</span>
  * <!-- .element: class="fragment fade-in" --> Jeśli funkcja zakończy się normalnie, <code>once_flag</code> zostaje ustawiona w stan "wywołana" (returning call). Gwarantowane jest, że wszystkie inne wywołania będą pasywne.
* <!-- .element: class="fragment fade-in" --> Kilka aktywnych zawołań na tej samej fladze <code>once_flag</code> jest kolejkowanych.
* <!-- .element: class="fragment fade-in" --> Jeśli tej samej flagi używamy do współbieżnych wywołań różnych funkcji, to nie jest wyspecyfikowane, która funkcja zostanie zawołana.

___
<!-- .slide: style="font-size: .85em" -->

### Zadanie 1: gonitwa

<div style="display: flex;">

<div style="width: 60%;">

```c++
void setWinner() {
    auto id = this_thread::get_id();
    auto sleepDuration = dist(rng);
    stringstream msg;
    msg << "Called " << __FUNCTION__
        << "(" << id << "). Chasing time: "
        << sleepDuration << "ms\n";
    cout << msg.str();

    // TODO: set me as a winner
    // but don't let others overwrite this!
}

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> 10 zawodników (wątków) ściga się o w zawodach o milion $
* <!-- .element: class="fragment fade-in" --> Tylko pierwszy zawodnik zdobywa nagrodę, reszta nie dostanie nic
* <!-- .element: class="fragment fade-in" --> Zaimplementuj funkcję <code>setWinner()</code> tak, aby zwycięski wątek ustawił siebie jako zwycięzcę i nie pozwolił na nadpisanie innym tej wartości

</div> <!-- .element: class="fragment fade-in" -->

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

### Zadanie 1 - rozwiązanie

```c++
void setWinner() {
    auto id = this_thread::get_id();
    auto sleepDuration = dist(rng);
    stringstream msg;
    msg << "Called " << __FUNCTION__
        << "(" << id << "). Chasing time: "
        << sleepDuration << "ms\n";
    cout << msg.str();
    this_thread::sleep_for(
        chrono::milliseconds(sleepDuration)
);

call_once(once, [&]{
    cout << "Call once for " << id << '\n';
    stringstream troublesomeConversion;
    troublesomeConversion << id;
    winnerId = troublesomeConversion.str();
    });
}
```
<!-- .element: class="fragment fade-in" -->

___
<!-- .slide: style="font-size: .88em" -->

### Zadanie 2: wykluczające się wywołania

<div style="display: flex;">

<div style="width: 60%;">

```c++
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

<div style="background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> Dopisz odpowiednie jednokrotne wywołania oraz komunikaty, aby na wyjściu pojawiło się to co poniżej
* <!-- .element: class="fragment fade-in" --> Nie modyfikuj konstruktora 😉

</div> <!-- .element: class="fragment fade-in" -->

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

### Zadanie 2 - rozwiązanie

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
