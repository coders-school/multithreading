<!-- .slide: data-background="#111111" -->

# Mutex - dobre praktyki

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Jak blokować?

Zawsze używaj mutexu z odpowiednim menadżerem (wrapperem RAII):

* <!-- .element: class="fragment fade-in" --> <code>lock_guard</code> - najprostszy, główny wybór
* <!-- .element: class="fragment fade-in" --> <code>unique_lock</code> - opóźnione blokowanie, próby zablokowania ograniczone czasowo, ...
* <!-- .element: class="fragment fade-in" --> <code>shared_lock</code> - współdzielona blokada do odczytu zmiennych
* <!-- .element: class="fragment fade-in" --> Jeśli wątki potrzebują zablokować kilka mutexów, to powinny być one zawsze blokowane w tej samej kolejności w każdym używających ich wątku. Zapobiega to zakleszczeniom (deadlock)
* <!-- .element: class="fragment fade-in" --> <code>scoped_lock</code> - blokuje kilka mutexów zapobiegając zakleszczeniom (deadlock)
* <!-- .element: class="fragment fade-in" --> Każdy wrapper RAII musi być nazwany!

___

## Kiedy blokować?

* <!-- .element: class="fragment fade-in" --> Blokuj zawsze gdy to konieczne
* <!-- .element: class="fragment fade-in" --> Unikaj blokowania gdzie tylko się da
* <!-- .element: class="fragment fade-in" --> Czas trwania blokady powinien być jak najkrótszy
  * <!-- .element: class="fragment fade-in" --> Jeśli danych do modyfikacji jest dużo i nie są one ułożone obok siebie w pamięci, to czas trwania blokady mutexu będzie bardzo długi, aby wszystkie dane zdążyły się pobrać do pamięci cache.
  * <!-- .element: class="fragment fade-in" --> Operacje I/O (wejścia/wyjścia) takie jak odczyty i zapisy plików, pobieranie danych przez sieć to długo lub ogólnie wywołanie funkcji systemowej to długo trwające procesy

___

## Gdzie blokować?

* <!-- .element: class="fragment fade-in" --> Mutex powinien istnieć razem z obiektem, do którego blokowania jest używany
* <!-- .element: class="fragment fade-in" --> Jeśli mutex blokuje dostęp do pola klasy, to powinien on istnieć w tej klasie
* <!-- .element: class="fragment fade-in" --> M&M rule - mutable & mutex go together (in classes)
* <!-- .element: class="fragment fade-in" --> Każdy zasób powinien mieć swój własny mutex, bo to upraszcza logikę blokowania (używamy <code>scoped_lock</code>)
* <!-- .element: class="fragment fade-in" --> W naprawdę rzadkich przypadkach jeden mutex może blokować kilka obiektów, ale należy naprawdę bardzo dokładnie przemyśleć logikę i wszystkie zależności czasowe
