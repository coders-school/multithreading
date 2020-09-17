<!-- .slide: data-background="#111111" -->

# Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Współdzielenie danych do odczytu

* <!-- .element: class="fragment fade-in" --> Wyobraź sobie, że oglądasz TV ze znajomymi. Wielu znajomych może oglądać TV razem z Tobą. Żadne z was nie wpływa w żaden sposób na oglądany materiał (nikt nie ma pilota :)).
* <!-- .element: class="fragment fade-in" --> Dokładnie tak samo jest ze współdzieleniem danych tylko do odczytu
    * <!-- .element: class="fragment fade-in" --> Wątki = Ty i znajomi
    * <!-- .element: class="fragment fade-in" --> Dane = film
    * <!-- .element: class="fragment fade-in" --> Odczyt danych przez jeden wątek nie zakłóca odczytu danych przez inny wątek
* <!-- .element: class="fragment fade-in" --> Brak modyfikacji = brak problemów
