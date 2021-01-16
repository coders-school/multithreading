<!-- .slide: data-background="#111111" -->

# Multithreading

## Moduł 4

### Zmienne atomowe

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Model pamięci C++

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

### Model pamięci C++ - kiedy synchronizować?

#### Czy tutaj potrzebna jest synchronizacja?
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

### Model pamięci C++ - kiedy synchronizować?

#### Czy tutaj potrzebna jest synchronizacja?
<!-- .element: class="fragment fade-in" -->

```cpp
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
    thread t([&]{ v[i] = i; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Nie, pomimo tej samej struktury obszary pamięci w których zapisujemy dane są rozłączne

___

### Model pamięci C++ - kiedy synchronizować?

#### Czy tutaj potrzebna jest synchronizacja?
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

### Model pamięci C++ - jak synchronizować?

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

___
<!-- .slide: style="font-size: .85em" -->

### Przykład: jak zatrzymać zapętlony wątek?

<div style="display: flex;">

<div style="width: 70%;">

```c++
#include <thread>
using namespace std;

int main() {
    bool stop = false;
    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

```bash
$> g++ 01_stop.cpp -lpthread -fsanitize=thread
$> ./a.out
WARNING: ThreadSanitizer: data race (pid=10179)
...
$> g++ 01_stop.cpp -lpthread -fsanitize=thread -03
$> ./a.out
deadlock
```
<!-- .element: class="fragment fade-in" -->

</div>

<div>

<img data-src="img/epic_fail.jpg" alt="epic fail" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

</div>

___

### Przykład: jak zatrzymać zapętlony wątek?

* <!-- .element: class="fragment fade-in" --> Zapis wykonywany w kilku krokach
* <!-- .element: class="fragment fade-in" --> Optymalizacje kompilatora
* <!-- .element: class="fragment fade-in" --> Optymalizacje w procesorze (cache)
* <!-- .element: class="fragment fade-in" --> Są wyścigi
* <!-- .element: class="fragment fade-in" --> Możliwe zakleszczenie
* <!-- .element: class="fragment fade-in" --> Jak zrobić to lepiej?

___
<!-- .slide: style="font-size: .85em" -->

### Przykład: jak zatrzymać zapętlony wątek – volatile?

<div style="display: flex;">

<div style="width: 70%;">

```c++
#include <thread>
using namespace std;

int main() {
    volatile bool stop = false;
    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

```bash
$> g++ 01b_volatile.cpp -lpthread -fsanitize=thread
$> ./a.out
WARNING: ThreadSanitizer: data race (pid=10179)
...
$> g++ 01b_volatile.cpp -lpthread -fsanitize=thread -03
$> ./a.out
WARNING: ThreadSanitizer: data race (pid=10179)
...
```
<!-- .element: class="fragment fade-in" -->

</div>

<div>

<img data-src="img/fail.jpg" alt="fail" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

</div>

___

### Przykład: jak zatrzymać zapętlony wątek – volatile?

* <!-- .element: class="fragment fade-in" --> Zapis wykonywany w kilku krokach
* <!-- .element: class="fragment fade-in" --> <strike>Optymalizacje kompilatora</strike>
* <!-- .element: class="fragment fade-in" --> Optymalizacje w procesorze (cache)
* <!-- .element: class="fragment fade-in" --> Są wyścigi
* <!-- .element: class="fragment fade-in" --> Jest niezdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> Iluzja poprawności
* <!-- .element: class="fragment fade-in" --> volatile == may be modified by external agents
* <!-- .element: class="fragment fade-in" --> volatile != may be modified concurrently by the program
* <!-- .element: class="fragment fade-in" --> Jak zrobić to lepiej?

___
<!-- .slide: style="font-size: .95em" -->

### Przykład: jak zatrzymać zapętlony wątek – zmienna z mutexem?

<div style="display: flex;">

<div style="width: 50%;">

```c++
#include <thread>
#include <mutex>

using namespace std;

int main() {
    bool flag = false;
    mutex m;

    auto stop = [&] {
        lock_guard<mutex> lg(m);
        return flag;
    };
```
<!-- .element: class="fragment fade-in" -->

</div>

<div style="width: 50%;">

```c++
    auto f = [&] {
        while (not stop()) {
            /* do sth... */
        }
    };
    thread t(f);
    {
        lock_guard<mutex> lg(m);
        flag = true;
    }
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

</div>

</div>

```bash
$> g++ 01c_mutex.cpp -lpthread -fsanitize=thread
$> ./a.out
$> g++ 01c_mutex.cpp -lpthread -fsanitize=thread -03
$> ./a.out
```
<!-- .element: class="fragment fade-in" -->

___

### Przykład: jak zatrzymać zapętlony wątek – zmienna z mutexem?

* <!-- .element: class="fragment fade-in" --> Poprawne!
* <!-- .element: class="fragment fade-in" --> Długi kod…
* <!-- .element: class="fragment fade-in" --> Wolne…

<div>

<img height="400px" data-src="img/spider_pig.jpg" alt="spider_pig" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

___
<!-- .slide: style="font-size: .95em" -->

### Przykład: jak zatrzymać zapętlony wątek – zmienna atomowa

```c++
#include <thread>
#include <atomic>
using namespace std;

int main() {
    atomic<bool> stop{false};
    auto f = [&] {
        while (not stop) {
            /* do sth... */
        }
    };
    thread t(f);
    stop = true;
    t.join();
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

```bash
$> g++ 01d_atomic.cpp -lpthread -fsanitize=thread -03
$> ./a.out
```
<!-- .element: class="fragment fade-in" -->

___

### Przykład: jak zatrzymać zapętlony wątek – zmienna atomowa

* <!-- .element: class="fragment fade-in" --> Poprawne!
* <!-- .element: class="fragment fade-in" --> Lekkie
* <!-- .element: class="fragment fade-in" --> Mało kodu

<div>

<img height="400px" data-src="img/perfect.jpeg" alt="perfect" class="plain">
<!-- .element: class="fragment fade-in" -->

</div>

___

### Zwykłe zmienne vs zmienne atomowe

Zwykłe zmienne
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> jednoczesny zapis i odczyt == niezdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> potrzeba blokowania muteksami w przypadku modyfikacji

Zmienne atomowe
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> jednoczesny zapis i odczyt == zdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> brak dodatkowych mechanizmów blokowania

___

### Zmienne atomowe

Zwykłe zmienne
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>#include <atomic></code>
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Lekka synchronizacja
* <!-- .element: class="fragment fade-in" --> Pozwala na prostą arytmetykę i operacje bitowe: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typowo: liczby, wskaźniki
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
  * <!-- .element: class="fragment fade-in" --> <code>store()</code> - zapisuje wartość w zmiennej atomowej, dodatkowo można podać <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator=()</code> - zapisuje wartość w zmiennej atomowej
  * <!-- .element: class="fragment fade-in" --> <code>load()</code> – odczytuje wartość ze zmiennej atomowej, dodatkowo można podać <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator T()</code> – odczytuje wartość ze zmiennej atomowej

___
<!-- .slide: style="font-size: .75em" -->

### Zadanie: synchronizacja danych

<div style="display: flex;">

<div style="width: 50%; font-size: .9em;">

```c++
vector<int> generateContainer() {
    vector<int> input =
        {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
    vector<int> output;
    vector<thread> threads;
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back([&]{
            output.push_back(input[i]);
        });
    }
    for (auto && t : threads) {
        t.join();
    }
    return output;
}
vector<int> generateOtherContainer() {
    int start = 5;
    bool add = true;
    vector<int> output;
    vector<thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&]{
            if (add)
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; font-size: .9em;">

```c++
                output.push_back(start+=i);
            else
                output.push_back(start-=i);
            add = !add;
        });
    }
    for (auto && t : threads) {
        t.join();
    }
    return output;
}

void powerContainer(vector<int>& input) {
    vector<thread> threads;
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back([&]{
            input[i]*=input[i];
        });
    }
    for (auto && t : threads) {
        t.join();
    }
}
```

</div> <!-- .element: class="fragment fade-in" -->

</div>

```bash
$> g++ 01_synchronization.cpp -Wall -Wextra -Werror -pedantic -lpthread -fsanitize=thread –O3
$> ./a.out
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Użyj właściwych mechanizmów synchronizacji

___
<!-- .slide: style="font-size: .9em" -->

### Rozwiązanie - synchronizacja danych

* <!-- .element: class="fragment fade-in" --> operacje wstawiania na vector powinny być synchronizowane za pomocą mutexu
* <!-- .element: class="fragment fade-in" --> wstawianie wątków do vectora threads jest wykonywane sekwencyjnie w pętli – nie trzeba tego synchronizować
* <!-- .element: class="fragment fade-in" --> gdy każdy wątek pisze do innego elementu vektora synchronizacja nie jest potrzebna
* <!-- .element: class="fragment fade-in" --> proste typy powinny być opakowane w typ <code>atomic</code>
  * <!-- .element: class="fragment fade-in" --> <code>bool add</code>
  * <!-- .element: class="fragment fade-in" --> <code>int start</code>
  * <!-- .element: class="fragment fade-in" --> funkcje lambda powinny być wyciągnięte przed pętle

```bash
$> g++ 01_synchronization.cpp -Wall -Wextra -Werror -pedantic -lpthread -fsanitize=thread –O3
$> ./a.out
2 4 6 8 10 1 3 5 7 9
4 16 36 64 100 1 9 25 49 81
1 0 2 -1 3 -2 4 -3 5 -4
1 0 4 1 9 4 16 9 25 16
```
<!-- .element: class="fragment fade-in" -->

___

### `std::memory_order`

* <!-- .element: class="fragment fade-in" --> W ramach optymalizacji kompilator ma prawo zmienić kolejność wykonywanych operacji
* <!-- .element: class="fragment fade-in" --> Kompilator musi wiedzieć, które operacje może przestawiać, a które muszą następować w określonej kolejności
* <!-- .element: class="fragment fade-in" --> SC - Sequential consistency (<code>memory_order_seq_cst</code>) gwarantuje zachowaną kolejność operacji ustaloną przez programistę, czasami koszem wydajności. Jest to domyślne zachowanie zmiennych <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Dzięki SC możemy poprawnie wnioskować jakie wartości mogą mieć zmienne niezależnie od optymalizacji procesora
  * <!-- .element: class="fragment fade-in" --> Optymalizacje kompilatora nie mogą tworzyć wyścigów
  * <!-- .element: class="fragment fade-in" --> Nudne szczegóły: <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">memory_order on cppreference.com</a>

___

### Sequential consistency i optymalizacje kompilatora

<div style="display: flex;">

<div style="width: 33%; font-size: .9em;">

```c++
// INPUT:
int foo(int a)
{
    if(a<1)
        b=2;
    if(a==2)
        b=2;
    if(a>2)
        b=2;
    return b;
}



```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 33%; font-size: .9em;">

```c++
// OPT1:
int foo(int a)
{
    if(a>2)
        b=2;
    else
        if(a<1)
            b=2;
        else
            if(a==2)
                b=2;
    return b;
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 33%; font-size: .9em;">

```c++
// OPT2:
int foo(int a)
{
    const int tmp=b;
    b=2;
    if(a==1)
        b=tmp;
    return b;
}





```
<!-- .element: class="fragment fade-in" -->
</div>

</div>

* <!-- .element: class="fragment fade-in" --> Poprawne?
* <!-- .element: class="fragment fade-in" --> Tylko OPT1
* <!-- .element: class="fragment fade-in" --> W OPT2 stan b został zmieniony niezależnie od wartości a
* <!-- .element: class="fragment fade-in" --> Inny wątek mógł odczytać wartość b w tym czasie

___

### Zmienna atomowa - podsumowanie

* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code> to lekka synchronizacja
* <!-- .element: class="fragment fade-in" --> Pozwala na prostą arytmetykę i operacje bitowe: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typowo: liczby, wskaźniki
* <!-- .element: class="fragment fade-in" --> Używa specjalnych instrukcji procesora do niepodzielnej modyfikacji danych
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code> na typach złożonych nie ma sensu
  * <!-- .element: class="fragment fade-in" --> nie ma specjalnych instrukcji procesora, które zapewniają niepodzielność takich operacji
  * <!-- .element: class="fragment fade-in" --> nie ma modelu pamięci transakcyjnej w C++ (jeszcze)
  * <!-- .element: class="fragment fade-in" --> jeśli się uda, to może nie działać zgodnie z założeniami (patrz. <a href="https://stackoverflow.com/questions/46069345/is-it-possible-to-create-an-atomic-vector-or-array-in-c/46069630">Stack Overflow</a>)
  * <!-- .element: class="fragment fade-in" --> należy użyć mutexów

___

### Przydatne linki

* <!-- .element: class="fragment fade-in" --> <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2393.html">C++ Atomic Types and Operations (C++ Standard)</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">std::memory_order on cppreference.com</a>
