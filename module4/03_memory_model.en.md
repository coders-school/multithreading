<!-- .slide: data-background="#111111" -->

# C++ memory model

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## C++ memory model

* <!-- .element: class="fragment fade-in" --> The smallest memory unit is 1 byte
* <!-- .element: class="fragment fade-in" --> Each byte has a unique memory address
* <!-- .element: class="fragment fade-in" --> Synchronization is not needed if you are writing something concurrently to different areas of memory

```cpp
vector<int> v{10};
thread t1([&]{ v[0] = 5; });
thread t2([&]{ v[1] = 15; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Synchronization is needed when you are writing something concurrently to the same memory areas
* <!-- .element: class="fragment fade-in" --> Synchronization is needed if at least one thread is writing and others are reading the same area of ​​memory
* <!-- .element: class="fragment fade-in" --> No synchronization when required == race condition == undefined behavior
* <!-- .element: class="fragment fade-in" --> <code>const</code> implies multi-threaded security, because it guarantees only reading
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>

___

## C++ memory model - when to sync?

### Is synchronization needed here?

<!-- .element: class="fragment fade-in" -->

```cpp
struct S {
    char a;
    int b;
} obj;
thread t1([&]{ obj.a = 'a'; });
thread t2([&]{ obj.b = 4; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Since C++11 no, despite the same structure, the memory areas in which we save data are separate
* <!-- .element: class="fragment fade-in" --> Race condition is possible in old POSIX threads

___

## C++ memory model - when to synchronize?

### Is synchronization needed here?

<!-- .element: class="fragment fade-in" -->

```cpp
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
    thread t([&]{ v[i] = i; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> No, despite the same structure, the memory areas in which we save data are separate

___

## C++ memory model - when to synchronize?

### Is synchronization needed here?

<!-- .element: class="fragment fade-in" -->

```cpp
vector<int> v;
for (int = 0; i < 10; i++)
    thread t([&]{ v.emplace_back(i); });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> YES
* <!-- .element: class="fragment fade-in" --> We have to increment the <code>end()</code> iterator while we are adding new elements - possible race condition
* <!-- .element: class="fragment fade-in" --> When adding a new object, the vector may be reallocated. Some threads may have iterators on the deprecated vector position.

___
<!-- .slide: style="font-size: .95em" -->

## C++ memory model - how to synchronize?

* <!-- .element: class="fragment fade-in" --> How to sync writing / writing + reading?
* <!-- .element: class="fragment fade-in" --> <code>std::mutex</code> - you already know it

```cpp
int a = 0;
mutex m;
thread t1([&]{
    lock_guard<mutex> lg(m);
    a = 1;
});
thread t2([&]{
    lock_guard<mutex> lg(m);
    a = 2;
});
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> <code>std::atomic&lt;T&gt;</code> - we'll get to know it now

```cpp
atomic<int> a = 0;
thread t1([&]{ a = 1; });
thread t2([&]{ a = 2; });
```
<!-- .element: class="fragment fade-in" -->
