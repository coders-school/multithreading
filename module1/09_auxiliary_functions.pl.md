<!-- .slide: data-background="#111111" -->

# Funkcje pomocnicze

___

## Wybór liczby wątków podczas implementacji

* <!-- .element: class="fragment fade-in" --> Zbyt dużo wątków – program działa wolniej,
* <!-- .element: class="fragment fade-in" --> Zbyt mało wątków – brak wykorzystania potencjału,
* <!-- .element: class="fragment fade-in" --> <code>std::thread::hardware_concurrency()</code> - zwraca liczbę dostępnych wątków współbieżnych. Funkcja ta może zwrócić 0, jeśli taka informacja nie będzie możliwa do uzyskania.

___

## Identyfikacja wątków

* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::get_id()</code> - zwraca obiekt typu <code>std::thread::id</code> reprezentujący identyfikator bieżącego wątku

```cpp
std::thread::id master_thread;
void some_core_part_of_the_algorithm()
{
    if (std::this_thread::get_id() == master_thread) {
        do_master_thread_work();
    }
    do_common_work();
}
```
<!-- .element: class="fragment fade-in" -->

___

## Usypianie wątków

* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::sleep_until(const chrono::time_point<Clock, Duration>& sleep_time)</code>
  * <!-- .element: class="fragment fade-in" -->blokuje wykonanie wątku przynajmniej do podanego jako parametr punktu czasu
* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::sleep_for(const chrono::duration<Rep, Period>& sleep_duration)</code>
  * <!-- .element: class="fragment fade-in" --> wstrzymuje wykonanie bieżącego wątku na (przynajmniej) określony interwał czasu
* <!-- .element: class="fragment fade-in" --> <code>std::this_thread::yield()</code>
  * <!-- .element: class="fragment fade-in" --> funkcja umożliwiająca podjęcie próby wywłaszczenia bieżącego wątku i przydzielenia czasu procesora innemu wątkowi

___

## Usypianie wątków

```cpp
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    using namespace std::chrono_literals;
    std::cout << "Hello waiter\n" << std::flush;

    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(2s);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Waited " << elapsed.count() << " ms\n";
}
```

Possible output:
<!-- .element: class="fragment fade-in" -->

```output
Hello waiter
Waited 2000.12 ms
```
<!-- .element: class="fragment fade-in" -->
