<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### `std::async`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### `std::async`

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
void promise_future_approach() {
  std::promise<int> promise;
  std::future<int> future = promise.get_future();
  auto function = [] (std::promise<int> promise) {
    // ...
    promise.set_value(10);
  };
  std::thread t(function, std::move(promise));
  std::cout << future.get() << std::endl;
  t.join();
}

void async_approach () {
  auto function = [] () {
    // ...
    return 20;
  };
  std::future<int> future = std::async(function);
  std::cout << future.get() << std::endl;
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> <code>#include <future></code>
* <!-- .element: class="fragment fade-in" --> <code>std::async()</code>
* <!-- .element: class="fragment fade-in" --> Opakowuje funkcję, która może zostać wywołana asynchronicznie
* <!-- .element: class="fragment fade-in" --> Zwraca odpowiednie <code>std::future<T></code>
* <!-- .element: class="fragment fade-in" --> Obsługuje wyjątki poprzez <code>std::promise</code>/<code>std::future</code>
* <!-- .element: class="fragment fade-in" --> Automatycznie tworzy wyjątki jeśli potrzeba
* <!-- .element: class="fragment fade-in" --> Można wybrać rodzaj zachowania poprzez parametr <code>policy</code> (<code>async</code>, <code>deferred</code>)

```bash
$> ./02_async
10
20
```
<!-- .element: class="fragment fade-in" -->
</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### `std::async`

* <!-- .element: class="fragment fade-in" --> <code>std::async</code> to wysoko-poziomowe rozwiązanie (w końcu!), które automatycznie zarządza wywołaniami asynchronicznymi z podstawowymi mechanizmami synchronizacji
* <!-- .element: class="fragment fade-in" --> Najwygodniejsza forma odpalania zadań:
  * <!-- .element: class="fragment fade-in" --> obsługa wartości zwracanych
  * <!-- .element: class="fragment fade-in" --> obsługa wyjątków
  * <!-- .element: class="fragment fade-in" --> synchronizacja (blokujące <code>get()</code> i <code>wait()</code> na <code>std::future</code>)
  * <!-- .element: class="fragment fade-in" --> scheduler - automatyczne kolejkowanie zadań realizowane poprzez implementację biblioteki standardowej
  * <!-- .element: class="fragment fade-in" --> możliwość manualnego wybrania rodzaju odpalenia (natychmiastowe, asynchroniczne <code>async</code>, synchroniczne <code>deferred</code>)