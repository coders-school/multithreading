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

```cpp []
vector<int> v{10};
thread t1([&]{ v[0] = 5; });
thread t2([&]{ v[1] = 15; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Synchronization is needed when you are writing something concurrently to the same memory areas
* <!-- .element: class="fragment fade-in" --> Synchronization is needed if at least one thread is writing and others are reading the same area of ​​memory
* <!-- .element: class="fragment fade-in" --> No synchronization when required == race condition == undefined behavior
* <!-- .element: class="fragment fade-in" --> <code>const</code> implies multi-threaded security, because it guarantees reading only
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">C++ Memory model on cppreference.com</a>

___

## Is Synchronization Needed Here?

```cpp []
struct S {
    char a;
    int b;
} obj;
thread t1([&]{ obj.a = 'a'; });
thread t2([&]{ obj.b = 4; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Since C++11 no. Despite the same structure, the memory areas in which we save data are separate
* <!-- .element: class="fragment fade-in" --> Race condition is possible in old POSIX threads

___

## Is Synchronization Needed Here?

```cpp []
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
    thread t([&]{ v[i] = i; });
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> YES
* <!-- .element: class="fragment fade-in" --> There is a data race on variable <code>i</code>
* <!-- .element: class="fragment fade-in" --> But the vector access is race free
* <!-- .element: class="fragment fade-in" --> Despite the same structure, the memory areas in which we save data are separate
* <!-- .element: class="fragment fade-in" --> Proper version that do not require synchronization:

```cpp []
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
   thread t([&, i]{ v[i] = i; });
```
<!-- .element: class="fragment fade-in" -->

___

## Is Synchronization Needed Here?

```cpp []
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

## How to synchronize?

How to sync writing / writing + reading?

* <!-- .element: class="fragment fade-in" --> <code>std::mutex</code>

```cpp []
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

* <!-- .element: class="fragment fade-in" --> <code>std::atomic&lt;T&gt;</code>

```cpp []
atomic<int> a = 0;
thread t1([&]{ a = 1; });
thread t2([&]{ a = 2; });
```
<!-- .element: class="fragment fade-in" -->
