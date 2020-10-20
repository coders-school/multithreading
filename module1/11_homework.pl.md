<!-- .slide: data-background="#111111" -->

# Podsumowanie

___

## Czym jest współbieżność?

* <!-- .element: class="fragment fade-in" --> Dwa procesy są współbieżne jeżeli jeden z nich rozpoczyna się przed zakończeniem drugiego

<img class="plain fragment fade-in" data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> Współbieżność polega na jednoczesnym wykonywaniu co najmniej dwóch czynności

___

## Kiedy stosować współbieżność?

* <!-- .element: class="fragment fade-in" --> Podział zagadnień
* <!-- .element: class="fragment fade-in" --> Wydajność:
  * <!-- .element: class="fragment fade-in" --> Zrównoleglanie zadań (podział zadania na części),
  * <!-- .element: class="fragment fade-in" --> Zrównoleglanie danych (wykonywanie tych samych zadań, na różnych fragmentach danych)

___

## Kiedy nie stosować współbieżności?

* <!-- .element: class="fragment fade-in" --> Gdy zbyt wiele wątków może obniżyć wydajność zamiast ją zwiększyć (koszt uruchomienia wątku, koszt zasobów – każdy wątek zajmuje pamięć typowo 8MB),
* <!-- .element: class="fragment fade-in" --> Gdy wzrost wydajności nie jest proporcjonalny do włożonego wysiłku i złożoności kodu (koszt utrzymania kodu jest równie istotny).
<!-- Oryginalnie to brzmiało: Gdy wzrost wydajności nie jest proporcjonalny do woźnego wysiłku i złożoności kodu (koszt utrzymania kodu jest równie istotny)., zamieniłem "woźnego" na "włożonego", ale nie jestem pewien -->

___
<!-- .slide: data-background="#111111" -->

# Zadania

___

## Zadanie 1

* <!-- .element: class="fragment fade-in" --> Zaimplementuj program, w którym 4 wątki będą wyświetlać swój <code>std::this_thread::id()</code> określoną liczbę razy

___
<!-- .slide: style="font-size: 0.8em" -->

## Zadanie 1 - rozwiązanie

```cpp
#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

template <typename TIME>
void daemon(int number, TIME time) {
    for (int i = 0 ; i < number; ++i) {
        std::cout << "Hi I'm thread with id: " << std::this_thread::get_id() << " Number: " << number << std::endl;
        std::this_thread::sleep_for(time);
    }
}

int main() {
    std::thread t1(daemon<decltype(1s)>, 20, 1s);
    std::thread t2(daemon<decltype(1500ms)>, 15, 1500ms);
    std::thread t3(daemon<decltype(2700ms)>, 10, 2700ms);

    t1.detach();
    t2.detach();
    t3.detach();

    daemon(4, 7s);
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

___

## Zadanie 2

* Zaimplementuj algorytm <code>std::accumulate</code> wykorzystując wielowątkowość

```cpp
template< class InputIt, class T>
T accumulate( InputIt first, InputIt last, T init);
```

```cpp
template<class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init)
{
    for (; first != last; ++first) {
        init = std::move(init) + *first; // std::move since C++20
    }
    return init;
}
```

___

## Wskazówka 1

* <!-- .element: class="fragment fade-in" --> Obliczenie wymaganych wątków
* <!-- .element: class="fragment fade-in" --> Stworzenie kontenera przechowującego wyniki

```cpp
const size_t hardwareThread = std::thread::hardware_concurrency();
const size_t neededThreads = std::min(size / minimumSize, hardwareThread);
const size_t chunkSize = size / neededThreads;

std::cout << "NeededThreads: " << neededThreads << std::endl;
std::cout << "ChunkSize: " << chunkSize << std::endl;
std::vector<std::thread> threads(neededThreads - 1);
std::vector<T> results(neededThreads);
```
<!-- .element: class="fragment fade-in" -->

___

## Wskazówka 2

* <!-- .element: class="fragment fade-in" --> Implementacja funktora dla wątków
  * <!-- .element: class="fragment fade-in" --> Funktor przyjmuje kolejne porcje danych
  * <!-- .element: class="fragment fade-in" --> Należy obliczyć i przekazać mu iteratory odnoszące się do początku i końca zakresu na którym ma operować wątek.
  * <!-- .element: class="fragment fade-in" --> Należy przekazać także przez referencje zmienną przechowującą wynik.

```cpp
std::thread([](IT first, IT last, T& result)
            {
                result = std::accumulate(first, last, T{});
            }, begin, end, std::ref(results[i]));
```
<!-- .element: class="fragment fade-in" -->

___

## Wskazówka 3

* <!-- .element: class="fragment fade-in" --> Należy wykorzystać również aktualny wątek, na którym wywoływana jest funkcja, aby nie czekał bezczynnie na wywołanie pozostałych wątków

```cpp
auto begin = first;
for (size_t i = 0; i < neededThreads - 1; ++i) {
    auto end = std::next(begin, chunkSize);
    threads[i] = std::thread([](IT first, IT last, T& result)
            {
                result = std::accumulate(first, last, T{});
            }, begin, end, std::ref(results[i]));
    begin = end;
}
results[neededThreads - 1] = std::accumulate(begin, last, T{});
```
<!-- .element: class="fragment fade-in" -->

___

## Czy zadanie było trudne?

* <!-- .element: class="fragment fade-in" --> Co sprawiło największy problem?
* <!-- .element: class="fragment fade-in" --> Czy można uprościć algorytm?
* <!-- .element: class="fragment fade-in" --> Czy można zastosować inne mechanizmy ułatwiające implementacje?

___

## Zadanie domowe

* Zaimplementuj algorytm <code>count_if</code> wykorzystując wielowątkowość

___

## Przydatne linki

* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/thread/thread><code>std::thread</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/utility/functional/ref><code>std::ref</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp-concurrency-and-parallelism>C++ Core Guidelines on Concurrency and Parallelism</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/>Top 20 C++ multithreading mistakes and how to avoid them</a>
