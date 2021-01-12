<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Polityki uruchamiania (launch policies)

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .96em" -->

### Polityki uruchamiania (launch policies)

#### `async(std::launch policy, Function&& f, Args&&... args);` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::launch::async</code> - wywołanie asynchroniczne w osobnym wątku systemowym
* <!-- .element: class="fragment fade-in" --> <code>std::launch::deferred</code> - leniwie wykonuje funkcję <code>f</code> w momencie pierwszego wywołania na obiekcie <code>future</code> metod <code>get()</code> lub <code>wait()</code>. Wykonanie jest synchroniczne, czyli wywołujący czeka na zakończenie funkcji <code>f</code>. Jeśli <code>get()</code> lub <code>wait()</code> nie zostaną zawołane funkcja <code>f</code> nie wykona się

#### `async(Function&& f, Args&&... args);` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Brak polityki - zachowuje się tak samo jak <code>async(std::launch::async | std::launch::deferred, f, args...)</code>. Implikacje:
  * <!-- .element: class="fragment fade-in" --> nie wiadomo, czy <code>f</code> zostanie wykonane współbieżnie
  * <!-- .element: class="fragment fade-in" --> nie wiadomo czy <code>f</code> wykona się w innym, czy w tym samym wątku, który wywołuje <code>get()</code> lub <code>wait()</code> na <code>future</code>
  * <!-- .element: class="fragment fade-in" --> nie można przewidzieć czy <code>f</code> w ogóle się wykona, bo mogą istnieć ścieżki w kodzie, gdzie <code>get()</code> lub <code>wait()</code> nie zostaną zawołane (np. z powodu wyjątków)