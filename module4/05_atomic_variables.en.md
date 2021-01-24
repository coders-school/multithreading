<!-- .slide: data-background="#111111" -->

# Atomic variables

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Ordinary variables vs Atomic variables

Ordinary variables
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> simultaneous write and read == undefined behavior
* <!-- .element: class="fragment fade-in" --> need to block with mutexes in case of modification

Atomic variables
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> simultaneous write and read == defined behavior
* <!-- .element: class="fragment fade-in" --> no additional locking mechanisms

___

## Atomic variables

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;atomic&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Light synchronization
* <!-- .element: class="fragment fade-in" --> Allows simple arithmetic and bitwise operations: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typically: numbers, pointers
* <!-- .element: class="fragment fade-in" --> The most important operations:
  * <!-- .element: class="fragment fade-in" --> <code>store()</code> - saves a value in an atomic variable, you can also specify <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator=()</code> - saves a value in an atomic variable
  * <!-- .element: class="fragment fade-in" --> <code>load()</code> - reads a value from an atomic variable, you can also specify <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator T()</code> - reads value from an atomic variable

___
<!-- .slide: style="font-size: .75em" -->

## Exercise: data synchronization

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

* <!-- .element: class="fragment fade-in" --> Use the correct synchronization mechanisms

___
<!-- .slide: style="font-size: .9em" -->

## Solution - data synchronization

* <!-- .element: class="fragment fade-in" --> vector insertions should be synchronized using a mutex
* <!-- .element: class="fragment fade-in" --> inserting threads into <code>threads</code> vector is performed sequentially in a loop - no need to synchronize it
* <!-- .element: class="fragment fade-in" --> when each thread writes to a different element of vector, synchronization is not needed
* <!-- .element: class="fragment fade-in" --> simple types should be wrapped in a <code>atomic</code> type
  * <!-- .element: class="fragment fade-in" --> <code>bool add</code>
  * <!-- .element: class="fragment fade-in" --> <code>int start</code>
  * <!-- .element: class="fragment fade-in" --> lambda functions should be pulled out from loops

```bash
$> g++ 01_synchronization.cpp -Wall -Wextra -Werror -pedantic -lpthread -fsanitize=thread –O3
$> ./a.out
2 4 6 8 10 1 3 5 7 9
4 16 36 64 100 1 9 25 49 81
1 0 2 -1 3 -2 4 -3 5 -4
1 0 4 1 9 4 16 9 25 16
```
<!-- .element: class="fragment fade-in" -->
