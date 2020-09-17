## Czym jest współbieżność?

* <!-- .element: class="fragment fade-in" --> Dwa procesy są współbieżne jeżeli jeden z nich rozpoczyna się przed zakończeniem drugiego.

<img data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> współbieżność polega na jednoczesnym wykonywaniu co najmniej dwóch czynności.

___

## Współbieżność w systemach komputerowych

* „Symulacja” współbieżności w systemach jednoprocesorowych przy wykorzystaniu mechanizmu przełączania zadań (ang. Task switching),
* Komputery/serwery wieloprocesorowe/wielordzeniowe obsługujące wiele aplikacji jednocześnie.

<img data-src="img/02.png" src="img/02.png">

___

## Modele współbieżności

* Współbieżność z wieloma procesami:
  * Osobne procesy (komunikacja przez sygnały, pliki, potoki, gniazda itd.),
  * Osobna przestrzeń pamięci, bezpieczeństwo przetwarzania wielowątkowego,
  * Długi i skomplikowany proces uruchamiania nowego procesu.
* Współbieżność z wieloma wątkami:
  * Uruchamianie wielu wątków w obrębie jednego procesu, które współdzielą pamięć (współdzielona przestrzeń adresowa),
  * Należy samemu zagwarantować bezpieczeństwo dostępu do współdzielonej pamięci,
  * Uruchomienie wątku jest szybsze niż uruchomienie nowego procesu,
  * Prostsze i szybsze metody komunikowania się pomiędzy watkami w obrębie procesu

___

## Kiedy stosować współbieżność?

* Podział zagadnień (np. odtwarzacz Blu-ray),
* Wydajność:
  * Zrównoleglanie zadań (podział zadania na części),
  * Zrównoleglanie danych (wykonywanie tych samych zadań, na różnych fragmentach danych)

___

## Kiedy nie stosować współbieżności?

* Gdy zbyt wiele wątków może obniżyć wydajność zamiast ją zwiększyć (koszt uruchomienia wątku, koszt zasobów – każdy watek zajmuje pamięć typowo 8MB.),
* Gdy wzrost wydajności nie jest proporcjonalny do woźnego wysiłku i złożoności kodu (koszt utrzymania kodu jest równie istotny).

___

## Proces oraz wątek

* Proces służy do organizowania wykonywania programu. W skład jednego programu wchodzi jeden lub więcej procesów. Zatem proces jest to cały kontekst niezbędny do wykonania programu.
* Zmianie w wyniku wykonywania procesu ulega między innymi segment danych, segment stosu, stan rejestrów procesora.
* W momencie wykonywania procesu system operacyjny przydziela procesowi niezbędne zasoby (pamięć, czas procesora itp.).
* Synchronizacja, sposób obsługi procesów itp. Kontrolowana jest przez system operacyjny.
* W obrębie każdego procesu istnieje jeden lub więcej wątków.
* Wątki tego samego procesu współdzielą większość przestrzeni adresowej (segment kodu i danych, otwarte pliki Itp.).
* Przełączanie kontekstu wątku jest stosunkowo szybki i nie obciążające system operacyjny.
* Tworzenie wątku wymaga mniej zasobów do działania i jest szybsze niż tworzenie procesu.
* Łatwa (ale również niebezpieczna) komunikacja pomiędzy wątkami w obrębie jednego procesu.
* Każdy wątek posiada odrębny stos (adres powrotu z funkcji oraz zmienne lokalne).

___

## C++ i obsługa wielowątkowości

* Standard C++11/14/17/20 – Wprowadzenie i rozwój bibliotek odpowiadających za obsługę wielowątkowości,
* Przed C++11 trzeba było korzystać z różnych bibliotek, lub ręcznie odwoływać się do interfejsów API udostępniających mechanizmy wielowątkowe,
* C++11 wprowadził również nowy model pamięci przystosowany do przetwarzania wielowątkowego na wielu platformach.
* Wprowadzono zarządzanie wątkami (ang. Thread), ochronę współdzielonych danych, synchronizacje operacji wykonywanych przez wątki, wykonywanie niskopoziomowych operacji atomowych itp.

