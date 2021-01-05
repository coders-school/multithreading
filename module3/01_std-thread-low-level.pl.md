<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### `std::thread` jest mechanizmem bardzo "niskopoziomowym"

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### `std::thread` jest mechanizmem bardzo "niskopoziomowym"

* <!-- .element: class="fragment fade-in" --> W jaki sposób <span style="color:#cf802a">zwrócić</span> z niego cokolwiek?
* <!-- .element: class="fragment fade-in" --> Jak przekazać <span style="color:#cf802a">wyjątek</span>?
* <!-- .element: class="fragment fade-in" --> Powinien być manualnie <span style="color:#cf802a">łączony</span>/<span style="color:#cf802a">rozłączany</span>
