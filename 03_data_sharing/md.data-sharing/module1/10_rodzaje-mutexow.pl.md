<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

### Rodzaje mutexów

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .88em" -->
### Rodzaje mutexów

* <!-- .element: class="fragment fade-in" --> <code>mutex</code>
    * <!-- .element: class="fragment fade-in" --> <code>void_lock()</code> - bieżący wątek jest wstrzymany, aż do momentu pozyskania blokady
    * <!-- .element: class="fragment fade-in" --> <code>void_unlock()</code> - jeżeli bieżący wątek jest posiadaczem blokady, to następuje jej zwolnienie
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock()</code> - próba pozyskania blokady bez wstrzymywania bieżącego wątku. Zwraca true jeśli blokada została pozyskana, w przeciwnym wypadku zwraca false
* <!-- .element: class="fragment fade-in" --> <code>timed_mutex</code>
    * <!-- .element: class="fragment fade-in" --> posiada metody umożliwiające zdefiniowanie maksymalnego czasu oczekiwania na pozyskanie blokady przez wątek
        * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_until(timeout_time)</code>
        * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_for(timeout_duration)</code>
* <!-- .element: class="fragment fade-in" --> <code>recursive_mutex</code>
    * <!-- .element: class="fragment fade-in" --> Ten sam wątek może wielokrotnie pozyskać mutex poprzez wywołanie metody <code>lock()</code> lub <code>try_lock()</code>
    * <!-- .element: class="fragment fade-in" --> Aby zwolnić mutex wątek musi odpowiednią ilość razy wywołać <code>unlock()</code>
* <!-- .element: class="fragment fade-in" --> <code>recursive_timed_mutex</code>
    * <!-- .element: class="fragment fade-in" --> posiada własności <code>timed_mutex</code>
    * <!-- .element: class="fragment fade-in" --> posiada własności <code>recursive_mutex</code>
* <!-- .element: class="fragment fade-in" --> <code>shared_mutex</code>
    * <!-- .element: class="fragment fade-in" --> możliwość pozyskiwania blokad współdzielonych przy pomocy metod:
        * <!-- .element: class="fragment fade-in" --> <code>void lock_shared()</code>
        * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared()</code>
        * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared_for(rel_time)</code>
        * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared_until(abs_time)</code>
        * <!-- .element: class="fragment fade-in" --> <code>void unlock_shared()</code>
