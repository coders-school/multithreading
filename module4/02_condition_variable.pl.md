<!-- .slide: data-background="#111111" -->

# Multithreading

## Moduł 4

### Zmienna warunku

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zmienna warunku (condition variable)

* <!-- .element: class="fragment fade-in" --> <code>#include <condition_variable></code>
* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable</code>
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje
  * <!-- .element: class="fragment fade-in" --> <code>wait()</code> – oczekuje na zmianę - blokuje obecny wątek dopóki nie zostanie on wybudzony
  * <!-- .element: class="fragment fade-in" --> <code>notify_one()</code> – wybudza jeden z wątków oczekujących na zmianę. Nie mamy kontroli nad tym, który z wątków zostanie powiadomiony.
  * <!-- .element: class="fragment fade-in" --> <code>notify_all()</code> – wybudza wszystkie wątki czekające na zmianę. Wątki te mogą konkurować o zasoby.

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

* <!-- .element: class="fragment fade-in" --> Popraw kod z pliku <code>02_wait_queue.cpp</code> tak, aby używał zmiennej warunkowej zamiast aktywnego czekania

</div> <!-- .element: class="fragment fade-in" -->

</div>

___

### Zadanie 2 - rozwiązanie

<div style="display: flex;">

<div style="width: 50%; font-size: .8em;">

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

<div style="width: 50%; font-size: .8em;">

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

</div>

___

### Zmienne warunku uszczęśliwiają foczki 🙂

<img data-src="img/foczka.jpg" alt="foczka" class="plain">

___

### Zmienna warunku - szczegóły

* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable</code> działa tylko z wyłącznymi blokadami (<code>unique_lock</code>)
* <!-- .element: class="fragment fade-in" --> <code>std::condition_variable_any</code> działa z każdym rodzajem blokad (<code>shared_lock</code>)
* <!-- .element: class="fragment fade-in" --> Są niekopiowalne
* <!-- .element: class="fragment fade-in" --> Metoda <code>wait()</code> przyjmuje blokadę oraz opcjonalnie predykat, dzięki któremu zostaną wybudzone tylko te wątki, dla których warunek jest spełniony
* <!-- .element: class="fragment fade-in" --> Wszystkie wątki, które czekają na zmiennej warunku muszą mieć zablokowany ten sam mutex. W przeciwnym wypadku jest niezdefiniowane zachowanie.
* <!-- .element: class="fragment fade-in" --> Metody <code>wait_for()</code> i <code>wait_until()</code> przyjmują jeszcze odpowiednio punkt w czasie lub okres czasu do którego wątki będą czekać na wybudzenie. Po upływie tego czasu wątki zostaną wybudzone.
* <!-- .element: class="fragment fade-in" --> Jeśli na zmiennej warunku czeka kilka wątków i każdy ma inny predykat, to użycie <code>notify_one()</code> może spowodować zakleszczenie. Wybudzony może zostać wątek, dla którego warunek nie został spełniony i jeśli żaden inny wątek nie zawoła <code>nofity_one()</code> lub <code>notify_all()</code> to wszystkie będą czekać.

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

</div>

<div style="width: 40%; font-size: .9em;">

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

</div>

___

### Wskazówki

Jeśli utkniesz:

* <!-- .element: class="fragment fade-in" --> Potrzebujesz mutexu i zmiennej warunkowej w klasie <code>PingPong</code>
* <!-- .element: class="fragment fade-in" --> Czekaj na zmiennej warunku za pomocą <code>wait_for()</code> w funkcji <code>stop()</code>
* <!-- .element: class="fragment fade-in" --> Sprawdzaj liczbę powtórzeń w wątkach ping i pong
* <!-- .element: class="fragment fade-in" --> Użyj dodatkowej zmiennej <code>bool</code>, która powie wszystkim wątkom, aby się zakończyły. gdy nastąpią wymagane warunki. Użyj tutaj typu <code>atomic<bool></code> (o nim później 🙂)
* <!-- .element: class="fragment fade-in" --> Wątki ping i pong powinny za pomocą <code>wait()</code> sprawdzać warunek, czy to ich kolej na działanie. Użyj dodatkowej zmiennej <code>bool</code>, która zostanie użyta w predykacie przekazanym do <code>wait()</code>.
* <!-- .element: class="fragment fade-in" --> Wątek pong powinien zakończyć program po osiągnięciu limitu odbić

___

### Zmienna warunku - zagrożenia

* <!-- .element: class="fragment fade-in" --> Fałszywe przebudzenie (spurious wakeup)
  * <!-- .element: class="fragment fade-in" --> Wątek czekający na zmiennej warunku cyklicznie co pewien okres czasu wybudza się i sprawcza czy nie przyszła notyfikacja
  * <!-- .element: class="fragment fade-in" --> W celu oczekiwania na zmiennej warunku wymagana co najmniej blokada <code>unique_lock</code>, gdyż podczas uśpienia wątek ją odblokowuje, a gdy wybudza się, aby sprawdzić notyfikację blokuje ją ponownie na chwilę, po czym znów ją odblokowuje i śpi dalej
  * <!-- .element: class="fragment fade-in" --> Predykat dodany do funkcji <code>wait()</code> zapobiega fałszywym przebudzeniom, gdyż dodaje dodatkowy warunek, który musi być spełniony, aby wątek się wybudził
* <!-- .element: class="fragment fade-in" --> Utracona notyfikacja (lost wakeup)
  * <!-- .element: class="fragment fade-in" --> Jeśli notyfikacja została wysłana zanim wątek oczekiwał na zmiennej, to jest ona utracona i nie wybudzi ona wątku
  * <!-- .element: class="fragment fade-in" --> Problem można obejść, gdy pojawi się fałszywe przebudzenie.
  * <!-- .element: class="fragment fade-in" --> Jeśli wątek oczekiwał na zmiennej warunku z predykatem, to predykat musi być spełniony, inaczej fałszywe przebudzenie nie nastąpi
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/condition-variables">Spurious wakeup and lost wakeup - article on modernescpp.com</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://www.modernescpp.com/index.php/thread-synchronization-with-condition-variables-or-tasks">Condition variables vs tasks - article on modernescpp.com</a>
