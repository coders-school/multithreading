<!-- .slide: data-background="#111111" -->

# Wisząca referencja

___
<!-- .slide: style="font-size: 0.8em" -->

## Problem - wisząca referencja

```cpp
#include <thread>

void do_sth(int i) { /* ... */ }

struct A {
    int& ref_;
    A(int& a) : ref_(a) {}
    void operator()() {
        do_sth(ref_); // potential access to
                      // a dangling reference
    }
};

std::thread create_thread() {
    int local = 0;
    A worker(local);
    std::thread t(worker);
    return t;
} // local is destroyed, reference in worker is dangling

int main() {
    auto t = create_thread();  // Undefined Behavior
    auto t2 = create_thread(); // Undefined Behavior
    t.join();
    t2.join();
    return 0;
}
```

___

## Problem - wisząca referencja

* <!-- .element: class="fragment fade-in" --> Trzeba zapewnić, że wątek ma poprawny dostęp do zasobów z których korzysta w czasie swojego życia, czyli np. coś nie jest usuwane wcześniej. To nie powinno być zaskoczeniem, bo nawet w jednowątkowej aplikacji trzeba o to dbać, inaczej mamy Undefined Behavior (UB).
* <!-- .element: class="fragment fade-in" --> Taki przypadek zachodzi, gdy wątek trzyma wskaźniki lub referencje do lokalnych obiektów i wątek ciągle żyje, gdy wychodzimy z lokalnej funkcji.
* <!-- .element: class="fragment fade-in" --> Kopiowanie danych do wątku jest bezpieczne, jeśli pracujesz na małych porcjach danych nie wymagających modyfikacji. Zawsze preferuj kopiowanie.
* <!-- .element: class="fragment fade-in" --> Zobacz <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp31-pass-small-amounts-of-data-between-threads-by-value-rather-than-by-reference-or-pointer>C++ Core Guidelines [CP.31]</a>
