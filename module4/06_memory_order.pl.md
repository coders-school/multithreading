<!-- .slide: data-background="#111111" -->

# `std::memory_order`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## `std::memory_order`

* <!-- .element: class="fragment fade-in" --> W ramach optymalizacji kompilator ma prawo zmienić kolejność wykonywanych operacji
* <!-- .element: class="fragment fade-in" --> Kompilator musi wiedzieć, które operacje może przestawiać, a które muszą następować w określonej kolejności
* <!-- .element: class="fragment fade-in" --> SC - Sequential consistency (<code>memory_order_seq_cst</code>) gwarantuje zachowaną kolejność operacji ustaloną przez programistę, czasami kosztem wydajności. Jest to domyślne zachowanie zmiennych <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Dzięki SC możemy poprawnie wnioskować jakie wartości mogą mieć zmienne niezależnie od optymalizacji procesora
  * <!-- .element: class="fragment fade-in" --> Optymalizacje kompilatora nie mogą tworzyć wyścigów
  * <!-- .element: class="fragment fade-in" --> Nudne szczegóły: <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">memory_order on cppreference.com</a>

___

## Sequential consistency i optymalizacje kompilatora

<div style="display: flex;">

<div style="width: 33%; font-size: .9em;">

```c++
// INPUT:
int foo(int a)
{
    if(a<1)
        b=2;
    if(a==2)
        b=2;
    if(a>2)
        b=2;
    return b;
}



```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 33%; font-size: .9em;">

```c++
// OPT1:
int foo(int a)
{
    if(a>2)
        b=2;
    else
        if(a<1)
            b=2;
        else
            if(a==2)
                b=2;
    return b;
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 33%; font-size: .9em;">

```c++
// OPT2:
int foo(int a)
{
    const int tmp=b;
    b=2;
    if(a==1)
        b=tmp;
    return b;
}





```
<!-- .element: class="fragment fade-in" -->
</div>

</div>

* <!-- .element: class="fragment fade-in" --> Poprawne?
* <!-- .element: class="fragment fade-in" --> Tylko OPT1
* <!-- .element: class="fragment fade-in" --> W OPT2 stan b został zmieniony niezależnie od wartości a
* <!-- .element: class="fragment fade-in" --> Inny wątek mógł odczytać wartość b w tym czasie

___

## Zmienna atomowa - podsumowanie

* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code> to lekka synchronizacja
* <!-- .element: class="fragment fade-in" --> Pozwala na prostą arytmetykę i operacje bitowe: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typowo: liczby, wskaźniki
* <!-- .element: class="fragment fade-in" --> Używa specjalnych instrukcji procesora do niepodzielnej modyfikacji danych
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code> na typach złożonych nie ma sensu
  * <!-- .element: class="fragment fade-in" --> nie ma specjalnych instrukcji procesora, które zapewniają niepodzielność takich operacji
  * <!-- .element: class="fragment fade-in" --> nie ma modelu pamięci transakcyjnej w C++ (jeszcze)
  * <!-- .element: class="fragment fade-in" --> jeśli się uda, to może nie działać zgodnie z założeniami (patrz. <a href="https://stackoverflow.com/questions/46069345/is-it-possible-to-create-an-atomic-vector-or-array-in-c/46069630">Stack Overflow</a>)
  * <!-- .element: class="fragment fade-in" --> należy użyć mutexów
