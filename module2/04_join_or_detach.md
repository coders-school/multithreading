<!-- .slide: data-background="#111111" -->

# `join()` czy `detach()`?

___
<!-- .slide: style="font-size: 0.8em" -->

## `join()` czy `detach()`?

<div class="multicolumn">
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        this_thread::sleep_for(1s);
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    return 0;
}
```

```output
$> g++ 04a_join.cpp –lpthread
$> ./a.out
Doing something in casualJob
Thread job done
```

</div>
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        this_thread::sleep_for(1s);
        cout << "Thread job done" << endl;
    });
    t.detach();
    casualJob();
    return 0;
}
```

```output
$> g++ 04b_detach.cpp –lpthread
$> ./a.out
Doing something in casualJob
```

</div>
</div>

___
<!-- .slide: style="font-size: 0.8em" -->

## `join()` czy `detach()`?

<div class="multicolumn">
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    this_thread::sleep_for(1s);
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    return 0;
}
```

```output
$> g++ 04c_join.cpp –lpthread
$> ./a.out
Thread job done
Doing something in casualJob
```

</div>
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    this_thread::sleep_for(1s);
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    t.detach();
    casualJob();
    return 0;
}
```

```output
$> g++ 04d_detach.cpp –lpthread
$> ./a.out
Thread job done
Doing something in casualJob
```

</div>
</div>

___
<!-- .slide: style="font-size: 0.8em" -->

## `join()` czy `detach()`?

<div class="multicolumn">
<div class="col">

```cpp
#include <thread>
#include <iostream>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    // no join() or detach()
    casualJob();

    return 0;
}
```

```output
$> g++ 05_no_join_no_detach.cpp –lpthread
$> ./a.out
Thread job done
Doing something in casualJob
terminate called without an active exception
Aborted (core dumped)
```

</div>
<div class="col">

```cpp
#include <thread>
#include <iostream>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    t.detach();
    return 0;
}
```

```output
$> g++ 05_join_and_detach.cpp –lpthread
$> ./a.out
Doing something in casualJob
Thread job done
terminate called after throwing an instance of
'std::system_error'
    what():  Invalid argument
Aborted (core dumped)
```

</div>
</div>

___

## `join()` czy `detach()`?

* <!-- .element: class="fragment fade-in" --> Wątek należy zawsze przyłączyć – <code>join()</code> lub odłączyć – <code>detach()</code>. Zawsze.
* <!-- .element: class="fragment fade-in" --> Destruktor wątku nie przyłącza go ani nie odłącza (brak RAII 😔)
* <!-- .element: class="fragment fade-in" --> Brak przyłączenia lub odłączenia wątku spowoduje zawołanie <code>std::terminate()</code>, które ubija aplikację
* <!-- .element: class="fragment fade-in" --> Metoda wątku <code>joinable()</code> zwraca <code>true</code>, jeśli można zrobić <code>join()</code>
* <!-- .element: class="fragment fade-in" --> <code>join()</code> można zrobić tylko raz i wyklucza się on z <code>detach()</code>. Należy użyć albo jednego albo drugiego
* <!-- .element: class="fragment fade-in" --> Jeśli wątek odłączamy, to zazwyczaj robimy to od razu po jego utworzeniu. Po odłączeniu nie możemy się już odwołać do wątku używając jego zmiennej
* <!-- .element: class="fragment fade-in" --> Jeśli wątek przyłączamy, to musimy wybrać właściwe miejsce na jego przyłączenie. <code>join()</code> jest operacją blokującą, która czeka, aż wątek zakończy pracę, więc zazwyczaj robi się to na końcu funkcji odpalającej wątek. Jeśli funkcja ta zwraca wątek, to można go przyłączyć jeszcze później.
