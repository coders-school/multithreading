<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie 1: reprodukcja wyścigów pod debuggerem

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

</div><!-- .element: style="font-size: .9em;" -->

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

</div><!-- .element: class="fragment fade-in" style="font-size: .9em; background-color: #8B3536;" -->

</div>
