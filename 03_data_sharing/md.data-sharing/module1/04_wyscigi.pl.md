<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Czy są tu wyścigi?

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

### Wyścigi (race conditions)

* <!-- .element: class="fragment fade-in" --> Wyścigi generują niedeterministyczne (losowe) zachowania programu.
* <!-- .element: class="fragment fade-in" --> Wyścig = Niezdefiniowane zachowanie.
* <!-- .element: class="fragment fade-in" --> Niechciane skutki uboczne zazwyczaj nie występują podczas większości uruchomień danej procedury.
* <!-- .element: class="fragment fade-in" --> Problemy mogą występować rzadziej niż 1 na 1000 uruchomień.
* <!-- .element: class="fragment fade-in" --> Wyścigi niesamowicie trudno wykryć. Zazwyczaj jeśli procesor nie jest obciążony to wszystko działa jak należy. Im bardziej obciążony procesor, tym bardziej rośnie ryzyko innej kolejności dostępu do danych. Takie problemy trudno jest zreprodukować.
* <!-- .element: class="fragment fade-in" --> Problem wyścigów jest krytyczny czasowo i może być zupełnie niemożliwy do wychwycenia pod debuggerem. Debugger wpływa na czasy wykonywania poszczególnych instrukcji.
* <!-- .element: class="fragment fade-in" --> Znacznie łatwiej zapobiegać wyścigom, bo leczenie jest długotrwałe i kosztowne.
* <!-- .element: class="fragment fade-in" --> Thread Sanitizer (TSan) - data race detector.