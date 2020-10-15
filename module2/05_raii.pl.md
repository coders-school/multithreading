<!-- .slide: data-background="#111111" -->

# RAII

___

## RAII

* <!-- .element: class="fragment fade-in" --> Resource Acquisition Is Initialization
* <!-- .element: class="fragment fade-in" --> Idiom (wzorzec) języka C++ gwarantujący bezpieczeństwo obsługi zasobów
* <!-- .element: class="fragment fade-in" --> Pozyskanie zasobu w konstruktorze
* <!-- .element: class="fragment fade-in" --> Zwolnienie zasobu w destruktorze
* <!-- .element: class="fragment fade-in" --> Automatyczne zwolnienie zasobu przy wystąpieniu wyjątku, dzięki mechanizmowi odwijania stosu
* <!-- .element: class="fragment fade-in" --> Znane klasy implementujące RAII:
  * <!-- .element: class="fragment fade-in" --> <code>unique_ptr</code> – wrapper na zwykły wskaźnik
  * <!-- .element: class="fragment fade-in" --> <code>shared_ptr</code> – wrapper na zwykły wskaźnik
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock</code> – wrapper na <code>mutex</code>
  * <!-- .element: class="fragment fade-in" --> <code>fstream</code> - wrapper na plik
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> nie implementuje RAII 😔
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> ma zablokowaną operację kopiowania
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> może być przenoszony tak jak <code>unique_ptr</code> (semantyka przenoszenia, <code>std::move</code>)

___
<!-- .slide: style="font-size: 0.8em" -->

## Zadanie 4: `scoped_thread`

```cpp
#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    // your implementation goes here
};

void do_sth(int) {
    this_thread::sleep_for(1s);
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    auto st3 = move(st);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed
```

___

## Zadanie 4: scoped_thread

* <!-- .element: class="fragment fade-in" --> Napisz mechanizm RAII na wątek - <code>scoped_thread</code>
* <!-- .element: class="fragment fade-in" --> Jakie operacje powinny zostać uwzględnione?
  * <!-- .element: class="fragment fade-in" --> konstruktor przyjmuje zasób – <code>std::thread</code>
  * <!-- .element: class="fragment fade-in" --> konstruktor nie pozwala utworzyć obiektu, jeśli przekażemy pusty wątek
  * <!-- .element: class="fragment fade-in" --> destruktor woła <code>join()</code>
  * <!-- .element: class="fragment fade-in" --> kopiowanie jest zabronione
  * <!-- .element: class="fragment fade-in" --> przenoszenie jest dozwolone
* <!-- .element: class="fragment fade-in" --> Kopiowanie wątków jest operacją usuniętą, kompilator na to nie pozwoli
* <!-- .element: class="fragment fade-in" --> Przenoszenie wątków jest dozwolone

___
<!-- .slide: style="font-size: 0.8em" -->

## Zadanie 4 - rozwiązanie

```cpp
class scoped_thread {
    std::thread t_;
public:
    explicit scoped_thread(std::thread t)
        : t_(std::move(t))
    {
        if (not t_.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread(scoped_thread &&) = default;
    scoped_thread& operator=(const scoped_thread &) = delete;
    scoped_thread& operator=(scoped_thread &&) = default;
};

void do_sth(int) {
    this_thread::sleep_for(1s);
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    auto st3 = move(st);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed
```
