<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

### Podsumowanie

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Podsumowanie

<div>

* <!-- .element: class="fragment fade-in" --> Współdzielenie danych tylko do odczytu (read-only) jest bezpieczne i nie wymaga stosowania blokad. Słówko const oznacza bezpieczeństwo danych (read-only).
* <!-- .element: class="fragment fade-in" --> Gdy jakiś wątek zablokuje mutex, to każdy inny wątek chcący go zablokować musi poczekać na odblokowanie. Blokowanie zbyt dużych sekcji krytycznych jest błędem.
* <!-- .element: class="fragment fade-in" --> Problem zakleszczenia (deadlock) występuje, kiedy 2 mutexy (lub więcej) są blokowane przez wątki w różnych kolejnościach albo mamy 1 mutex, który nie zostanie odblokowany np. z powodu wyjątku i program nie przejdzie do funkcji unlock().
* <!-- .element: class="fragment fade-in" --> Mutexy znacznie spowalniają wykonywanie programu, ale zabezpieczają dane, jeśli są użyte w poprawny sposób.
* <!-- .element: class="fragment fade-in" --> W małych programach mutexy można trzymać jako obiekty globalne, ale w większych powinny być trzymane w odpowiedniej klasie, której operacje mają być blokowane. Jeśli taka klasa ma kilka zasobów, które mogą być modyfikowane niezależnie, to należy mieć kilka mutexów (1 na każdy zasób). Trzeba uważać na zakleszczenie.
* <!-- .element: class="fragment fade-in" --> Pomimo zablokowanych mutexów, jeśli używamy gdzieś wskaźników lub referencji do obiektów, które modyfikujemy w sekcji krytycznej, to cała ochrona jest na nic. Ochrona danych mutexami wymaga więc uważnego modelowania interfejsów.
* <!-- .element: class="fragment fade-in" --> Zawsze używaj muteksów poprzez menadżery blokad (wrappery RAII)
* <!-- .element: class="fragment fade-in" --> Używaj <code>std::atomic<T></code> (zmiennych atomowych). Dają one wygodny sposób lekkiej synchronizacji danych bez użycia mutexów, ale o tym w kolejnej części

</div>
<!-- .element: style="font-size: .81em" -->
___

### Praca domowa

* <!-- .element: class="fragment fade-in" --> Zaimplementuj problem ucztujących filozofów z użyciem wątków i mutexów.
  * <!-- .element: class="fragment fade-in" --> Każdy filozof ma być kontrolowany przez oddzielny wątek.
  * <!-- .element: class="fragment fade-in" --> Każdy sztuciec ma być chroniony przez 1 mutex
  * <!-- .element: class="fragment fade-in" --> Postaraj się o wizualizację problemu
  * <!-- .element: class="fragment fade-in" --> Strzeż się zakleszczeń
  * <!-- .element: class="fragment fade-in" --> Pobaw się liczbą filozofów i zobacz czy zmienia się zachowanie programu
  * <!-- .element: class="fragment fade-in" --> Zadanie dodatkowe: Strzeż się zagłodzenia (starvation). Jest to sytuacja w której przynajmniej 1 wątek z powodu implementacji lub swojego niższego priorytetu nigdy nie dostanie wszystkich wymaganych zasobów. Doimplementuj w tym celu pewien mechanizm, który zapobiegnie zagłodzeniu.

___

### Przydatne linki

* <!-- .element: class="fragment fade-in" --> <a href="https://people.freebsd.org/~lstewart/articles/cpumemory.pdf">What every programmer should know about memory</a> – Ulrich Drepper
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">The C++ memory model</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.wikipedia.org/wiki/Dining_philosophers_problem">Problem ucztujących filozofów</a>
