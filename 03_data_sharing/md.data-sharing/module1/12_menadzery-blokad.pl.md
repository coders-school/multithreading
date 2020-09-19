<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Menadżery blokad

<div style="display: flex;">

<div>

* <!-- .element: class="fragment fade-in" --> <code>lock_guard<Mutex></code>
    * <!-- .element: class="fragment fade-in" --> najprostszy, główny wybór
    * <!-- .element: class="fragment fade-in" --> konstruktor wywołuje <code>lock()</code> na mutexie
    * <!-- .element: class="fragment fade-in" --> destruktor woła <code>unlock()</code>
    * <!-- .element: class="fragment fade-in" --> jest niekopiowalny
* <!-- .element: class="fragment fade-in" --> <code>unique_lock<Mutex></code>
    * <!-- .element: class="fragment fade-in" --> opóźnione blokowanie
    * <!-- .element: class="fragment fade-in" --> próby zablokowania ograniczone czasowo
    * <!-- .element: class="fragment fade-in" --> blokowanie rekursywne
    * <!-- .element: class="fragment fade-in" --> podejmowanie nieblokujących prób pozyskania blokady <code>(try_lock)</code>
    * <!-- .element: class="fragment fade-in" --> korzystanie z <code>timed_mutex</code>
    * <!-- .element: class="fragment fade-in" --> korzystanie ze zmiennych warunkowych (condition variable)
    * <!-- .element: class="fragment fade-in" --> jest niekopiowalny
    * <!-- .element: class="fragment fade-in" --> jest przenaszalny (move)

</div>

<div>

* <!-- .element: class="fragment fade-in" --> <code>scoped_lock<Mutexes…></code>
    * <!-- .element: class="fragment fade-in" --> blokuje kilka mutexów
    * <!-- .element: class="fragment fade-in" --> zapobiega zakleszczeniom (deadlock)
    * <!-- .element: class="fragment fade-in" --> konstruktor blokuje wszystkie mutexy w bezpiecznej kolejności, unikając blokad
    * <!-- .element: class="fragment fade-in" --> destruktor odblokowuje je w kolejności odwrotnej
    * <!-- .element: class="fragment fade-in" --> jest niekopiowalny
* <!-- .element: class="fragment fade-in" --> <code>shared_lock<SharedMutex></code>
    * <!-- .element: class="fragment fade-in" --> menadżer współdzielonych blokad do odczytu zmiennych
    * <!-- .element: class="fragment fade-in" --> kilka wątków może współdzielić blokadę <code>SharedMutex</code>
    * <!-- .element: class="fragment fade-in" --> inny wątek może pozyskać blokadę <code>SharedMutex</code> na wyłączność za pomocą menadżera <code>unique_lock</code>
    * <!-- .element: class="fragment fade-in" --> te same własności co <code>unique_lock</code>

</div>
