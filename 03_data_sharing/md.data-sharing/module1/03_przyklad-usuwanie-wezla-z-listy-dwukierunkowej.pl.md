<!-- .slide: data-background="#111111" -->

# Multithreading

## Przykład: usuwanie węzła z listy dwukierunkowej

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Przykład: usuwanie węzła z listy dwukierunkowej

<div style="display:flex;">

<div>

1. <!-- .element: class="fragment fade-in" --> Znajdź węzeł N do usunięcia
2. <!-- .element: class="fragment fade-in" --> Ustaw wskaźnik <code>NEXT</code> w węźle N-1 na N+1
3. <!-- .element: class="fragment fade-in" --> Ustaw wskaźnik <code>PREV</code> w węźle N+1 na N-1
4. <!-- .element: class="fragment fade-in" --> Usuń węzeł N

</div>

<div style="width: 32%;">

>Pomiędzy krokami 2 i 3 wskaźniki są ustawione w nieprawidłowy, niejednolity sposób

</div><!-- .element: class="fragment fade-in" -->

</div>

___
<!-- .slide: data-transition="fade-out" -->
#### Znajdź węzeł N do usunięcia

<img src="img/1.png" style="padding-top:58px;padding-bottom:58px;" class="plain">

___
<!-- .slide: data-transition="fade" -->
#### Ustaw wskaźnik <code>NEXT</code> w węźle N-1 na N+1

<img src="img/2.png" style="padding-bottom:58px;" class="plain">

___
<!-- .slide: data-transition="fade" -->
#### Ustaw wskaźnik <code>PREV</code> w węźle N+1 na N-1

<img src="img/3.png">

___
<!-- .slide: data-transition="fade-in" -->
#### Usuń węzeł N

<img src="img/4.png">

___

### Przykład: usuwanie węzła z listy dwukierunkowej

* <!-- .element: class="fragment fade-in" --> Usuwanie węzła z listy składa się z kilku kroków, następuje modyfikacja kilku węzłów
* <!-- .element: class="fragment fade-in" --> Co jeśli inny wątek będzie iterował po liście gdy trwa usuwanie węzła N?
* <!-- .element: class="fragment fade-in" --> Co jeśli inny wątek zacznie usuwać węzeł N+1 w czasie gdy trwa usuwanie węzła N?
* <!-- .element: class="fragment fade-in" --> Zjawisko to nosi nazwę wyścigów (race conditions)
* <!-- .element: class="fragment fade-in" --> Wyścigi zazwyczaj występują, gdy trzeba zmodyfikować dwa lub więcej oddzielnych kawałków danych, tak jak wskaźniki na przykładzie z listą