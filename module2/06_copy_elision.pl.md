<!-- .slide: data-background="#111111" -->

# Copy elision (RVO)

___
<!-- .slide: style="font-size: 0.8em" -->

## Copy elision (RVO)

```cpp
#include <thread>

std::thread f() {
    return std::thread([]{});
}

int main() {
    auto t = f();           // copy elision - ok
    // auto t2 = t;         // copying not allowed
    auto t3 = std::move(t); // moving is ok
    t3.join();              // join on t3, not t
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Kopiowanie wątku jest zabronione
* <!-- .element: class="fragment fade-in" --> Zwracanie kopii z funkcji podlega zasadom copy elision – kompilator optymalizuje kod, poprzez wyrzucenie zbędnego kopiowania
* <!-- .element: class="fragment fade-in" --> RVO (Return Value Optimization) to szczególny przypadek copy elision
* <!-- .element: class="fragment fade-in" --> Jeśli zmienna lokalna utworzona w funkcji jest zwraca przez kopię nastąpi RVO
* <!-- .element: class="fragment fade-in" --> Zmienna zostanie od razu utworzona w odpowiednim miejscu na stosie, gdzie jest możliwy dostęp do niej z poziomu wyższej ramki stosu
* <!-- .element: class="fragment fade-in" --> Dzięki RVO można zwracać wątki z funkcji poprzez kopię