___

## „Hello World”

```cpp
#include <iostream>
#include <thread>

void hello()
{
    std::cout << "Hello, world\n";
}

int main()
{
    std::thread t(hello);
    t.join();
}
```

___

## Zarządzanie wątkami

* std::thread,
* std::thread::join()
* std::thread::detach()
* std::thread::joinable()

```cpp
#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    std::thread t;
    std::cout << "before starting, joinable: " << std::boolalpha << t.joinable() << '\n';

    t = std::thread(foo);
    std::cout << "after starting, joinable: " << std::boolalpha << t.joinable() << '\n';

    t.join();
    std::cout << "after joining, joinable: " << std::boolalpha << t.joinable() << '\n';
}

```output
before starting, joinable: false
after starting, joinable: true
after joining, joinable: false
```

___

## Co przekazać do std::thread?

* Funkcje
* Funktor (obiekt funkcyjny) – obiekt, który możemy wywołać tak jak funkcję (np. lambda)
* Wskaźnik na funkcje lub wskaźnik do funkcji składowej
* Obiekt funkcyjny jest <strong>kopiowany</strong> do obszaru pamięci należącej do nowo utworzonego wątku

___

## Co przekazać do std::thread?

```cpp
struct Bar {
    void operator()()
    {
        std::cout << "Hello world";
    }
}

void foo()
{
    std::cout << "Hello world";
}

int main()
{
    std::thread t1([]() {
        "Hello world"
    });

    std::thread t2(foo);

    Bar bar;
    std::thread t3(bar);
}
```

```cpp
void foo()
{
    std::cout << "Hello world\n";
}

class Bar {
public:
    void foo()
    {
        std::cout << "Hello world\n";
    };
};

int main()
{
    std::thread t(*foo);
    t.join();

    class Bar bar;
    std::thread t1(&Bar::foo, bar);
    t1.join();

    return 0;
}
```

___

## std::thread::detach()

* Zadanie 1:
  * Jakie zagrożenia kryje kod poniżej?
  * Jak go poprawić?

```cpp
#include <iostream>
#include <thread>

void do_something(int data)
{
    std::cout << "Data: " << data << "\n" << std::flush;
}

struct Foo {
public:
    Foo(int& i): i_(i) {}
    void operator()()
    {
        for (int k = 0; k < 100000 ; ++k) {
            do_something(++i_);
        }
    }

private:
    int& _i;
};

void oops()
{
    int some_local_state = 0;
    Foo foo(some_local_state);
    std::thread t(foo);
    std::cout << "Detach thread\n" << std::flush;
    t.detach();
}

int main()
{
    std::thread t(oops);
    t.join();

    return 0;
}
```

___

## `std::terminate` i `std::thread::~thread`

* If <code>*this</code> has an associated thread (<code>joinable() == true</code>), <a href=https://en.cppreference.com/w/cpp/error/terminate>std::terminate()</a> is called.
* <strong>ZAWSZE</strong> należy zawołać std::join() lub std::detach() na wątku!
* Co z wyjątkami, jakieś pomysły?

___

## `std::exception`

* Zadanie 2:
  * Jakie zagrożenia kryje kod poniżej?
  * Jak lepiej można rozwiązać poniższy problem?

```cpp
void do_something(int data)
{
    std::cout << "Data: " << data << "\n" << std::flush;
}

struct Foo {
public:
    Foo(int& i): i_(i) {}
    void operator()()
    {
        for (int k = 0; k < 100000 ; ++k) {
            do_something(++i_);
        }
    }

private:
    int& _i;
};

void bar() {throw std::runtime_error("Error"); }

void oops()
{
    int some_local_state = 0;
    Foo foo(some_local_state);
    std::thread t(foo);
    std::cout << "Detach thread\n" << std::flush;
    t.detach();
}

int main()
{
    oops();
    return 0;
}
```
