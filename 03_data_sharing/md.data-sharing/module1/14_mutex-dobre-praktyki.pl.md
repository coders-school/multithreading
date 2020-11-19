<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

### Mutex - dobre praktyki

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Mutex - dobre praktyki

* <!-- .element: class="fragment fade-in" --> Zawsze używaj mutexu z odpowiednim menadżerem (wrapperem RAII):
    * <!-- .element: class="fragment fade-in" --> <code>lock_guard</code> - najprostszy, główny wybór
    * <!-- .element: class="fragment fade-in" --> <code>unique_lock</code> - opóźnione blokowanie, próby zablokowania ograniczone czasowo, ...
    * <!-- .element: class="fragment fade-in" --> <code>scoped_lock</code> - blokuje kilka mutexów, zapobiega zakleszczeniom (deadlock)
    * <!-- .element: class="fragment fade-in" --> <code>shared_lock</code> - współdzielona blokada do odczytu zmiennych
* <!-- .element: class="fragment fade-in" --> Blokuj zawsze gdy to konieczne
* <!-- .element: class="fragment fade-in" --> Unikaj blokowania gdzie tylko się da
* <!-- .element: class="fragment fade-in" --> Czas trwania blokady powinien być jak najkrótszy
    * <!-- .element: class="fragment fade-in" --> Jeśli danych do modyfikacji jest dużo i nie są one ułożone obok siebie w pamięci, to czas trwania blokady mutexu będzie bardzo długi, aby wszystkie dane zdążyły się pobrać do pamięci cache.
    * <!-- .element: class="fragment fade-in" --> Operacje I/O (wejścia/wyjścia) takie jak odczyty i zapisy plików, pobieranie danych przez kartę sieciową to długo trwające procesy
* <!-- .element: class="fragment fade-in" --> Jeśli wątki potrzebują zablokować kilka mutexów, to powinny być one zawsze blokowane w tej samej kolejności w każdym używających ich wątku. Zapobiega to zakleszczeniom (deadlock)