<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### The riddle

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### The riddle

<div style="display: flex;">

<div style="width: 50%;">

```c++
#include <iostream>
#include <string>
#include <future>

int main() {
  std::string x = "x";
  std::async(std::launch::async, [&x](){
    x = "y";
  });
  std::async(std::launch::async, [&x](){
    x = "z";
  });
  std::cout << x;
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> What will be displayed on screen?
  * <!-- .element: class="fragment fade-in" --> x
  * <!-- .element: class="fragment fade-in" --> y
  * <!-- .element: class="fragment fade-in" --> z
  * <!-- .element: class="fragment fade-in" --> It depends (on what?)

</div> <!-- .element: class="fragment fade-in" -->

<div>

___

### Odpowiedź

<div style="display: flex;">

<div style="width: 50%;">

```c++
#include <iostream>
#include <string>
#include <future>

int main() {
  std::string x = "x";
  std::async(std::launch::async, [&x](){
    x = "y";
  });
  std::async(std::launch::async, [&x](){
    x = "z";
  });
  std::cout << x;
}
```
<!-- .element: class="fragment fade-in" -->
```bash
$> ./06_riddle
z
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 50%; background-color: #8B3536; padding: 5px 10px; font-size: .86em;">

* <!-- .element: class="fragment fade-in" --> What will be displayed on screen?
  * <!-- .element: class="fragment fade-in" --> x
  * <!-- .element: class="fragment fade-in" --> y
  * <!-- .element: class="fragment fade-in" --> z
  * <!-- .element: class="fragment fade-in" --> It depends (on what?)
* <!-- .element: class="fragment fade-in" --> Answer:
  * <!-- .element: class="fragment fade-in" --> z
* <!-- .element: class="fragment fade-in" --> Explanation:
  * <!-- .element: class="fragment fade-in" --> if the future object is temporary, it waits in the destructor until the task is over. So the second task will be run after the first one and even though they will be in other threads, their execution will be synchronized
  * <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/future/-future">cppreference -future</a>
  * <!-- .element: class="fragment fade-in" --> <a href="http://cppquiz.org/quiz/question/48">Source</a>
  * <!-- .element: class="fragment fade-in" --> std::futures from std::async aren't special! - Scott Meyers
* <!-- .element: class="fragment fade-in" --> Conclusions:
  * <!-- .element: class="fragment fade-in" --> If you want to have asynchronous calls, you have to save the result in the <code>std::future</code> variable

</div> <!-- .element: class="fragment fade-in" -->

<div>
