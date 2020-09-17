## Wątek – std::thread

* <!-- .element: class="fragment fade-in" --> Wątek jest obiektem
* <!-- .element: class="fragment fade-in" --> <code>#include <thread></code>
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code>
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
  * <!-- .element: class="fragment fade-in" --> constructor – uruchamia wątek
  * <!-- .element: class="fragment fade-in" --> <code>get_id()</code> – pobranie identyfikatora wątku
  * <!-- .element: class="fragment fade-in" --> <code>join()</code> – przyłączenie wątku
  * <!-- .element: class="fragment fade-in" --> <code>detach()</code> – odłączenie wątku
  * <!-- .element: class="fragment fade-in" --> <code>joinable()</code> – czy można przyłączyć wątek

___

## Wątek – std::thread

* <!-- .element: class="fragment fade-in" --> Funkcje i klasy pomocnicze dla wątków w bibliotece standardowej
  * <!-- .element: class="fragment fade-in" --> <code>std::thread::hardware_concurrency()</code> - zwraca liczbę dostępnych wątków współbieżnych. Funkcja ta może zwrócić 0, jeśli taka informacja nie będzie możliwa do uzyskania. Utworzenie większej liczby wątków jest możliwe i nazywa się "oversubscription". Efekty przełączania kontekstu mogą mieć jednak negatywny wpływ na wydajność.
* <!-- .element: class="fragment fade-in" --> <code>std::this_thread</code>
  * <!-- .element: class="fragment fade-in" --> <code>sleep_for(const chrono::duration<Rep, Period>& sleep_duration)</code> - wstrzymuje wykonanie bieżącego wątku na (przynajmniej) określony interwał czasu
  * <!-- .element: class="fragment fade-in" --> <code>sleep_until(const chrono::time_point<Clock, Duration>& sleep_time)</code> - blokuje wykonanie wątku przynajmniej do podanego jako parametr punktu czasu
  * <!-- .element: class="fragment fade-in" --> <code>yield()</code> - funkcja umożliwiające podjęcie próby wywłaszczenia bieżącego wątku i przydzielenia czasu procesora innemu wątkowi
  * <!-- .element: class="fragment fade-in" --> <code>get_id()</code> - zwraca obiekt typu std::thread::id reprezentujący identyfikator bieżącego wątku

___

## Podstawowe użycie

```cpp
#include <thread>
#include <iostream>
using namespace std;

void action()
{
    cout << "Hello ";
    cout << this_thread::get_id();
    cout << " thread" << endl;
}

int main()
{
    thread t(action);
    // can do other stuff here
    t.join();
    return 0;
}
```

```output
$> g++ 01_hello.cpp –lpthread
$> ./a.out
Hello 47082117789440 thread
```

___

## Podstawowe użycie z lambdą

```cpp
#include <thread>
#include <iostream>
using namespace std;

int main()
{
    thread t([]
    {
        cout << "Hello ";
        cout << this_thread::get_id();
        cout << " thread" << endl;
    });
    // can do other stuff here
    t.join();
    return 0;
}
```

```output
$> g++ 02_hello_lambda.cpp –lpthread
$> ./a.out
Hello 47937732544256 thread
```

___

## Zadanie 1: przekazywanie parametrów

```cpp
#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    // run add function in a thread
    // pass 3 and 4 as arguments
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Utwórz wątek i odpal w nim funkcję <code>add()</code> przekazując do niej liczby 3 i 4

___

## Zadanie 1 - rozwiązanie

```cpp
#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    thread t(add, 5, 6);
    t.join();
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Jak przekazać wynik obliczeń z powrotem do funkcji main()?
  * <!-- .element: class="fragment fade-in" --> Nie da się poprzez return, wątki do tego nie służą
  * <!-- .element: class="fragment fade-in" --> Można zapisać coś w globalnej zmiennej, ale to proszenie się problemy – synchronizacja
  * <!-- .element: class="fragment fade-in" --> Właściwy sposób to przekazanie jako parametr referencję do zmiennej, którą zmodyfikujemy w wątku

