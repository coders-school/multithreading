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

___
<!-- .slide: style="font-size: .85em" -->

### Przykład: thread-safe Singleton

```c++
class Singleton {
    static std::unique_ptr<Singleton> instance_;
    Singleton() = default;
public:
    static Singleton& getInstance() {
        std::mutex mutex_;
        std::unique_lock<std::mutex> lock(mutex_);
        if (!instance_) {
            instance_.reset(new Singleton{});
        }
        lock.unlock();
        return *instance_;
    }
};
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Wolne (mutexy)
* <!-- .element: class="fragment fade-in" --> Bezpieczne
* <!-- .element: class="fragment fade-in" --> Długi kod

___

### Przykład: thread-safe Singleton

```c++
class Singleton {
    static std::unique_ptr<Singleton> instance_;
    static std::once_flag flag_;
    Singleton() = default;
public:
    static Singleton& getInstance() {
        std::call_once(flag_, [&] {
            instance_.reset(new Singleton{});
        });
        return *instance_;
    }
};
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Wolne (once_flag)
* <!-- .element: class="fragment fade-in" --> Bezpieczne
* <!-- .element: class="fragment fade-in" --> Trochę mniej kodu

___

### Przykład: thread-safe Singleton

```c++
class Singleton {
    Singleton() = default;

public:
    static Singleton& getInstance() {
        static Singleton instance_;
        return instance_;
    }
};

// Meyers Singleton
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Najszybsze
* <!-- .element: class="fragment fade-in" --> Bezpieczne – statyczna inicjalizacja jest thread-safe od C++11
* <!-- .element: class="fragment fade-in" --> Krótkie
* <!-- .element: class="fragment fade-in" --> Śliczne

___

### Przykład: thread-safe Singleton

```c++
class Singleton {
    static std::unique_ptr<Singleton> instance_;
    static std::once_flag flag_;
    Singleton() = default;
public:
    static Singleton& getInstance() {
        std::call_once(flag_, [&] {
            instance_.reset(new Singleton{});
        });
        return *instance_;
    }
};
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Wolne (once_flag)
* <!-- .element: class="fragment fade-in" --> Bezpieczne
* <!-- .element: class="fragment fade-in" --> <strike>Trochę mniej kodu</strike>

___
<!-- .slide: style="font-size: .83em" -->

### Zadanie 3: wyjątki w call_once

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

<div style="background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> Spróbuj naprawić problem z rzucaniem wyjątków w call_once
* <!-- .element: class="fragment fade-in" --> Wg <a href="https://en.cppreference.com/w/cpp/thread/call_once">cppreference.com</a>:
  * <!-- .element: class="fragment fade-in" --> <i>If that invocation throws an exception, it is propagated to the caller of call_once, and the flag is not flipped so that another call will be attempted (such call to call_once is known as exceptional)</i>

</div> <!-- .element: class="fragment fade-in" -->

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

### Zadanie 3 - rozwiązanie

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

<div style="background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> Spróbuj naprawić problem z rzucaniem wyjątków w call_once
* <!-- .element: class="fragment fade-in" --> Wg <a href="https://en.cppreference.com/w/cpp/thread/call_once">cppreference.com</a>:
  * <!-- .element: class="fragment fade-in" --> If that invocation throws an exception, it is propagated to the caller of call_once, and the flag is not flipped so that another call will be attempted (such call to call_once is known as exceptional)

* <!-- .element: class="fragment fade-in" -->  <b>NIE DA SIĘ! (Przynajmniej u mnie 🙂)</b>

</div> <!-- .element: class="fragment fade-in" -->

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

### Wyjątki w call_once – bug w implementacji biblioteki standardowej

* <!-- .element: class="fragment fade-in" --> Jeśli once_flag jest w stanie "wywołana", call_once natychmiast zwraca - return (passive call)
* <!-- .element: class="fragment fade-in" style="color: #fff" --> Jeśli once_flag jest w stanie "nie wywołana", call_once wykonuje przekazaną funkcję, przekazując do niej dalsze argumenty (active call)
  * <!-- .element: class="fragment fade-in" --> Jeśli funkcja rzuci wyjątkiem to jest on propagowany dalej, a <code>once_flag</code> nie zostaje ustawiona w stanie "wywołana" (exceptional call), więc inny <code>call_once</code> może zostać wywołany (przynajmniej w teorii 🙂) – <a href="https://github.com/PaddlePaddle/Paddle/issues/6681"bug w implementacji</a>, <a href="https://en.cppreference.com/w/cpp/thread/call_once#Example">przykład na cppreference.com</a> też nie działa, podobno działa w MSVC (Visual Studio Compiler)
  * <!-- .element: class="fragment fade-in" style="color: #555" --> Jeśli funkcja zakończy się normalnie, once_flag zostaje ustawiona w stan "wywołana" (returning call). Gwarantowane jest, że wszystkie inne wywołania będą pasywne.
* <!-- .element: class="fragment fade-in" --> Kilka aktywnych zawołań na tej samej fladze once_flag jest kolejkowanych.
* <!-- .element: class="fragment fade-in" --> Jeśli tej samej flagi używamy do współbieżnych wywołań różnych funkcji, to nie jest wyspecyfikowane, która funkcja zostanie zawołana.

___
<!-- .slide: style="color: #bbb" -->

### Przydatne linki

* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/call_once">std::call_once on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/once_flag">std::once_flag on cppreference.com</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://github.com/PaddlePaddle/Paddle/issues/6681">STL bug in exception handling in call_once</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://stackoverflow.com/questions/26985370/stdcall-once-vs-stdmutex-for-thread-safe-initialization">call_once vs mutex on stackoverflow</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://stackoverflow.com/questions/17712001/how-is-meyers-implementation-of-a-singleton-actually-a-singleton">Meyers Singleton on stackoverflow</a>
