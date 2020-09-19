<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Sekcja krytyczna

<div style="display: flex;">

<div>

```c++
class SafeList {
    mutex m;
public:
    void remove_safely(Node* node) {
        lock_guard<mutex> lock(m);
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    } // automatic unlocking
    // other methods
};
```
<!-- .element: style="width: 100%;" -->
</div>

<div>

* <!-- .element: class="fragment fade-in" --> Sekcja krytyczna to fragment programu, który może być wykonywany tylko przez 1 wątek na raz
* <!-- .element: class="fragment fade-in" --> Zazwyczaj jest realizowana z użyciem mutexu jako blokady dostępu
* <!-- .element: class="fragment fade-in" --> Zawsze używaj menadżera blokady (np. <code>lock_guard<mutex></code>) w celu zapewnienia mechanizmu RAII
* <!-- .element: class="fragment fade-in" --> Całą sekcję krytyczną zazwyczaj wydziela się do osobnej funkcji

</div><!-- .element: style="background-color: #8B3536;" class="fragment fade-in" -->

</div>
