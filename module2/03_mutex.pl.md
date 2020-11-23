<!-- .slide: data-background="#111111" -->

# Mutex

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Mutex - unikanie wyścigów

* <!-- .element: class="fragment fade-in" --> Mutex (Mutual Exclusion) - wzajemne wykluczanie
* <!-- .element: class="fragment fade-in" --> Implementacja blokady
* <!-- .element: class="fragment fade-in" --> <code>#include &lt;mutex&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::mutex</code>
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
  * <!-- .element: class="fragment fade-in" --> <code>void lock()</code> - zablokowanie mutexu. Operacja blokująca. Jeśli mutex jest zablokowany przez inny wątek to oczekujemy na odblokowanie.
  * <!-- .element: class="fragment fade-in" --> <code>void unlock()</code> - odblokowanie mutexu.
  * <!-- .element: class="fragment fade-in" --> <code>bool try_lock()</code> - zablokowanie mutexu. Operacja nieblokująca. Jeśli mutex jest już zablokowany przez inny wątek to kontynuuje dalsze wykonanie wątku. Zwraca true, jeśli udało się zablokować mutex

___

### Zadanie 2: zabezpieczenie zapisu do strumienia

<div style="display: flex;">

<div style="font-size: .8em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    cout << "Thread [" << id << "]: "
         << "Job done!" << endl;
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 99%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Zabezpiecz kod tak, aby każdy wątek mógł bezpiecznie wpisać do strumienia swój pełny tekst.
* <!-- .element: class="fragment fade-in" --> Nie powinny być możliwe przypadki takie jak poniższe

```bash
$> g++ 02_threads_in_collection.cpp -lpthread
$> ./a.out
...
Thread [10]: Job done!
Thread [9]: Thread [Job done!11]: Job done!
Thread [6]: Job done!
...
$> ./a.out
...
Thread [Thread [5]: Job done!
Thread [13]: Job done!
17]: Job done!
...
```
<!-- .element: class="fragment fade-in" style="width: 100%;" -->

</div>

</div>

___

### Zadanie 2 - rozwiązanie z użyciem mutexu

<div style="display: flex;">

<div style="font-size: .8em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    m.lock();
    cout << "Thread [" << id << "]: "
         << "Job done!" << endl;
    m.unlock();
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Działa... w tym przypadku.
* <!-- .element: class="fragment fade-in" --> Kod zablokowany mutexem to tzw. sekcja krytyczna.
* <!-- .element: class="fragment fade-in" --> Czas trwania blokady musi być możliwie najkrótszy
  * <!-- .element: class="fragment fade-in" --> błędem jest blokowanie funkcji <code>sleep_for()</code>.
* <!-- .element: class="fragment fade-in" --> Globalny mutex
  * <!-- .element: class="fragment fade-in" --> zazwyczaj mutexy umieszcza się w klasach, których operacje muszą być blokowane.
* <!-- .element: class="fragment fade-in" --> Co jeśli podczas trwania blokady nastąpi wyjątek?
  * <!-- .element: class="fragment fade-in" --> nie zostanie zawołane <code>unlock()</code>
  * <!-- .element: class="fragment fade-in" --> inne wątki nigdy nie skończą pracy
  * <!-- .element: class="fragment fade-in" --> brak RAII.
* <!-- .element: class="fragment fade-in" --> Czy można lepiej?

</div>

</div>

___

### Zadanie 2 - rozwiązanie z użyciem `lock_guard`

<div style="display: flex;">

<div style="font-size: .75em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

using namespace std;

void do_work(ind id, mutex & m) {
    this_thread::sleep_for(100ms);
    lock_guard<mutex> lock(m);
    cout << "Thread [" << id << "]: "
         << "Job done!" << endl;
}

int main() {
    mutex m;
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i, ref(m)));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Działa.
* <!-- .element: class="fragment fade-in" --> Bezpieczne w przypadku wystąpienia wyjątku  - jest RAII.
* <!-- .element: class="fragment fade-in" --> Mutex przekazany przez referencję.
* <!-- .element: class="fragment fade-in" --> Formatowanie i składanie tekstu podczas trwania blokady trochę trwa - strumienie nie słyną z wydajności.
* <!-- .element: class="fragment fade-in" --> Czy można szybciej?

</div>

</div>

___

### Zadanie 2 - rozwiązanie z użyciem `stringstream`

<div style="display: flex;">

<div style="font-size: .75em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>

