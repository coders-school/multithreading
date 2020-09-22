<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Mutex - unikanie wyścigów

* <!-- .element: class="fragment fade-in" --> Mutex (Mutual Exclusion) - wzajemne wykluczanie
* <!-- .element: class="fragment fade-in" --> Implementacja blokady
* <!-- .element: class="fragment fade-in" --> <code>#include &lt;mutex&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::mutex</code>
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
    * <!-- .element: class="fragment fade-in" --> <code>void lock()</code> - zablokowanie mutexu. Operacja blokująca. Jeśli mutex jest zablokowany przez inny wątek to oczekujemy na odblokowanie.
    * <!-- .element: class="fragment fade-in" --> <code>void unlock()</code> - Odblokowanie mutexu.
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock()</code> - zablokowanie mutexu. Operacja nieblokująca. Jeśli mutex jest już zablokowany przez inny wątek to kontynuuje dalsze wykonanie wątku. Zwraca true, jeśli udało się zablokować mutex
