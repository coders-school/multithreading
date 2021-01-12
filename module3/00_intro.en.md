## Something about you

* One of the most interesting things from Cpp Core Guidelines pre-work?
* Do you prefer threads or async?

___
<!-- .slide: data-background="img/lukasz.jpg" -->

<h2 style = "text-shadow: 2px 2px black;"> Łukasz Ziobroń </h2>
<div class="box fragment" style="width: 45%; left: 0; top: 100px;">

### Experience not only in programming

* Trainer and DevOps @ Coders School
* C ++ and Python developer @ Nokia and Credit Suisse
* Team leader and trainer @ Nokia
* Scrum Master @ Nokia & Credit Suisse
* Code Reviewer @ Nokia
* Web developer (HTML, PHP, CSS) @ StarCraft Area

</div>

<div class="box fragment" style="width: 45%; right: 0; top: 100px;">

### Experience as a trainer

* C ++ courses @ Coders School
* Practical Aspects of Software Engineering @ PWr & UWr
* Nokia Academy
* Internal corporate training

</div>

<div class="box fragment" style="width: 45%; left: 0; top: 400px;">

### Experience in public speaking

* code::dive conference
* code::dive community
* Polish Academic Championship in Team Programming

</div>

<div class="box fragment" style="width: 45%; right: 0; top: 400px;">

### Interests

* StarCraft Brood War & StarCraft II
* Motorcycles
* Photography
* Archery
* Andragogy
* Marketing 🆕

</div>

___

## Contract

* <!-- .element: class="fragment fade-in" --> 🎰 The Vegas rule
* <!-- .element: class="fragment fade-in" --> 🗣 Discussion, not lecture
* <!-- .element: class="fragment fade-in" --> ☕️ Additional breaks on demand
* <!-- .element: class="fragment fade-in" --> ⌚️ Punctuality

___
<!-- .slide: style="font-size: 0.7em" -->

## Pre-test

```cpp
#include <future>
#include <iostream>

int main() {
    int x = 0;
    auto f = std::async(std::launch::deferred, [&x]{
        x = 1;
    });

    x = 2;
    f.get();
    x = 3;
    std::cout << x;
    return 0;
}
```

1. the type of f is <code>promise&lt;int&gt;</code>
2. the type of f is <code>future&lt;void&gt;</code>
3. running <code>async()</code> without a launch policy may cause an undefined behavior
4. this program always prints 3
5. <code>x = 2</code> assignment cause a data race
6. if async was run with <code>std::launch::async</code>, there would be a data race
7. <code>x = 3</code> assignment is safe, because it happens after synchronization with async task
8. <code>future&lt;void&gt;</code> may be used to synchronize tasks

Note: 2, 3, 4, 6, 7, 8
