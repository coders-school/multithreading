<!-- .slide: data-background="#111111" -->

# Multithreading

## Zadanie 1: reprodukcja wyścigów pod debuggerem

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie 1: reprodukcja wyścigów pod debuggerem

<div style="display: flex;">

<div>

* Skompiluj program threads_write.cpp</br>`g++ 01_threads_write.cpp -lpthread -g`
* Uruchom program pod debuggerem (gdb lub inny)</br>`gdb --tui ./a.out`
* Spraw, aby na końcu programu w zmiennej x była wartość 2
* Spraw, aby na końcu programu w zmiennej x była wartość 3
* Przydatne komendy:
    * <code>b 5</code> - ustawia breakpoint w piątej linii
    * <code>watch x</code> - obserwowanie zmian zmiennej x (debugger zatrzyma się gdy nastąpi jej modyfikacja)
    * <code>c</code> - kontynuowanie debugowania
    * <code>info threads</code> - informacje o wątkach
    * <code>thread 3</code> - przełączenie na wątek 3
    * <code>n</code> - następna instrukcja
    * <code>fin</code> - wykonanie wszystkiego do końca bieżącej funkcji
    * <code>up</code> - przejście do wyższej ramki stosu
    * <code>down</code> - przejście do niższej ramki stosu
    * <code>del br</code> - usunięcie wszystkich breakpointów
    * <code>CTRL + L</code> - odświeżenie widoku

</div><!-- .element: style="font-size: .9em;" -->

<div>

Jedna z możliwych sekwencji:
* <code>b 5</code>
* <code>b 17</code>
* <code>r</code>
* <code>c</code>
* <code>p x</code>
    * <code>$ = 2</code>
* <code>del br</code>
* <code>b 6</code>
* <code>b 17</code>
* <code>r</code>
* <code>c</code>
* <code>p x</code>
    * <code>$ = 3</code>

</div><!-- .element: style="font-size: .9em; background-color: #8B3536;" -->

</div>