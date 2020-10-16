<!-- .slide: data-background="#111111" -->
# Passing parameters

___

## Task 1: passing parameters

```cpp
#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    // run add function in a thread
    // pass 3 and 4 as arguments
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Create a thread and run <code>add()</code> function on it, passing the numbers 3 and 4 to it

___

## Task 1 - solution

```cpp
#include <thread>
#include <iostream>
using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    thread t(add, 5, 6);
    t.join();
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> How to pass the result of a calculation back to <code>main()</code> function?
  * <!-- .element: class="fragment fade-in" --> You can't do it through <code>return</code>, threads don't do that
  * <!-- .element: class="fragment fade-in" --> You can write something in a global variable, but it's asking for trouble - synchronization
  * <!-- .element: class="fragment fade-in" --> The correct way is to pass a reference to the variable that we will modify in the thread as a parameter

___

## Exercise 2: passing references

```cpp
#include <thread>
#include <iostream>
using namespace std;

void add10(int & a)
{
    a += 10;
}

int main()
{
    // run add10 function in a thread
    // pass 5 as an argument and read it's value
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Create a thread and run <code>add10()</code> function on it, passing the number 5 to it
* <!-- .element: class="fragment fade-in" --> Write the result on the screen

___

## Task 2 - solution

```cpp
void add10(int & a) {
    a += 10;
}

int main() {
    int five = 5;
    thread t(add10, ref(five));
    cout << five << endl;
    t.join();
    cout << five << endl;
    return 0;
}
```

```output
$> g++ zadanie2.cpp –lpthread
$> ./a.out
5
15
```

* <!-- .element: class="fragment fade-in" --> <code>std::ref()</code> causes the object to be passed by reference

___
<!-- .slide: style="font-size: 0.8em" -->
## Exercise 3: passing a class method

```cpp
#include <thread>
#include <iostream>
#include <string>
using namespace std;

class Car {
    int production_year;
    string model_name;
public:
    void setData(int year, const string & model) {
        production_year = year;
        model_name = model;
    }
    void print() {
        cout << model_name << " " << production_year << endl;
    }
};

int main() {
    Car toyota;
    // set year to 2015, model to "Corolla" in a thread
    toyota.print();
    return 0;
}
```

* <!-- .element: class="fragment fade-in" --> Create a thread and run <code>setData()</code> method on it, which will set the toyota object year of production to 2015 and the model to "Corolla"

___
<!-- .slide: style="font-size: 0.85em" -->
## Task 3 - solution

```cpp
class Car {
    int production_year;
    string model_name;
public:
    void setData(int year, const string & model) {
        production_year = year;
        model_name = model;
    }
    void print() {
        cout << model_name << " " << production_year << endl;
    }
};

int main() {
    Car toyota;
    thread t(&Car::setData, &toyota, 2015, "Corolla");
    t.join();
    toyota.print();
    return 0;
}
```

```output
$> g++ zadanie3.cpp –lpthread
$> ./a.out
Corolla 2015
```
<!-- .element: class="fragment fade-in" -->

___

## Task 3 - solution

* <!-- .element: class="fragment fade-in" --> Why there is no <code>std::ref()</code> next to "Corolla" parameter?
  * <!-- .element: class="fragment fade-in" --> temporary objects can be attached to <code>const &</code>
  * <!-- .element: class="fragment fade-in" --> using <code>std::ref("Corolla")</code> will give us a reference to a temporary variable (in this case it's safe)
  * <!-- .element: class="fragment fade-in" --> there is a danger of passing a dangling reference

___

## Passing parameters

* <!-- .element: class="fragment fade-in" --> The constructor of the thread as the first parameter gets any callable object - lambda, function, functor. Callable is copied into thread memory.
* <!-- .element: class="fragment fade-in" --> Successive parameters of the thread's constructor are passed to the function (lambdas)
* <!-- .element: class="fragment fade-in" --> Parameters are forwarded (copied or moved) to thread memory.
* <!-- .element: class="fragment fade-in" --> Passing a reference is done by using <code>std::ref()</code>
* <!-- .element: class="fragment fade-in" --> Passing a constant reference is done by using <code>std::cref()</code>
* <!-- .element: class="fragment fade-in" --> A class method that starts in a thread as the first parameter takes hidden pointer to the object on which it is to be called.

___

## Passing parameters

```cpp
#include <thread>

void foo() { /* ... */ }
// foo() – function without parameters
std::thread t1(&foo);

void bar(int a, int & b) { /* ... */ }
int field = 5;
// bar(1, field) – function with parameters
std::thread t2(&bar, 1, std::ref(field));

struct SomeClass {
    void method(int a, int b, int & c) { /* ... */ }
};
SomeClass someObject;
// someObject.method(1, 2, field) – class method
std::thread t3(&SomeClass::method, someObject, 1, 2, std::ref(field));
```

___

## Traps when passing arguments

* What are the pitfalls of the code below?

```cpp
void f(int i, std::string const& s);
void oops(int arg)
{
    char buffer[1024];
    sprintf(buffer, "%i", arg);
    std::thread t(f, 3, buffer);
    t.detach();
}
```
<!-- .element: class="fragment fade-in" -->

___

## Traps when passing arguments

* <!-- .element: class="fragment fade-in" --> Pay attention to the passed arguments
  * <!-- .element: class="fragment fade-in" --> If the variable is a pointer/reference, its lifespan should be longer than lifespan of the thread that operates on it.
  * <!-- .element: class="fragment fade-in" --> If there is a risk of implicit conversion, it's best to pass the converted argument straight away.

```cpp
void f(int i, std::string const& s);
void not_oops(int arg)
{
    char buffer[1024];
    sprintf(buffer, "%i", arg);
    std::thread t(f, 3, std::string(buffer));
    t.detach();
}
```
<!-- .element: class="fragment fade-in" -->
