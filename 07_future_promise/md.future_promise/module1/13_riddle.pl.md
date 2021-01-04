<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Zagadka

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zagadka

<div style="display: flex;">

<div style="width: 50%;">

```c++
#include <iostream>
#include <string>
#include <future>

int main() {
  std::string x = "x";
  std::async(std::launch::async, [&x](){
    x = "y";
  });
  std::async(std::launch::async, [&x](){
    x = "z";
  });
  std::cout << x;
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> Co wyświetli się na ekranie?
  * <!-- .element: class="fragment fade-in" --> x
  * <!-- .element: class="fragment fade-in" --> y
  * <!-- .element: class="fragment fade-in" --> z
  * <!-- .element: class="fragment fade-in" --> To zależy (od czego?)

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Odpowiedź

<div style="display: flex;">

<div style="width: 50%;">

```c++
#include <iostream>
#include <string>
#include <future>

int main() {
  std::string x = "x";
  std::async(std::launch::async, [&x](){
    x = "y";
  });
  std::async(std::launch::async, [&x](){
    x = "z";
  });
  std::cout << x;
}
```
<!-- .element: class="fragment fade-in" -->
```bash
$> ./06_riddle
z
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; background-color: #8B3536; padding: 5px 10px; font-size: .86em;">

* <!-- .element: class="fragment fade-in" --> Co wyświetli się na ekranie?
  * <!-- .element: class="fragment fade-in" --> x
  * <!-- .element: class="fragment fade-in" --> y
  * <!-- .element: class="fragment fade-in" --> z
  * <!-- .element: class="fragment fade-in" --> To zależy (od czego?)
* <!-- .element: class="fragment fade-in" --> Odpowiedź:
  * <!-- .element: class="fragment fade-in" --> z
* <!-- .element: class="fragment fade-in" --> Wyjaśnienie:
  * <!-- .element: class="fragment fade-in" --> jeśli obiekt future jest tymczasowy to w destruktorze czeka dopóki zadanie się nie skończy. Drugie zadanie zostanie więc uruchomione po pierwszym i pomimo, że będą one w innych wątkach to ich wykonanie będzie zsynchronizowane
  * <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/future/-future">cppreference -future</a>
  * <!-- .element: class="fragment fade-in" --> <a href="http://cppquiz.org/quiz/question/48">Źródło</a>
  * <!-- .element: class="fragment fade-in" --> std::futures from std::async aren't special! - Scott Meyers
* <!-- .element: class="fragment fade-in" --> Wnioski:
  * <!-- .element: class="fragment fade-in" --> Jeśli chcesz mieć wywołania asynchroniczne to wynik działania musisz zapisać w zmiennej <code>std::future</code>

</div> <!-- .element: class="fragment fade-in" -->

<div>