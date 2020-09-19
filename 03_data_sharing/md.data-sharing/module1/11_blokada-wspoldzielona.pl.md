<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Blokada współdzielona - `shared_mutex`

<div style="display: flex;">

<div>

```c++
#include <deque>
#include <shared_mutex>
std::deque<int> ids;
std::shared_mutex mtxIds;
int getIdsIndex() { /* ... */ }
void process(int) { /* ... */ }
int newValue() { /* ... */ }
void reader() {
    int index = getIdsIndex();
    std::shared_lock<std::shared_mutex> lock(mtxIds);
    int value = ids[index];
    lock.unlock();
    process(value);
}
void writer() {
    int index = getIdsIndex();
    std::lock_guard<std::shared_mutex> lock(mtxIds);
    ids[index] = newValue();
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div style="background-color: #8B3536;">

* <!-- .element: class="fragment fade-in" --> Współdzielone blokady używane są w trakcie czytania danych
    * <!-- .element: class="fragment fade-in" --> <code>shared_lock<shared_mutex></code>
* <!-- .element: class="fragment fade-in" --> Do zapisu danych trzeba pozyskać wyłączną blokadę
    * <!-- .element: class="fragment fade-in" --> <code>lock_guard<shared_mutex></code>
    * <!-- .element: class="fragment fade-in" --> <code>unique_lock<shared_mutex></code>
</div><!-- .element: class="fragment fade-in" --> 

</div>
