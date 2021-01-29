<!-- .slide: data-background="#111111" -->

# `std::memory_order`

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Normal variables vs Atomic variables

Normal variables
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> simultaneous write and read == undefined behavior
* <!-- .element: class="fragment fade-in" --> need to block with mutexes in case of modification

Atomic variables
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> simultaneous write and read == defined behavior
* <!-- .element: class="fragment fade-in" --> no additional locking mechanisms required

___

## Atomic variables

* <!-- .element: class="fragment fade-in" --> <code>#include &lt;atomic&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Light synchronization
* <!-- .element: class="fragment fade-in" --> Allows simple arithmetic and bitwise operations: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typically: numbers, pointers
* <!-- .element: class="fragment fade-in" --> The most important operations:
  * <!-- .element: class="fragment fade-in" --> <code>store()</code> - saves the value in an atomic variable, you can also specify <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator=()</code> - saves the value in an atomic variable
  * <!-- .element: class="fragment fade-in" --> <code>load()</code> - reads the value from an atomic variable, you can also specify <code>std::memory_order</code>
  * <!-- .element: class="fragment fade-in" --> <code>operator T()</code> - reads the value from an atomic variable

___

## `std::memory_order`

* <!-- .element: class="fragment fade-in" --> As part of optimization, the compiler has the right to change the order of operations
* <!-- .element: class="fragment fade-in" --> The compiler needs to know which operations can be rearranged and which must follow a certain order
* <!-- .element: class="fragment fade-in" --> SC - Sequential consistency (<code>memory_order_seq_cst</code>) ensures that the order of operations set by the developer is preserved, sometimes at the expense of performance. This is the default behavior of <code>std::atomic</code> variables
* <!-- .element: class="fragment fade-in" --> Thanks to SC, we can correctly conclude what values may variables have regardless of the processor optimization
  * <!-- .element: class="fragment fade-in" --> Compiler optimizations cannot create race conditions
  * <!-- .element: class="fragment fade-in" --> Boring Details: <a href="https://en.cppreference.com/w/cpp/atomic/memory_order">memory_order on cppreference.com</a>

___

## Sequential consistency and compiler optimizations

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

* <!-- .element: class="fragment fade-in" --> Correct?
* <!-- .element: class="fragment fade-in" --> Only OPT1
* <!-- .element: class="fragment fade-in" --> In OPT2, state b has changed regardless of the value of a
* <!-- .element: class="fragment fade-in" --> Another thread may have read b value at this time

___

## Atomic variable - Summary

* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code> it's a light synchronization
* <!-- .element: class="fragment fade-in" --> Allows simple arithmetic and bitwise operations: ++, --, +=, -=, &=, |=, ^=
* <!-- .element: class="fragment fade-in" --> Typically: numbers, pointers
* <!-- .element: class="fragment fade-in" --> Uses special processor instructions to modify data atomically
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code> doesn't make sense on complex types
  * <!-- .element: class="fragment fade-in" --> there are no special processor instructions that ensure the indivisibility of such operations
  * <!-- .element: class="fragment fade-in" --> no transactional memory model in C++ (yet)
  * <!-- .element: class="fragment fade-in" --> if it is successful, it may not work as intended (see <a href="https://stackoverflow.com/questions/46069345/is-it-possible-to-create-an-atomic-vector-or-array-in-c/46069630">Stack Overflow</a>)
  * <!-- .element: class="fragment fade-in" --> you must use mutexes