using namespace std;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    stringstream ss;
    ss << "Thread [" << id << "]: "
       << "Job done!" << endl;
    cout << ss.rdbuf();
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Działa.
* <!-- .element: class="fragment fade-in" --> Formatowanie tekstu poza strumieniem wyjściowym.
* <!-- .element: class="fragment fade-in" --> Mutex jest zbędny
* <!-- .element: class="fragment fade-in" --> Pojedyncza operacja zapisu do strumienia jest niepodzielna.

</div>

</div>

___

## Sekcja krytyczna

```c++
class SafeList {
    mutex m;
public:
    void remove_safely(Node* node) {
        lock_guard<mutex> lock(m);
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    } // automatic unlocking
    // other methods
};
```

* <!-- .element: class="fragment fade-in" --> Sekcja krytyczna to fragment programu, który może być wykonywany tylko przez 1 wątek na raz
* <!-- .element: class="fragment fade-in" --> Zazwyczaj jest realizowana z użyciem mutexu jako blokady dostępu
* <!-- .element: class="fragment fade-in" --> Zawsze używaj menadżera blokady (np. <code>lock_guard<mutex></code>) w celu zapewnienia mechanizmu RAII
* <!-- .element: class="fragment fade-in" --> Całą sekcję krytyczną zazwyczaj wydziela się do osobnej funkcji

___
<!-- .slide: style="font-size: .85em" -->
## Rodzaje mutexów

* <!-- .element: class="fragment fade-in" --> <code>mutex</code>
  * <!-- .element: class="fragment fade-in" --> <code>void lock()</code> - bieżący wątek jest wstrzymany, aż do momentu pozyskania blokady
  * <!-- .element: class="fragment fade-in" --> <code>void unlock()</code> - jeżeli bieżący wątek jest posiadaczem blokady, to następuje jej zwolnienie
  * <!-- .element: class="fragment fade-in" --> <code>bool try_lock()</code> - próba pozyskania blokady bez wstrzymywania bieżącego wątku. Zwraca true jeśli blokada została pozyskana, w przeciwnym wypadku zwraca false
* <!-- .element: class="fragment fade-in" --> <code>timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> posiada metody umożliwiające zdefiniowanie maksymalnego czasu oczekiwania na pozyskanie blokady przez wątek
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_until(timeout_time)</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_for(timeout_duration)</code>
* <!-- .element: class="fragment fade-in" --> <code>recursive_mutex</code>
  * <!-- .element: class="fragment fade-in" --> ten sam wątek może wielokrotnie pozyskać mutex poprzez wywołanie metody <code>lock()</code> lub <code>try_lock()</code>
  * <!-- .element: class="fragment fade-in" --> aby zwolnić mutex wątek musi odpowiednią ilość razy wywołać <code>unlock()</code>
* <!-- .element: class="fragment fade-in" --> <code>recursive_timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> posiada własności <code>timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> posiada własności <code>recursive_mutex</code>
* <!-- .element: class="fragment fade-in" --> <code>shared_mutex</code>
  * <!-- .element: class="fragment fade-in" --> możliwość pozyskiwania blokad współdzielonych przy pomocy metod:
    * <!-- .element: class="fragment fade-in" --> <code>void lock_shared()</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared()</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared_for(rel_time)</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared_until(abs_time)</code>
    * <!-- .element: class="fragment fade-in" --> <code>void unlock_shared()</code>

___

## Blokada współdzielona - `shared_mutex`

<div style="display: flex;">

<div style="font-size: .8em; width: 99%">

```c++
#include <deque>
#include <shared_mutex>
std::deque<int> ids;
std::shared_mutex mtxIds;
int getIdsIndex() { /* ... */ }
void process(int) { /* ... */ }
int newValue() { /* ... */ }
void reader() {
    int index = getIdsIndex();
    std::shared_lock<std::shared_mutex> lock(mtxIds);
    int value = ids[index];
    lock.unlock();
    process(value);
}
void writer() {
    int index = getIdsIndex();
    std::lock_guard<std::shared_mutex> lock(mtxIds);
    ids[index] = newValue();
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Współdzielone blokady używane są w trakcie czytania danych
  * <!-- .element: class="fragment fade-in" --> <code>shared_lock<shared_mutex></code>
* <!-- .element: class="fragment fade-in" --> Do zapisu danych trzeba pozyskać wyłączną blokadę
  * <!-- .element: class="fragment fade-in" --> <code>lock_guard<shared_mutex></code>
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock<shared_mutex></code>

</div>

</div>
