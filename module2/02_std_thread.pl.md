<!-- .slide: data-background="#111111"-->
# `std::thread`

___

## C++ i obsługa wielowątkowości

* <!-- .element: class="fragment fade-in" --> Standard C++11/14/17/20 – Wprowadzenie i rozwój bibliotek odpowiadających za obsługę wielowątkowości,
* <!-- .element: class="fragment fade-in" --> Przed C++11 trzeba było korzystać z różnych bibliotek, lub ręcznie odwoływać się do interfejsów API udostępniających mechanizmy wielowątkowe,
* <!-- .element: class="fragment fade-in" --> C++11 wprowadził również nowy model pamięci przystosowany do przetwarzania wielowątkowego na wielu platformach.
* <!-- .element: class="fragment fade-in" --> Wprowadzono zarządzanie wątkami (ang. Thread), ochronę współdzielonych danych, synchronizacje operacji wykonywanych przez wątki, wykonywanie niskopoziomowych operacji atomowych itp.

___

## Wątek – `std::thread`

* <!-- .element: class="fragment fade-in" --> Wątek jest obiektem
* <!-- .element: class="fragment fade-in" --> <code>#include &lt;thread&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code>
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
  * <!-- .element: class="fragment fade-in" --> constructor – uruchamia wątek
  * <!-- .element: class="fragment fade-in" --> <code>get_id()</code> – pobranie identyfikatora wątku
  * <!-- .element: class="fragment fade-in" --> <code>join()</code> – przyłączenie wątku
  * <!-- .element: class="fragment fade-in" --> <code>detach()</code> – odłączenie wątku
  * <!-- .element: class="fragment fade-in" --> <code>joinable()</code> – czy można przyłączyć wątek

___

## Podstawowe użycie

```cpp
#include <thread>
#include <iostream>
using namespace std;

void action() {
    cout << "Hello ";
    cout << this_thread::get_id();
    cout << " thread" << endl;
}

int main() {
    thread t(action);
    // can do other stuff here
    t.join();
    return 0;
}
```

```output
$> g++ 01_hello.cpp –lpthread
$> ./a.out
Hello 47082117789440 thread
```
<!-- .element: class="fragment fade-in" -->

___

## Co przekazać do `std::thread`?

* <!-- .element: class="fragment fade-in" --> Funkcje
* <!-- .element: class="fragment fade-in" --> Funktor (obiekt funkcyjny) – obiekt, który możemy wywołać tak jak funkcję (np. lambda)
* <!-- .element: class="fragment fade-in" --> Wskaźnik na funkcję lub wskaźnik do funkcji składowej
* <!-- .element: class="fragment fade-in" --> Obiekt funkcyjny jest <strong>kopiowany</strong> do obszaru pamięci należącej do nowo utworzonego wątku

___

## Co przekazać do `std::thread`?

<div class="multicolumn">
<div class="col">

```cpp
struct Bar {
    void operator()() {
        std::cout << "Hello world";
    }
}

void foo() {
    std::cout << "Hello world";
}

int main() {
    std::thread t1([]() {
        "Hello world"
    });

    std::thread t2(foo);

    Bar bar;
    std::thread t3(bar);
}
```

</div>
<div class="col">

```cpp
void foo() {
    std::cout << "Hello world\n";
}

class Bar {
public:
    void foo() {
        std::cout << "Hello world\n";
    };
};

int main() {
    std::thread t(*foo);
    t.join();

    class Bar bar;
    std::thread t1(&Bar::foo, bar);
    t1.join();
    return 0;
}
```

</div>
</div>

___

## Puste wątki (Not-A-Thread)

* <!-- .element: class="fragment fade-in" --> Wątki są odpalane od razu po ich utworzeniu, o ile tylko przekażemy do nich tzw. thread of execution lub callable (funkcja, funktor, lambda). Są one powiązane z wątkami systemowymi.
* <!-- .element: class="fragment fade-in" --> Wątki są przypięte do swojej zmiennej w wątku, który go stworzył. Stworzenie pustego wątku <code>std::thread t;</code> nie odpala niczego.
* <!-- .element: class="fragment fade-in" --> Pusty wątek (Not-A-Thread) nie jest powiązany z żadnym wątkiem systemowym i nie woła się na nim <code>join()</code> lub <code>detach()</code>

___

## Puste wątki (Not-A-Thread)

```cpp
#include <thread>
#include <iostream>
using namespace std;

int main() {
    thread t;
    t.join();   // not allowed on an empty thread
    t.detach(); // not allowed on an empty thread
    return 0;
}
```

```output
$> g++ 03_join_empty_thread.cpp –lpthread
$> ./a.out
terminate called after throwing an instance of 'std::system_error'
    what():  Invalid argument
Aborted (core dumped)
```
<!-- .element: class="fragment fade-in" -->
