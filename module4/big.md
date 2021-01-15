<!-- .slide: data-background="#111111" -->

# Multithreading

## Moduł 4

### Zmienne warunku

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie 1: kolejka FIFO

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
template <typename T>
class ThreadsafeQueue {
    deque<T> queue_;
    // TODO: Make it thread-safe :)

public:
    void push(const T & element) {
        queue_.push_front(element);
    }
    T pop() {
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        return queue_.empty();
    }
};

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> W pliku `01_threadsafe_queue.cpp` znajduje się wybrakowana implementacja kolejki FIFO
* <!-- .element: class="fragment fade-in" --> Napraw wątek `textProducer`, aby generował prawidłowe napisy:
  * <!-- .element: class="fragment fade-in" --> This is random text number 0
  * <!-- .element: class="fragment fade-in" --> This is random text number 1
  * <!-- .element: class="fragment fade-in" --> …
  * <!-- .element: class="fragment fade-in" --> This is random text number n
* <!-- .element: class="fragment fade-in" --> Zabezpiecz operacje na kolejce przed dostępem z wielu wątków (make it thread-safe ☺)
* <!-- .element: class="fragment fade-in" --> Jaki problem widzisz?

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Zadanie 1 – rozwiązanie: naprawione wyświetlanie tekstu

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number "
                + i);
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " +
                to_string(i));
}

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> W pliku `01_threadsafe_queue.cpp` znajduje się wybrakowana implementacja kolejki FIFO
* <!-- .element: class="fragment fade-in" --> Napraw wątek `textProducer`, aby generował prawidłowe napisy:
  * <!-- .element: class="fragment fade-in" --> This is random text number 0
  * <!-- .element: class="fragment fade-in" --> This is random text number 1
  * <!-- .element: class="fragment fade-in" --> …
  * <!-- .element: class="fragment fade-in" --> This is random text number n
* <!-- .element: class="fragment fade-in" --> Zabezpiecz operacje na kolejce przed dostępem z wielu wątków (make it thread-safe ☺)
* <!-- .element: class="fragment fade-in" --> Jaki problem widzisz?

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Zadanie 1 – rozwiązanie: threadsafe queue

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
template <typename T>
class ThreadsafeQueue {
    deque<T> queue_;
    mutable mutex m_;
    using Lock = lock_guard<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
    }
    T pop() {
        Lock l(m_);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        Lock l(m_);
        return queue_.empty();
    }
};

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> W pliku `01_threadsafe_queue.cpp` znajduje się wybrakowana implementacja kolejki FIFO
* <!-- .element: class="fragment fade-in" --> Napraw wątek `textProducer`, aby generował prawidłowe napisy:
  * <!-- .element: class="fragment fade-in" --> This is random text number 0
  * <!-- .element: class="fragment fade-in" --> This is random text number 1
  * <!-- .element: class="fragment fade-in" --> …
  * <!-- .element: class="fragment fade-in" --> This is random text number n
* <!-- .element: class="fragment fade-in" --> Zabezpiecz operacje na kolejce przed dostępem z wielu wątków (make it thread-safe ☺)
* <!-- .element: class="fragment fade-in" --> Jaki problem widzisz?

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Problem – grzanie CPU i efekt cieplarniany

<img data-src="../efekt_cieplarniany.jpg" alt="efekt cieplarniany" class="plain">

___

