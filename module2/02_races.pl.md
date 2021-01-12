<!-- .slide: data-background="#111111" -->

# Wyścigi (race conditions)

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Czy są tu wyścigi?

<div style="display:flex;">

<div style="width:60%;">

```c++
#include <thread>
#include <iostream>
#include <functional>

void abc(int &a) { a = 2; }
void def(int &a) { a = 3; }

int main() {
    int x = 1;
    std::thread t1(abc, std::ref(x));
    std::thread t2(def, std::ref(x));

    t1.join();
    t2.join();

    std::cout << x << std::endl;
}
```

</div>

<div>

Co wyświetli się na ekranie?<!-- .element: class="fragment fade-in" -->

```bash
$> g++ 01_threads_write.cpp -lpthread
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
2
```
<!-- .element: class="fragment fade-in" style="transform: scale(.90); width: 106%; margin:-35px 0 0 -30px;" -->

</div>

</div>
<!-- Wartości w stylowaniu zostały nadane w celu ładniejszego wyświetlania w Reveal.js. Na podglądzie zwykłego md również wygląda to dobrze. Przynajmniej u mnie. -->

___

## Wyścigi (race conditions)

* <!-- .element: class="fragment fade-in" --> Wyścigi generują niedeterministyczne (losowe) zachowania programu.
* <!-- .element: class="fragment fade-in" --> Wyścig = Niezdefiniowane zachowanie.
* <!-- .element: class="fragment fade-in" --> Niechciane skutki uboczne zazwyczaj nie występują podczas większości uruchomień danej procedury.
* <!-- .element: class="fragment fade-in" --> Problemy mogą występować rzadziej niż 1 na 1000 uruchomień.
* <!-- .element: class="fragment fade-in" --> Wyścigi niesamowicie trudno wykryć. Zazwyczaj jeśli procesor nie jest obciążony to wszystko działa jak należy. Im bardziej obciążony procesor, tym bardziej rośnie ryzyko innej kolejności dostępu do danych. Takie problemy trudno jest zreprodukować.
* <!-- .element: class="fragment fade-in" --> Problem wyścigów jest krytyczny czasowo i może być zupełnie niemożliwy do wychwycenia pod debuggerem. Debugger wpływa na czasy wykonywania poszczególnych instrukcji.
* <!-- .element: class="fragment fade-in" --> Znacznie łatwiej zapobiegać wyścigom, bo leczenie jest długotrwałe i kosztowne.
* <!-- .element: class="fragment fade-in" --> Thread Sanitizer (TSan) - data race detector.

___
<!-- .slide: data-background="img/tsan.png" data-background-position="left top" -->
## Thread sanitizer

```bash
$> g++ 01_threads_write.cpp -lpthread -fsanitize=thread -O2 -g
$> ./a.out
```

___

### Zadanie: reprodukcja wyścigów pod debuggerem

<div style="display: flex;">

<div>

* <!-- .element: class="fragment fade-in" --> Skompiluj program threads_write.cpp</br><code>g++ 01_threads_write.cpp -lpthread -g</code>
* <!-- .element: class="fragment fade-in" --> Uruchom program pod debuggerem (gdb lub inny)</br><code>gdb --tui ./a.out</code>
* <!-- .element: class="fragment fade-in" --> Spraw, aby na końcu programu w zmiennej x była wartość 2
* <!-- .element: class="fragment fade-in" --> Spraw, aby na końcu programu w zmiennej x była wartość 3
* <!-- .element: class="fragment fade-in" --> Przydatne komendy:
  * <!-- .element: class="fragment fade-in" --> <code>b 5</code> - ustawia breakpoint w piątej linii
  * <!-- .element: class="fragment fade-in" --> <code>watch x</code> - obserwowanie zmian zmiennej x (debugger zatrzyma się gdy nastąpi jej modyfikacja)
  * <!-- .element: class="fragment fade-in" --> <code>c</code> - kontynuowanie debugowania
  * <!-- .element: class="fragment fade-in" --> <code>info threads</code> - informacje o wątkach
  * <!-- .element: class="fragment fade-in" --> <code>thread 3</code> - przełączenie na wątek 3
  * <!-- .element: class="fragment fade-in" --> <code>n</code> - następna instrukcja
  * <!-- .element: class="fragment fade-in" --> <code>fin</code> - wykonanie wszystkiego do końca bieżącej funkcji
  * <!-- .element: class="fragment fade-in" --> <code>up</code> - przejście do wyższej ramki stosu
  * <!-- .element: class="fragment fade-in" --> <code>down</code> - przejście do niższej ramki stosu
  * <!-- .element: class="fragment fade-in" --> <code>del br</code> - usunięcie wszystkich breakpointów
  * <!-- .element: class="fragment fade-in" --> <code>CTRL + L</code> - odświeżenie widoku

</div><!-- .element: style="font-size: .9em; margin: 0 10px" -->

<div>

Jedna z możliwych sekwencji:

* <!-- .element: class="fragment fade-in" --> <code>b 5</code>
* <!-- .element: class="fragment fade-in" --> <code>b 17</code>
* <!-- .element: class="fragment fade-in" --> <code>r</code>
* <!-- .element: class="fragment fade-in" --> <code>c</code>
* <!-- .element: class="fragment fade-in" --> <code>p x</code>
  * <!-- .element: class="fragment fade-in" --> <code>$ = 2</code>
* <!-- .element: class="fragment fade-in" --> <code>del br</code>
* <!-- .element: class="fragment fade-in" --> <code>b 6</code>
* <!-- .element: class="fragment fade-in" --> <code>b 17</code>
* <!-- .element: class="fragment fade-in" --> <code>r</code>
* <!-- .element: class="fragment fade-in" --> <code>c</code>
* <!-- .element: class="fragment fade-in" --> <code>p x</code>
  * <!-- .element: class="fragment fade-in" --> <code>$ = 3</code>

</div><!-- .element: class="fragment fade-in" style="font-size: .9em; background-color: #252627; padding: 20px" -->

</div>
