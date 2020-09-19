<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Współdzielenie danych z ich modyfikacją

* <!-- .element: class="fragment fade-in" --> Wyobraź sobie, że współdzielisz mieszkanie ze współlokatorami. Każde z was ma własny pokój, ale toaleta jest wspólna. Gdy ktoś w niej jest, musi zablokować drzwi. Jeśli chcecie skorzystać z toalety w tym samym czasie nie możecie tego zrobić. Długie oczekiwanie, aż inna osoba przestanie okupować toaletę jest frustrujące. Toaleta po skorzystaniu jest w innym stanie niż przed (mniej papieru, zużyta woda, ciekawsze zapachy…)
* <!-- .element: class="fragment fade-in" --> Dokładnie tak samo jest ze współdzieleniem danych :)
    * <!-- .element: class="fragment fade-in" --> Wątki = Ty i współlokator
    * <!-- .element: class="fragment fade-in" --> Zasób (dane) = toaleta (papier, woda)
    * <!-- .element: class="fragment fade-in" --> Mutex = blokada drzwi
* <!-- .element: class="fragment fade-in" --> Modyfikacja danych (użycie toalety) = problem - wymagana jest synchronizacja
* <!-- .element: class="fragment fade-in" --> Co gdyby nie było drzwi z blokadą do toalety? Ktoś mógłby nam spuścić wodę w trakcie jej użytkowania albo podebrać ostatni listek papieru toaletowego.