### Aktywne czekanie

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
void saveToFile(StringQueue & sq) {
    ofstream file("/tmp/sth.txt");
    while (file) {
        while (sq.empty()) { /* nop */ }
        file << sq.pop() << endl;
    }
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> Aktywne czekanie (busy waiting) to stan, w którym wątek ciągle sprawdza, czy został spełniony pewien warunek
* <!-- .element: class="fragment fade-in" --> Inna nazwa tego problemu to wirująca blokada (spinlock)
* <!-- .element: class="fragment fade-in" --> Problem rozwiązuje zmienna warunku (condition variable)

</div> <!-- .element: class="fragment fade-in" -->

<div>

<img data-src="../aktywne_czekanie.png" alt="aktywne_czekanie" class="plain">

___

### Zmienna warunku (condition variable)

* <!-- .element: class="fragment fade-in" --> `#include <condition_variable>`
* <!-- .element: class="fragment fade-in" --> `std::condition_variable`
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje
  * <!-- .element: class="fragment fade-in" --> `wait()` – oczekuje na zmianę - blokuje obecny wątek dopóki nie zostanie on wybudzony
  * <!-- .element: class="fragment fade-in" --> `notify_one()` – wybudza jeden z wątków oczekujących na zmianę. Nie mamy kontroli nad tym, który z wątków zostanie powiadomiony.
  * <!-- .element: class="fragment fade-in" --> `notify_all()` – wybudza wszystkie wątki czekające na zmianę. Wątki te mogą konkurować o zasoby.

___

### Zadanie 2: kolejka FIFO przyjazna Arktyce

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
template <typename T>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    using Lock = lock_guard<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
    }
    T pop() {
        Lock l(m_);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        Lock l(m_);
        return queue_.empty();
    }
};
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> Popraw kod z pliku `02_wait_queue.cpp` tak, aby używał zmiennej warunkowej zamiast aktywnego czekania

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Zadanie 2 - rozwiązanie

<div style="display: flex;">

<div style="width: 50%; font-size: .9em;">

```c++
// includes

template <typename T>
class WaitQueue {
    deque<T> queue_;
    mutable mutex m_;
    condition_variable nonEmpty_;
    using Lock = unique_lock<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
        nonEmpty_.notify_all();
    }
    T pop() {
        Lock l(m_);
        auto hasData = [&]{ return not queue_.empty(); };
        nonEmpty_.wait(l, hasData);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
};
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; font-size: .9em;">

```c++
using StringQueue = WaitQueue<string>;

void provideData(StringQueue & sq) {
    string txt;
    while (getline(cin, txt))
        sq.push(txt);
}

void saveToFile(StringQueue & sq) {
    ofstream file("/tmp/sth.txt");
    while (file)
        file << sq.pop() << endl;
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " +
                to_string(i));
}

