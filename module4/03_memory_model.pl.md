<!-- .slide: data-background="#111111" -->

# Model pamięci C++

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Model pamięci C++

* <!-- .element: class="fragment fade-in" --> Najmniejsza jednostka – 1 bajt
* <!-- .element: class="fragment fade-in" --> Każdy bajt ma unikalny adres w pamięci
* <!-- .element: class="fragment fade-in" --> Synchronizacja nie jest potrzebna jeśli zapisujemy coś wielowątkowo do różnych obszarów pamięci

```cpp
vector<int> v{10};
thread t1([&]{ v[0] = 5; });
thread t2([&]{ v[1] = 15; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Synchronizacja jest potrzebna jeśli zapisujemy coś wielowątkowo do tych samych obszarów pamięci
* <!-- .element: class="fragment fade-in" --> Synchronizacja jest potrzebna jeśli co najmniej jeden wątek zapisuje a inne odczytują ten sam obszar pamięci
* <!-- .element: class="fragment fade-in" --> Brak synchronizacji gdy jest wymagana == wyścig == niezdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> <code>const</code> implikuje bezpieczeństwo wielowątkowe, bo gwarantuje tylko odczyt
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>

___

## Model pamięci C++ - kiedy synchronizować?

### Czy tutaj potrzebna jest synchronizacja?
<!-- .element: class="fragment fade-in" -->

```cpp
struct S {
    char a;
    int b;
} obj;
thread t1([&]{ obj.a = 'a'; });
thread t2([&]{ obj.b = 4; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Od C++11 nie, pomimo tej samej struktury obszary pamięci w których zapisujemy dane są rozłączne
* <!-- .element: class="fragment fade-in" --> W starych wątkach POSIX możliwy jest wyścig

___

## Model pamięci C++ - kiedy synchronizować?

### Czy tutaj potrzebna jest synchronizacja?
<!-- .element: class="fragment fade-in" -->

```cpp
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
    thread t([&]{ v[i] = i; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Nie, pomimo tej samej struktury obszary pamięci w których zapisujemy dane są rozłączne

___

## Model pamięci C++ - kiedy synchronizować?

### Czy tutaj potrzebna jest synchronizacja?
<!-- .element: class="fragment fade-in" -->

```cpp
vector<int> v;
for (int = 0; i < 10; i++)
    thread t([&]{ v.emplace_back(i); });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> TAK
* <!-- .element: class="fragment fade-in" --> Podczas wrzucania nowego obiektu trzeba zinkrementować iterator <code>end()</code> – możliwy wyścig
* <!-- .element: class="fragment fade-in" --> Podczas wrzucania nowego obiektu może dojść do realokacji wektora. Niektóre wątki mogą mieć iteratory na nieaktualną pozycję wektora.

___
<!-- .slide: style="font-size: .95em" -->

## Model pamięci C++ - jak synchronizować?

* <!-- .element: class="fragment fade-in" --> Jak zsynchronizać zapisy / zapis + odczyt?
* <!-- .element: class="fragment fade-in" --> <code>std::mutex</code> – to już znacie

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
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::atomic<T></code> – to teraz poznamy

```cpp
atomic<int> a = 0;
thread t1([&]{ a = 1; });
thread t2([&]{ a = 2; });
```
<!-- .element: class="fragment fade-in" -->
