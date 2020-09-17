<!-- .slide: data-background="#111111" -->

# Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Przykład: usuwanie węzła z listy dwukierunkowej

1. <!-- .element: class="fragment fade-in" --> Znajdź węzeł N do usunięcia
2. <!-- .element: class="fragment fade-in" --> Ustaw wskaźnik <code>NEXT</code> w węźle N-1 na N+1
3. <!-- .element: class="fragment fade-in" --> Ustaw wskaźnik <code>PREV</code> w węźle N+1 na N-1
4. <!-- .element: class="fragment fade-in" --> Usuń węzeł N