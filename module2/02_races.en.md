<!-- .slide: data-background="#111111" -->

# Race conditions

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Are there races here?

<div style="display:flex;">

<div style="width:60%;">

```c++
#include <thread>
#include <iostream>
#include <functional>

void abc(int &a) { a = 2; }
void def(int &a) { a = 3; }

int main() {
    int x = 1;
    std::thread t1(abc, std::ref(x));
    std::thread t2(def, std::ref(x));

    t1.join();
    t2.join();

    std::cout << x << std::endl;
}
```

</div>

<div>

What will appear on the screen?<!-- .element: class="fragment fade-in" -->

```bash
$> g++ 01_threads_write.cpp -lpthread
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
3
$> ./a.out
2
```
<!-- .element: class="fragment fade-in" style="transform: scale(.90); width: 106%; margin:-35px 0 0 -30px;" -->

</div>

</div>
<!-- Wartości w stylowaniu zostały nadane w celu ładniejszego wyświetlania w Reveal.js. Na podglądzie zwykłego md również wygląda to dobrze. Przynajmniej u mnie. -->

___

## Race conditions

* <!-- .element: class="fragment fade-in" --> Races generate nondeterministic (random) program behavior.
* <!-- .element: class="fragment fade-in" --> Race = Undefined behavior.
* <!-- .element: class="fragment fade-in" --> Unwanted side effects usually do not occur during most runs of the procedure.
* <!-- .element: class="fragment fade-in" --> Problems may occur less than 1 in 1,000 runs.
* <!-- .element: class="fragment fade-in" --> Racing is incredibly hard to spot. Usually, when the CPU is not under load, everything works as it should. The more loaded the CPU, the more the risk of a different order of data access increases. Such problems are difficult to reproduce.
* <!-- .element: class="fragment fade-in" --> The racing problem is time-critical and can be completely impossible to catch under the debugger. The debugger affects the execution times of individual statements.
* <!-- .element: class="fragment fade-in" --> It is much easier to prevent races because treatment is time-consuming and expensive.
* <!-- .element: class="fragment fade-in" --> Thread Sanitizer (TSan) - data race detector.

___
<!-- .slide: data-background="img/tsan.png" data-background-position="left top" -->
## Thread sanitizer

```bash
$> g++ 01_threads_write.cpp -lpthread -fsanitize=thread -O2 -g
$> ./a.out
```

___

### Purpose: reproduction of races under the debugger

<div style="display: flex;">

<div>

* <!-- .element: class="fragment fade-in" --> Compile threads_write.cpp </br><code>g++ 01_threads_write.cpp -lpthread -g</code>
* <!-- .element: class="fragment fade-in" --> Run the program under the debugger (gdb or other) </br><code>gdb --tui ./a.out</code>
* <!-- .element: class="fragment fade-in" --> Make x have the value 2 at the end of the program
* <!-- .element: class="fragment fade-in" --> Make x have the value 3 at the end of the program
* <!-- .element: class="fragment fade-in" --> Useful commands:
  * <!-- .element: class="fragment fade-in" --> <code>b 5</code> - sets the breakpoint on the fifth line
  * <!-- .element: class="fragment fade-in" --> <code>watch x</code> - observing changes in variable x (the debugger will stop when it is modified)
  * <!-- .element: class="fragment fade-in" --> <code>c</code> - continuing debugging
  * <!-- .element: class="fragment fade-in" --> <code>info threads</code> - thread information
  * <!-- .element: class="fragment fade-in" --> <code>thread 3</code> - switch to thread 3
  * <!-- .element: class="fragment fade-in" --> <code>n</code> - next instruction
  * <!-- .element: class="fragment fade-in" --> <code>fin</code> - execute everything until the end of the current function
  * <!-- .element: class="fragment fade-in" --> <code>up</code> - transition to the higher stack frame
  * <!-- .element: class="fragment fade-in" --> <code>down</code> - transition to the lower stack frame
  * <!-- .element: class="fragment fade-in" --> <code>del br</code> - removing all breakpoints
  * <!-- .element: class="fragment fade-in" --> <code>CTRL + L</code> - view refresh

</div><!-- .element: style="font-size: .9em; margin: 0 10px" -->

<div>

One of the possible sequences:

* <!-- .element: class="fragment fade-in" --> <code>b 5</code>
* <!-- .element: class="fragment fade-in" --> <code>b 17</code>
* <!-- .element: class="fragment fade-in" --> <code>r</code>
* <!-- .element: class="fragment fade-in" --> <code>c</code>
* <!-- .element: class="fragment fade-in" --> <code>p x</code>
  * <!-- .element: class="fragment fade-in" --> <code>$ = 2</code>
* <!-- .element: class="fragment fade-in" --> <code>del br</code>
* <!-- .element: class="fragment fade-in" --> <code>b 6</code>
* <!-- .element: class="fragment fade-in" --> <code>b 17</code>
* <!-- .element: class="fragment fade-in" --> <code>r</code>
* <!-- .element: class="fragment fade-in" --> <code>c</code>
* <!-- .element: class="fragment fade-in" --> <code>p x</code>
  * <!-- .element: class="fragment fade-in" --> <code>$ = 3</code>

</div><!-- .element: class="fragment fade-in" style="font-size: .9em; background-color: #252627; padding: 20px" -->

</div>
