<!-- .slide: data-background="#111111" -->

# Model pamięci C++

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Model pamięci

* <!-- .element: class="fragment fade-in" --> Najmniejsza jednostka pamięci to 1 bajt
* <!-- .element: class="fragment fade-in" --> Każdy bajt ma unikalny adres w pamięci
* <!-- .element: class="fragment fade-in" --> Synchronizacja nie jest potrzebna jeśli zapisujemy coś współbieżnie do różnych obszarów pamięci

  ```cpp
  vector<int> v(10);
  thread t1([&]{ v[0] = 5; });
  thread t2([&]{ v[1] = 15; });
  ```

* <!-- .element: class="fragment fade-in" --> Synchronizacja jest potrzebna jeśli zapisujemy coś współbieżnie do tych samych obszarów pamięci
* <!-- .element: class="fragment fade-in" --> Synchronizacja jest potrzebna jeśli co najmniej jeden wątek zapisuje a inne odczytują ten sam obszar pamięci
* <!-- .element: class="fragment fade-in" --> Brak synchronizacji gdy jest wymagana == wyścig == niezdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> <code>const</code> implikuje bezpieczeństwo wielowątkowe, bo gwarantuje tylko odczyt

[C++ Memory model on cppreference.com](https://en.cppreference.com/w/cpp/language/memory_model)
<!-- .element: class="fragment fade-in" -->

___

## Czy tutaj potrzebna jest synchronizacja?

```cpp
struct S {
    char a;
    int b;
} obj;
thread t1([&]{ obj.a = 'a'; });
thread t2([&]{ obj.b = 4; });
```

* <!-- .element: class="fragment fade-in" --> W starych wątkach POSIX możliwy jest wyścig
* <!-- .element: class="fragment fade-in" --> Od C++11 kod jest poprawny i nie trzeba go synchronizować
* <!-- .element: class="fragment fade-in" --> Pomimo tej samej struktury obszary pamięci w których zapisujemy dane są rozłączne

___

## Czy tutaj potrzebna jest synchronizacja?

```cpp
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
   thread t([&]{ v[i] = i; });
```

* <!-- .element: class="fragment fade-in" --> TAK
* <!-- .element: class="fragment fade-in" --> Jest wyścig na zmiennej <code>i</code>
* <!-- .element: class="fragment fade-in" --> Ale vector jest wolny od wyścigów
* <!-- .element: class="fragment fade-in" --> Pomimo tej samej struktury obszary pamięci w których zapisujemy dane są rozłączne
* <!-- .element: class="fragment fade-in" --> Poprawna wersja niewymagająca synchronizacji:

```cpp
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
   thread t([&]{ v[i] = i; });
```
<!-- .element: class="fragment fade-in" -->

___

## Czy tutaj potrzebna jest synchronizacja?

```cpp
vector<int> v;
for (int = 0; i < 10; i++)
    thread t([&]{ v.emplace_back(i); });
```

* <!-- .element: class="fragment fade-in" --> TAK
* <!-- .element: class="fragment fade-in" --> Podczas dodawania nowego obiektu trzeba inkrementować iterator <code>end()</code> – możliwy wyścig
* <!-- .element: class="fragment fade-in" --> Podczas dodawania nowego obiektu może dojść do realokacji wektora. Niektóre wątki mogą mieć iteratory na nieaktualną pozycję wektora.

___

## Sposoby synchronizacji

<div style="display: flex">

<div class="fragment fade-in" style="width: 50%">

### `std::mutex`

```cpp
int a = 0;
mutex m;
thread t1([&]{
    lock_guard<mutex> lg(m);
    a = 1;
});
thread t2([&]{
    lock_guard<mutex> lg(m);
    a = 2;
});
```

</div>

<div class="fragment fade-in" style="width: 50%">

### `std::atomic<T>`

```cpp
atomic<int> a = 0;
thread t1([&]{ a = 1; });
thread t2([&]{ a = 2; });
```

</div>

</div>

___

## Typ `std::atomic<T>`

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;atomic></code>
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Lekka synchronizacja
* <!-- .element: class="fragment fade-in" --> Pozwala na prostą arytmetykę i operacje bitowe: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typowo: liczby, wskaźniki
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
  * <code>operator=()</code> - zapisuje wartość w zmiennej atomowej
  * <code>store()</code> - zapisuje wartość w zmiennej atomowej, dodatkowo można podać <code>std::memory_order</code>
  * <code>operator T()</code> – odczytuje wartość ze zmiennej atomowej
  * <code>load()</code> – odczytuje wartość ze zmiennej atomowej, dodatkowo można podać <code>std::memory_order</code>

`std::atomic<T>` będzie poruszony dokładniej na oddzielnym szkoleniu
<!-- .element: class="fragment fade-in" -->

___

### Zadanie: synchronization

#### `exercises/05_synchronization.cpp`

* Użyj właściwych mechanizmów synchronizacji
  * `std::mutex` + locks
  * `std::atomic`
* Wybierz właściwe miejsca na blokowanie
* Unikaj niepotrzebnego blokowania