int main() {
    // without changes
}
```

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Zmienne warunku uszczęśliwiają foczki ☺

<img data-src="../foczka.jpg" alt="foczka" class="plain">

___

### Zmienna warunku - szczegóły

* <!-- .element: class="fragment fade-in" --> `std::condition_variable` działa tylko z wyłącznymi blokadami (`unique_lock`)
* <!-- .element: class="fragment fade-in" --> `std::condition_variable_any` działa z każdym rodzajem blokad (`shared_lock`)
* <!-- .element: class="fragment fade-in" --> Są niekopiowalne
* <!-- .element: class="fragment fade-in" --> Metoda `wait()` przyjmuje blokadę oraz opcjonalnie predykat, dzięki któremu zostaną wybudzone tylko te wątki, dla których warunek jest spełniony
* <!-- .element: class="fragment fade-in" --> Wszystkie wątki, które czekają na zmiennej warunku muszą mieć zablokowany ten sam mutex. W przeciwnym wypadku jest niezdefiniowane zachowanie.
* <!-- .element: class="fragment fade-in" --> Metody `wait_for()` i `wait_until()` przyjmują jeszcze odpowiednio punkt w czasie lub okres czasu do którego wątki będą czekać na wybudzenie. Po upływie tego czasu wątki zostaną wybudzone.
* <!-- .element: class="fragment fade-in" --> Jeśli na zmiennej warunku czeka kilka wątków i każdy ma inny predykat, to użycie `notify_one()` może spowodować zakleszczenie. Wybudzony może zostać wątek, dla którego warunek nie został spełniony i jeśli żaden inny wątek nie zawoła `nofity_one()` lub `notify_all()` to wszystkie będą czekać.

___

### Zadanie domowe: ping-pong

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> 1 wątek wypisuje "ping" oraz kolejny numer
* <!-- .element: class="fragment fade-in" --> 2 wątek wypisuje "pong" oraz kolejny numer
* <!-- .element: class="fragment fade-in" --> Zaczyna wątek ping, a kończy zawsze pong. Wątki muszą pracować na przemian. Nie mogą być 2 pingi lub pongi po sobie. Program nie może zakończyć się pingiem, na który nie będzie odpowiedzi – ponga.
* <!-- .element: class="fragment fade-in" --> Zakończenie działania programu ma nastąpić albo po wykonanej liczbie odbić albo po limicie czasowym, w zależności które wystąpi pierwsze. Powód zakończenia powinien zostać wyświetlony na ekranie
* <!-- .element: class="fragment fade-in" --> Parametry programu:
  * <!-- .element: class="fragment fade-in" --> liczba odbić
  * <!-- .element: class="fragment fade-in" --> limit czasowy (w sekundach)

<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

```bash
$> g++ 03_ping_pong.cpp -lpthread
-std=c++17 -fsanitize=thread
$> ./a.out 1 10
ping 0
pong 0
Ping reached repetitions limit
Pong reached repetitions limit
$> ./a.out 12 1
ping 0
pong 0
ping 1
pong 1
ping 2
pong 2
Timeout
```

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Wskazówki

Jeśli utkniesz:

* <!-- .element: class="fragment fade-in" --> Potrzebujesz mutexu i zmiennej warunkowej w klasie `PingPong`
* <!-- .element: class="fragment fade-in" --> Czekaj na zmiennej warunku za pomocą `wait_for()` w funkcji `stop()`
* <!-- .element: class="fragment fade-in" --> Sprawdzaj liczbę powtórzeń w wątkach ping i pong
* <!-- .element: class="fragment fade-in" --> Użyj dodatkowej zmiennej `bool`, która powie wszystkim wątkom, aby się zakończyły. gdy nastąpią wymagane warunki. Użyj tutaj typu `atomic<bool>` (o nim później ☺)
* <!-- .element: class="fragment fade-in" --> Wątki ping i pong powinny za pomocą `wait()` sprawdzać warunek, czy to ich kolej na działanie. Użyj dodatkowej zmiennej `bool`, która zostanie użyta w predykacie przekazanym do `wait()`.
* <!-- .element: class="fragment fade-in" --> Wątek pong powinien zakończyć program po osiągnięciu limitu odbić

___

### Zmienna warunku - zagrożenia

* <!-- .element: class="fragment fade-in" --> Fałszywe przebudzenie (spurious wakeup)
  * <!-- .element: class="fragment fade-in" --> Wątek czekający na zmiennej warunku cyklicznie co pewien okres czasu wybudza się i sprawcza czy nie przyszła notyfikacja
  * <!-- .element: class="fragment fade-in" --> W celu oczekiwania na zmiennej warunku wymagana co najmniej blokada `unique_lock`, gdyż podczas uśpienia wątek ją odblokowuje, a gdy wybudza się, aby sprawdzić notyfikację blokuje ją ponownie na chwilę, po czym znów ją odblokowuje i śpi dalej
  * <!-- .element: class="fragment fade-in" --> Predykat dodany do funkcji `wait()` zapobiega fałszywym przebudzeniom, gdyż dodaje dodatkowy warunek, który musi być spełniony, aby wątek się wybudził
* <!-- .element: class="fragment fade-in" --> Utracona notyfikacja (lost wakeup)
  * <!-- .element: class="fragment fade-in" --> Jeśli notyfikacja została wysłana zanim wątek oczekiwał na zmiennej, to jest ona utracona i nie wybudzi ona wątku
  * <!-- .element: class="fragment fade-in" --> Problem można obejść, gdy pojawi się fałszywe przebudzenie.
  * <!-- .element: class="fragment fade-in" --> Jeśli wątek oczekiwał na zmiennej warunku z predykatem, to predykat musi być spełniony, inaczej fałszywe przebudzenie nie nastąpi
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/condition-variables">Spurious wakeup and lost wakeup - article on modernescpp.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/thread-synchronization-with-condition-variables-or-tasks">Condition variables vs tasks - article on modernescpp.com</a>
