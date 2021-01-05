<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Więcej o `std::promise`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Więcej o `std::promise`

```c++
std::thread t(function, std::move(promise));
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> mogą być jedynie przenoszone

```c++
std::future<int> future = promise.get_future();
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> zwraca <code>future</code> związaną z wynikiem <code>promised</code>
* <!-- .element: class="fragment fade-in" --> Ponowne zawołanie rzuci wyjątkiem

<!-- Inna myśl jaką miałem na to zdanie to przetłumaczenie future i promised i zrobienie:
    'zwraca "przeszłość" powiązaną z "obiecanym" wynikiem' 
-->
___

### Jak "ustawić" wartość?

<div style="display:flex;">

<div style="width: 35%;">

* <!-- .element: class="fragment fade-in" --> ustawienie wartości

```c++
promise.set_value(10);
```
 <!-- .element: class="fragment fade-in" -->
</div>
<div style="width: 65%;">

* <!-- .element: class="fragment fade-in" --> <code>std::promise</code>/<code>std::future</code> są używane by utworzyć <span style="color:#AD5758;">jednokierunkowy kanał komunikacji</span>

```c++
promise.set_exception(std::make_exception_ptr(e));
```
<!-- .element: class="fragment fade-in" -->
```c++
try {
    // ...
} catch {
    promise.set_ecxeption(std::current_exception());
}
```
<!-- .element: class="fragment fade-in" -->
</div>

</div>

<div style="background-color: #8B3536; padding: 3px 22px;">

<code>std::promise</code> można "ustawić" tylko raz

</div> <!-- .element: class="fragment fade-in" -->

___

### Więcej o `std::future`

##### `future.valid();` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> sprawdza czy <code>future</code> może zostać <span style="color:#AD5758;">użyte</span>

##### `future.wait;` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <span style="color:#AD5758;">oczekuje</span> aż wynik będzie dostępny

<div style="background-color: #8B3536; padding: 3px 22px;">

nie używaj "niepoprawnej" `future`

</div> <!-- .element: class="fragment fade-in" -->

___

### jak pozyskać wartość?

##### `future.get();` <!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <span style="color:#AD5758;">oczekuje</span> aż wynik będzie dostępny i <span style="color:#AD5758;">zwraca</span> go
* <!-- .element: class="fragment fade-in" --> automatycznie <span style="color:#AD5758;">rzuci</span> przechowywanym wyjątkiem
* <!-- .element: class="fragment fade-in" --> <span style="color:#AD5758;">unieważni</span> <code>future</code>

<div style="background-color: #8B3536; padding: 3px 22px;">

`std::future` może zostać pozyskane tylko raz

</div> <!-- .element: class="fragment fade-in" -->