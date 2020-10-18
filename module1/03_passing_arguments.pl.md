<!-- .slide: data-background="#111111" -->
# Przekazywanie parametrów

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

* <!-- .element: class="fragment fade-in" --> Jak przekazać wynik obliczeń z powrotem do funkcji <code>main()</code>?
  * <!-- .element: class="fragment fade-in" --> Nie da się poprzez <code>return</code>, wątki do tego nie służą
  * <!-- .element: class="fragment fade-in" --> Można zapisać coś w globalnej zmiennej, ale to proszenie się o problemy – synchronizacja
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
void add10(int & a) {
    a += 10;
}

int main() {
    int five = 5;
    thread t(add10, ref(five));
    cout << five << endl;
    t.join();
    cout << five << endl;
    return 0;
}
```

```output
$> g++ zadanie2.cpp –lpthread
$> ./a.out
5
15
```

* <!-- .element: class="fragment fade-in" --> <code>std::ref()</code> powoduje, że przekazujemy obiekt przez referencję

___
<!-- .slide: style="font-size: 0.8em" -->
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
<!-- .slide: style="font-size: 0.85em" -->
## Zadanie 3 - rozwiązanie

```cpp
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
<!-- .element: class="fragment fade-in" -->

___

## Zadanie 3 - rozwiązanie

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
* <!-- .element: class="fragment fade-in" --> Metoda klasy odpalana w wątku jako pierwszy ukryty parametr przyjmuje wskaźnik do obiektu, na którym ma zostać wywołana.

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

## Pułapki podczas przekazywania argumentów

* Jakie pułapki kryje kod poniżej?

```cpp
void f(int i, std::string const& s);
void oops(int arg)
{
    char buffer[1024];
    sprintf(buffer, "%i", arg);
    std::thread t(f, 3, buffer);
    t.detach();
}
```
<!-- .element: class="fragment fade-in" -->

___

## Pułapki podczas przekazywania argumentów

* <!-- .element: class="fragment fade-in" --> Należy zwrócić uwagę na przekazywane argumenty
  * <!-- .element: class="fragment fade-in" --> Jeżeli zmienna jest wskaźnikiem/referencją, należy zadbać aby długość jej życia była dłuższa niż wątku, który na niej operuje.
  * <!-- .element: class="fragment fade-in" --> Jeżeli istnieje ryzyko niejawnej konwersji, najlepiej od razu przekazać przekonwertowany argument.

```cpp
void f(int i, std::string const& s);
void not_oops(int arg)
{
    char buffer[1024];
    sprintf(buffer, "%i", arg);
    std::thread t(f, 3, std::string(buffer));
    t.detach();
}
```
<!-- .element: class="fragment fade-in" -->
