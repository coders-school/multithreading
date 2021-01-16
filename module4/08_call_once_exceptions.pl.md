<!-- .slide: data-background="#111111" -->

# Multithreading

## Moduł 4

### Wyjątki w call_once

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

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

* Jeśli once_flag jest w stanie "wywołana", call_once natychmiast zwraca - return (passive call)
* <!-- .element: style="color: #fff" --> Jeśli once_flag jest w stanie "nie wywołana", call_once wykonuje przekazaną funkcję, przekazując do niej dalsze argumenty (active call)
  * Jeśli funkcja rzuci wyjątkiem to jest on propagowany dalej, a `once_flag` nie zostaje ustawiona w stanie "wywołana" (exceptional call), więc inny `call_once` może zostać wywołany <span style="color: #f33">(przynajmniej w teorii 🙂) – [bug w implementacji](https://github.com/PaddlePaddle/Paddle/issues/6681), [przykład na cppreference.com](https://en.cppreference.com/w/cpp/thread/call_once#Example) też nie działa, podobno działa w MSVC (Visual Studio Compiler)</span>
  * <!-- .element: style="color: #555" --> Jeśli funkcja zakończy się normalnie, once_flag zostaje ustawiona w stan "wywołana" (returning call). Gwarantowane jest, że wszystkie inne wywołania będą pasywne.
* Kilka aktywnych zawołań na tej samej fladze once_flag jest kolejkowanych.
* Jeśli tej samej flagi używamy do współbieżnych wywołań różnych funkcji, to nie jest wyspecyfikowane, która funkcja zostanie zawołana.

___
<!-- .slide: style="color: #bbb" -->

### Przydatne linki

* <!-- .element: class="fragment fade-in" --> <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2393.html">C++ Atomic Types and Operations (C++ Standard)</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">std::memory_order on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/call_once">std::call_once on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/once_flag">std::once_flag on cppreference.com</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://github.com/PaddlePaddle/Paddle/issues/6681">STL bug in exception handling in call_once</a>
  * <!-- .element: class="fragment fade-in" --> <a href="https://stackoverflow.com/questions/26985370/stdcall-once-vs-stdmutex-for-thread-safe-initialization">call_once vs mutex on stackoverflow</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://stackoverflow.com/questions/17712001/how-is-meyers-implementation-of-a-singleton-actually-a-singleton">Meyers Singleton on stackoverflow</a>
