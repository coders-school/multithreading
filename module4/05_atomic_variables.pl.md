<!-- .slide: data-background="#111111" -->

# Zmienne atomowe

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Zwykłe zmienne vs zmienne atomowe

Zwykłe zmienne
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> jednoczesny zapis i odczyt == niezdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> potrzeba blokowania muteksami w przypadku modyfikacji

Zmienne atomowe
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> jednoczesny zapis i odczyt == zdefiniowane zachowanie
* <!-- .element: class="fragment fade-in" --> brak dodatkowych mechanizmów blokowania

___

## Zmienne atomowe

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;atomic&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Lekka synchronizacja
* <!-- .element: class="fragment fade-in" --> Pozwala na prostą arytmetykę i operacje bitowe: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typowo: liczby, wskaźniki
* <!-- .element: class="fragment fade-in" --> Najważniejsze operacje:
  * <!-- .element: class="fragment fade-in" --> <code>store()</code> - zapisuje wartość w zmiennej atomowej, dodatkowo można podać <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator=()</code> - zapisuje wartość w zmiennej atomowej
  * <!-- .element: class="fragment fade-in" --> <code>load()</code> – odczytuje wartość ze zmiennej atomowej, dodatkowo można podać <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator T()</code> – odczytuje wartość ze zmiennej atomowej

___
<!-- .slide: style="font-size: .75em" -->

## Zadanie: synchronizacja danych

<div style="display: flex;">

<div style="width: 50%; font-size: .9em;">

```c++
vector<int> generateContainer() {
    vector<int> input =
        {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
    vector<int> output;
    vector<thread> threads;
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back([&]{
            output.push_back(input[i]);
        });
    }
    for (auto && t : threads) {
        t.join();
    }
    return output;
}
vector<int> generateOtherContainer() {
    int start = 5;
    bool add = true;
    vector<int> output;
    vector<thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&]{
            if (add)
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; font-size: .9em;">

```c++
                output.push_back(start+=i);
            else
                output.push_back(start-=i);
            add = !add;
        });
    }
    for (auto && t : threads) {
        t.join();
    }
    return output;
}

void powerContainer(vector<int>& input) {
    vector<thread> threads;
    for (auto i = 0u; i < input.size(); i++) {
        threads.emplace_back([&]{
            input[i]*=input[i];
        });
    }
    for (auto && t : threads) {
        t.join();
    }
}
```

</div> <!-- .element: class="fragment fade-in" -->

</div>

```bash
$> g++ 01_synchronization.cpp -Wall -Wextra -Werror -pedantic -lpthread -fsanitize=thread –O3
$> ./a.out
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Użyj właściwych mechanizmów synchronizacji

___
<!-- .slide: style="font-size: .9em" -->

## Rozwiązanie - synchronizacja danych

* <!-- .element: class="fragment fade-in" --> operacje wstawiania na vector powinny być synchronizowane za pomocą mutexu
* <!-- .element: class="fragment fade-in" --> wstawianie wątków do vectora threads jest wykonywane sekwencyjnie w pętli – nie trzeba tego synchronizować
* <!-- .element: class="fragment fade-in" --> gdy każdy wątek pisze do innego elementu vectora synchronizacja nie jest potrzebna
* <!-- .element: class="fragment fade-in" --> proste typy powinny być opakowane w typ <code>atomic</code>
  * <!-- .element: class="fragment fade-in" --> <code>bool add</code>
  * <!-- .element: class="fragment fade-in" --> <code>int start</code>
  * <!-- .element: class="fragment fade-in" --> funkcje lambda powinny być wyciągnięte przed pętle

```bash
$> g++ 01_synchronization.cpp -Wall -Wextra -Werror -pedantic -lpthread -fsanitize=thread –O3
$> ./a.out
2 4 6 8 10 1 3 5 7 9
4 16 36 64 100 1 9 25 49 81
1 0 2 -1 3 -2 4 -3 5 -4
1 0 4 1 9 4 16 9 25 16
```
<!-- .element: class="fragment fade-in" -->
