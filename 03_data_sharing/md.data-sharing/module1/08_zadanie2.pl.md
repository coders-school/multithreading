<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie 2: zabezpieczenie zapisu do strumienia

<div style="display: flex;">

<div>

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    cout << "Thread [" << id << "]: " << "Job done!" << endl;
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
</div><!-- .element: style="font-size: .9em;" -->

<div>

<div>

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
<!-- .element: class="fragment fade-in" style="width: 100%; transform: scale(.98);" -->
</div><!-- .element: class="fragment fade-in" style="font-size: .9em; background-color: #8B3536; padding: 9px 0 0 0; margin: 22px 0;" -->

</div>

</div>

___

### Zadanie 2 - rozwiązanie z użyciem mutexu

<div style="display: flex;">

<div>

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
    cout << "Thread [" << id << "]: " << "Job done!" << endl;
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
</div><!-- .element: style="font-size: .72em;" -->

<div>

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

</div><!-- .element: class="fragment fade-in" style="background-color: #8B3536;" -->

</div>

___

### Zadanie 2 - rozwiązanie z użyciem `lock_guard`

<div style="display: flex;">

<div>

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
    cout << "Thread [" << id << "]: " << "Job done!" << endl;
}

int main() {
    mutex m;
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i ,ref(m)));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div><!-- .element: style="font-size: .72em;" -->

<div>

* <!-- .element: class="fragment fade-in" --> Działa.
* <!-- .element: class="fragment fade-in" --> Bezpieczne w przypadku wystąpienia wyjątku  - jest RAII.
* <!-- .element: class="fragment fade-in" --> Mutex przekazany przez referencję.
* <!-- .element: class="fragment fade-in" --> Formatowanie i składanie tekstu podczas trwania blokady trochę trwa - strumienie nie słyną z wydajności.
* <!-- .element: class="fragment fade-in" --> Czy można szybciej?

</div><!-- .element: class="fragment fade-in" style="background-color: #8B3536;" -->

</div>
___

### Zadanie 2 - rozwiązanie z użyciem `stringstream`

<div style="display: flex;">

<div>

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
    ss << "Thread [" << id << "]: " << "Job done!" << endl;
    //lock_guard<mutex> lock(m);
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
</div><!-- .element: style="font-size: .72em;" -->

<div>

* <!-- .element: class="fragment fade-in" --> Działa.
* <!-- .element: class="fragment fade-in" --> Formatowanie tekstu poza strumieniem wyjściowym.
* <!-- .element: class="fragment fade-in" --> Mutex jest zbędny
    * <!-- .element: class="fragment fade-in" --> Sama pojedyncza operacja zapisu do strumienia jest niepodzielna.

</div><!-- .element: class="fragment fade-in" style="background-color: #8B3536;" -->

</div>
