<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### `std::thread` is a very "low-level" mechanism

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### `std::thread` is a very "low-level" mechanism

* <!-- .element: class="fragment fade-in" --> How to <span style="color:#AD5758">return</span> something from it?
* <!-- .element: class="fragment fade-in" --> How to forward an <span style="color:#AD5758">exception</span>?
* <!-- .element: class="fragment fade-in" --> Should be manually <span style="color:#AD5758">joined</span>/<span style="color:#AD5758">detached</span>