___

## Zadanie 2: przekazywanie referencji

```cpp
#include <thread>
#include <iostream>
using namespace std;

void add10(int & a)
{
    a += 10;
}

int main()
{
    // run add10 function in a thread
    // pass 5 as an argument and read it's value
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Utwórz wątek i odpal w nim funkcję <code>add10()</code> przekazując do niej liczbę 5
* <!-- .element: class="fragment fade-in" --> Wypisz wynik na ekran

___

## Zadanie 2 - rozwiązanie

```cpp
#include <thread>
#include <iostream>
using namespace std;

void add10(int & a)
{
    a += 10;
}

int main()
{
    int five = 5;
    thread t(add10, ref(five));
    cout << five << endl;
    t.join();
    cout << five << endl;
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> <code>std::ref()</code> powoduje, że przekazujemy obiekt przez referencję

```output
$> g++ zadanie2.cpp –lpthread
$> ./a.out
5
15
```

___

## Zadanie 3: przekazywanie metody klasy

```cpp
#include <thread>
#include <iostream>
#include <string>
using namespace std;

class Car {
    int production_year;
    string model_name;
public:
    void setData(int year, const string & model) {
        production_year = year;
        model_name = model;
    }
    void print() {
        cout << model_name << " " << production_year << endl;
    }
};

int main() {
    Car toyota;
    // set year to 2015, model to "Corolla" in a thread
    toyota.print();
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Utwórz wątek i odpal w nim metodę <code>setData()</code>, która ustawi w obiekcie toyota rok produkcji na 2015, a model na "Corolla"

___

## Zadanie 3 - rozwiązanie

```cpp
#include <thread>
#include <iostream>
#include <string>
using namespace std;

class Car {
    int production_year;
    string model_name;
public:
    void setData(int year, const string & model) {
        production_year = year;
        model_name = model;
    }
    void print() {
        cout << model_name << " " << production_year << endl;
    }
};

int main() {
    Car toyota;
    thread t(&Car::setData, &toyota, 2015, "Corolla");
    t.join();
    toyota.print();
    return 0;
}
```

```output
$> g++ zadanie3.cpp –lpthread
$> ./a.out
Corolla 2015
```

* <!-- .element: class="fragment fade-in" --> Dlaczego przy parametrze "Corolla" nie ma <code>std::ref()</code>?
* <!-- .element: class="fragment fade-in" --> obiekty tymczasowe można podpiąć pod <code>const &</code>
* <!-- .element: class="fragment fade-in" --> użycie <code>std::ref("Corolla")</code> da nam referencję do zmiennej tymczasowej (w tym przypadku jest to bezpieczne)
* <!-- .element: class="fragment fade-in" --> istnieje niebezpieczeństwo przekazania wiszącej referencji (dangling reference)

___

## Przekazywanie parametrów

* <!-- .element: class="fragment fade-in" --> Konstruktor wątku jako pierwszy parametr dostaje jakikolwiek "wołalny" obiekt (callable) - lambda, funkcja, funktor. Callable jest kopiowane do pamięci wątku.
* <!-- .element: class="fragment fade-in" --> Kolejne parametry konstruktora wątku są przekazywane do funkcji (lambdy)
* <!-- .element: class="fragment fade-in" --> Parametry są forwardowane (kopiowane lub przenoszone) do pamięci wątku.
* <!-- .element: class="fragment fade-in" --> Przekazanie referencji odbywa się przez użycie <code>std::ref()</code>
* <!-- .element: class="fragment fade-in" --> Przekazanie stałej referencji odbywa się przez użycie <code>std::cref()</code>
* <!-- .element: class="fragment fade-in" --> Metoda klasy odpalana w wątku jako pierwszy ukryty parametr przyjmuje wskaźnik do obiektu, na którym ma zostać wywołana

___

## Przekazywanie parametrów

```cpp
#include <thread>

void foo() { /* ... */ }
// foo() – function without parameters
std::thread t1(&foo);

void bar(int a, int & b) { /* ... */ }
int field = 5;
// bar(1, field) – function with parameters
std::thread t2(&bar, 1, std::ref(field));

struct SomeClass {
    void method(int a, int b, int & c) { /* ... */ }
};
SomeClass someObject;
// someObject.method(1, 2, field) – class method
std::thread t3(&SomeClass::method, someObject, 1, 2, std::ref(field));
```

___

## Puste wątki (Not-A-Thread)

```cpp
#include <thread>
#include <iostream>
using namespace std;
int main() {
thread t;
t.join(); // not allowed on an empty thread
t.detach(); // not allowed on an empty thread
return 0;
}
```

```output
$> g++ 03_join_empty_thread.cpp –lpthread
$> ./a.out
terminate called after throwing an instance of 'std::system_error'
    what():  Invalid argument
Aborted (core dumped)
```

* <!-- .element: class="fragment fade-in" --> Wątki są odpalane od razu po ich utworzeniu, o ile tylko przekażemy do nich tzw. thread of execution lub callable (funkcja, funktor, lambda). Są one powiązane z wątkami systemowymi.
* <!-- .element: class="fragment fade-in" --> Wątki są przypięte do swojej zmiennej w wątku, który go stworzył. Stworzenie pustego wątku <code>std::thread t;</code> nie odpala niczego.
* <!-- .element: class="fragment fade-in" --> Pusty wątek (Not-A-Thread) nie jest powiązany z żadnym wątkiem systemowym i nie woła się na nim <code>join()</code> lub <code>detach()</code>

___

## join() czy detach()?

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

___

## join() czy detach()?

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

___

## join() czy detach()?

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
terminate called after throwing an instance of 'std::system_error'
    what():  Invalid argument
Aborted (core dumped)
```

___

## join() czy detach()?

* <!-- .element: class="fragment fade-in" --> Wątek należy zawsze przyłączyć – <code>join()</code> lub odłączyć – <code>detach()</code>. Zawsze.
* <!-- .element: class="fragment fade-in" --> Destruktor wątku nie przyłącza go ani nie odłącza (brak RAII 😔)
* <!-- .element: class="fragment fade-in" --> Brak przyłączenia lub odłączenia wątku spowoduje zawołanie <code>std::terminate()</code>, które ubija aplikację
* <!-- .element: class="fragment fade-in" --> Metoda wątku <code>joinable()</code> zwraca <code>true</code>, jeśli można zrobić <code>join()</code>
* <!-- .element: class="fragment fade-in" --> <code>join()</code> można zrobić tylko raz i wyklucza się on z <code>detach()</code>. Należy użyć albo jednego albo drugiego
* <!-- .element: class="fragment fade-in" --> Jeśli wątek odłączamy, to zazwyczaj robimy to od razu po jego utworzeniu. Po odłączeniu nie możemy się już odwołać do wątku używając jego zmiennej
* <!-- .element: class="fragment fade-in" --> Jeśli wątek przyłączamy, to musimy wybrać właściwe miejsce na jego przyłączenie. <code>join()</code> jest operacją blokującą, która czeka, aż wątek zakończy pracę, więc zazwyczaj robi się to na końcu funkcji odpalającej wątek. Jeśli funkcja ta zwraca wątek, to można go przyłączyć jeszcze później.

___

## RAII

* <!-- .element: class="fragment fade-in" --> Resource Acquisition Is Initialization
* <!-- .element: class="fragment fade-in" --> Idiom (wzorzec) języka C++ gwarantujący bezpieczeństwo obsługi zasobów
* <!-- .element: class="fragment fade-in" --> Pozyskanie zasobu w konstruktorze
* <!-- .element: class="fragment fade-in" --> Zwolnienie zasobu w destruktorze
* <!-- .element: class="fragment fade-in" --> Automatyczne zwolnienie zasobu przy wystąpieniu wyjątku, dzięki mechanizmowi odwijania stosu
* <!-- .element: class="fragment fade-in" --> Znane klasy implementujące RAII:
  * <!-- .element: class="fragment fade-in" --> <code>unique_ptr</code> – wrapper na zwykły wskaźnik
  * <!-- .element: class="fragment fade-in" --> <code>shared_ptr</code> – wrapper na zwykły wskaźnik
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock</code> – wrapper na <code>mutex</code>
  * <!-- .element: class="fragment fade-in" --> <code>fstream</code> - wrapper na plik
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> nie implementuje RAII 😔
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> ma zablokowaną operację kopiowania
* <!-- .element: class="fragment fade-in" --> <code>std::thread</code> może być przenoszony tak jak <code>unique_ptr</code> (semantyka przenoszenia, <code>std::move</code>)

___

## Zadanie 4: scoped_thread

```cpp
#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    // your implementation goes here
};

void do_sth(int) {
    this_thread::sleep_for(1s);
}

void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}

int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    auto st3 = move(st);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed
```

* <!-- .element: class="fragment fade-in" --> Napisz mechanizm RAII na wątek - <code>scoped_thread</code>
* <!-- .element: class="fragment fade-in" --> Jakie operacje powinny zostać uwzględnione?
  * <!-- .element: class="fragment fade-in" --> konstruktor przyjmuje zasób – <code>std::thread</code>
  * <!-- .element: class="fragment fade-in" --> konstruktor nie pozwala utworzyć obiektu, jeśli przekażemy pusty wątek
  * <!-- .element: class="fragment fade-in" --> destruktor woła <code>join()</code>
  * <!-- .element: class="fragment fade-in" --> kopiowanie jest zabronione
  * <!-- .element: class="fragment fade-in" --> przenoszenie jest dozwolone
* <!-- .element: class="fragment fade-in" --> Kopiowanie wątków jest operacją usuniętą, kompilator na to nie pozwoli
* <!-- .element: class="fragment fade-in" --> Przenoszenie wątków jest dozwolone

___

## Zadanie 4 - rozwiązanie

```cpp
#include <thread>
#include <stdexcept>
#include <chrono>
#include <iostream>
using namespace std;

class scoped_thread {
    std::thread t_;
public:
    explicit scoped_thread(std::thread t)
        : t_(std::move(t))
    {
        if (not t_.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread(scoped_thread &&) = default;
    scoped_thread& operator=(const scoped_thread &) = delete;
    scoped_thread& operator=(scoped_thread &&) = default;
};
void do_sth(int) {
    this_thread::sleep_for(1s);
}
void do_sth_unsafe_in_current_thread() {
    throw runtime_error("Whoa!");
}
int main() {
    scoped_thread st(std::thread(do_sth, 42));
    // auto st2 = st; // copying not allowed
    auto st3 = move(st);
    try {
        do_sth_unsafe_in_current_thread();
    } catch (const exception & e) {
        cout << e.what() << endl;
    }
    return 0;
} // thread is safely destroyed
```

___

## Copy elision (RVO)

```cpp
#include <thread>

std::thread f() {
    return std::thread([]{});
}

int main() {
    auto t = f(); // copy elision - ok
    // auto t2 = t;         // copying not allowed
    auto t3 = std::move(t); // moving is ok
    t3.join(); // join on t3, not t
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Kopiowanie wątku jest zabronione
* <!-- .element: class="fragment fade-in" --> Zwracanie kopii z funkcji podlega zasadom copy elision – kompilator optymalizuje kod, poprzez wyrzucenie zbędnego kopiowania
* <!-- .element: class="fragment fade-in" --> RVO (Return Value Optimization) to szczególny przypadek copy elision
* <!-- .element: class="fragment fade-in" --> Jeśli zmienna lokalna utworzona w funkcji jest zwraca przez kopię nastąpi RVO
* <!-- .element: class="fragment fade-in" --> Zmienna zostanie od razu utworzona w odpowiednim miejscu na stosie, gdzie jest możliwy dostęp do niej z poziomu wyższej ramki stosu
* <!-- .element: class="fragment fade-in" --> Dzięki RVO można zwracać wątki z funkcji poprzez kopię

___

## Problem - wisząca referencja

```cpp
#include <thread>

void do_sth(int i) { /* ... */ }

struct A {
    int& ref_;
    A(int& a) : ref_(a) {}
    void operator()() {
        do_sth(ref_); // potential access to
                      // a dangling reference
    }
};

std::thread create_thread() {
    int local = 0;
    A worker(local);
    std::thread t(worker);
    return t;
} // local is destroyed, reference in worker is dangling

int main() {
    auto t = create_thread(); // Undefined Behavior
    auto t2 = create_thread(); // Undefined Behavior
    t.join();
    t2.join();
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Trzeba zapewnić, że wątek ma poprawny dostęp do zasobów z których korzysta w czasie swojego życia, czyli np. coś nie jest usuwane wcześniej. To nie powinno być zaskoczeniem, bo nawet w jednowątkowej aplikacji trzeba o to dbać, inaczej mamy Undefined Behavior (UB).
* <!-- .element: class="fragment fade-in" --> Taki przypadek zachodzi, gdy wątek trzyma wskaźniki lub referencje do lokalnych obiektów i wątek ciągle żyje, gdy wychodzimy z lokalnej funkcji.
* <!-- .element: class="fragment fade-in" --> Kopiowanie danych do wątku jest bezpieczne. Jeśli pracujesz na małych porcjach danych nie wymagających modyfikacji zawsze preferuj kopiowanie.
* <!-- .element: class="fragment fade-in" --> Zobacz <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp31-pass-small-amounts-of-data-between-threads-by-value-rather-than-by-reference-or-pointer>C++ Core Guidelines [CP.31]</a>

___

## Problem - wyjątki w wątkach

```cpp
#include <thread>
#include <iostream>

int main() {
    try {
        std::thread t1([]{
            throw std::runtime_error("WTF - What a Terrible Failure");
        });
        t1.join();
    } catch (const std::exception & ex) {
        std::cout << "Thread exited with exception: " << ex.what() << "\n";
    }
    return 0;
}
```

```output
$> g++ 09_exceptions_not_working.cpp –lpthread
$> ./a.out
terminate called after throwing an instance of 'std::runtime_error'
    what():  WTF - What a Terrible Failure
Aborted (core dumped)
```

___

## Problem - wyjątki w wątkach

```cpp
#include<iostream>
#include<thread>
#include<exception>
#include<stdexcept>

int main()
{
    std::exception_ptr thread_exception = nullptr;
    std::thread t([](std::exception_ptr & te) {
        try {
            throw std::runtime_error("WTF");
        } catch (...) {
            te = std::current_exception();
        }
    }, std::ref(thread_exception));
    t.join();
    if (thread_exception) {
        try {
            std::rethrow_exception(thread_exception);
        } catch (const std::exception & ex) {
            std::cout << "Thread exited with an exception: "
            << ex.what() << "\n";
        }
    }
    return 0;
}
```

```output
$> g++ 10_exceptions_working.cpp –lpthread
$> ./a.out
Thread exited with an exception: WTF
```

* <!-- .element: class="fragment fade-in" --> Nie można standardowo złapać wyjątków w innym wątku niż tym, który rzucił wyjątek
* <!-- .element: class="fragment fade-in" --> Aby przechwycić wyjątek rzucony z innego wątku należy użyć wskaźnika na wyjątek <code>-std::exception_ptr</code>
* <!-- .element: class="fragment fade-in" --> Wątek rzucający wyjątek powinien przypisać do wskaźnika na wyjątek obecny wyjątek za pomocą <code>std::current_exception()</code>
* <!-- .element: class="fragment fade-in" --> Wątek, który chce złapać wyjątek powinien sprawdzić, czy <code>std::exception_ptr</code> został ustawiony i jeśli tak jest rzucić ten wyjątek ponownie poprzez <code>std::rethrow_exception()</code>
* <!-- .element: class="fragment fade-in" --> Warto używać w wyjątkach funkcji <code>noexcept</code>, aby mieć pewność, że wyjątki nie będą rzucane

___

## Zadanie 5: wątki w kolekcjach

* <!-- .element: class="fragment fade-in" --> Napisz krótki program, w którym 20 wątków jest trzymane w wektorze.
* <!-- .element: class="fragment fade-in" --> Każdy wątek ma za zadanie poczekać 1 sekundę, po czym wyświetlić swój numer, który przyjmuje jako parametr oraz znak nowej linii.
* <!-- .element: class="fragment fade-in" --> Tworzenie wątków i ich przyłączanie powinno zostać zrealizowane w 2 oddzielnych pętlach
* <!-- .element: class="fragment fade-in" --> Uruchom program kilka razy i zaobserwuj jakie daje wyniki

___

## Zadanie 5 - rozwiązanie

```cpp
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

void do_work(int id) {
    this_thread::sleep_for(1s);
    cout << id << endl;
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

* <!-- .element: class="fragment fade-in" --> Napisz krótki program, w którym 20 wątków jest trzymane w wektorze.
* <!-- .element: class="fragment fade-in" --> Każdy wątek ma za zadanie poczekać 1 sekundę, po czym wyświetlić swój numer, który przyjmuje jako parametr oraz znak nowej linii.
* <!-- .element: class="fragment fade-in" --> Tworzenie wątków i ich przyłączanie powinno zostać zrealizowane w 2 oddzielnych pętlach
* <!-- .element: class="fragment fade-in" --> Uruchom program kilka razy i zaobserwuj jakie daje wyniki

___

## Zadanie 5 – możliwe rezultaty

|$> ./a.out|$> ./a.out|$> ./a.out|$> ./a.out|
|:---------|:---------|:---------|:---------|
|0         |4         |18        |0         |
|1         |5         |0         |1         |
|3         |7         |16        |16        |
|2         |6         |2         |3         |
|5         |14        |3         |19        |
|6         |15        |4         |9         |
|8         |16        |5         |8         |
|10        |18        |6         |15        |
|12        |13        |15        |11        |
|7         |11        |8         |5         |
|11        |9         |9         |18        |
|9         |2         |10        |17        |
|13        |0         |11        |4         |
|4         |8         |12        |12        |
|14        |10        |13        |13        |
|15        |17        |14        |7         |
|17        |12        |19        |10        |
|16        |1         |1         |2         |
|19        |19        |7         |6         |
|18        |3         |17        |4         |

* <!-- .element: class="fragment fade-in" --> Dlaczego tak się dzieje?
* <!-- .element: class="fragment fade-in" --> Strumień wyjściowy cout jest tylko jeden. Jest on wspólnym zasobem współdzielonym między wątkami
* <!-- .element: class="fragment fade-in" --> Może dochodzić do przeplotów w trakcie dostępu do strumienia (jeden wątek zacznie coś wpisywać i nie skończy, a już drugi wejdzie mu w paradę i wpisze swój numer)
* <!-- .element: class="fragment fade-in" --> Współdzielenie zasobów to typowy problem wielowątkowości
* <!-- .element: class="fragment fade-in" --> Jak sobie z tym poradzić? To już temat na kolejną lekcję 🙂

___

## Przydatne linki

* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/thread/thread><code>std::thread</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/utility/functional/ref><code>std::ref</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp-concurrency-and-parallelism>C++ Core Guidelines on Concurrency and Parallelism</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/>Top 20 C++ multithreading mistakes and how to avoid them</a>
