<!-- .slide: data-background="#111111" -->

# C++ memory model

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Memory model

* <!-- .element: class="fragment fade-in" --> Smallest unit - 1 byte
* <!-- .element: class="fragment fade-in" --> Each byte has a unique memory address
* <!-- .element: class="fragment fade-in" --> Synchronization is not needed if you are writing something multithreaded to different areas of memory

  ```cpp
  vector<int> v{10};
  thread t1([&]{ v[0] = 5; });
  thread t2([&]{ v[1] = 15; });
  ```

* <!-- .element: class="fragment fade-in" --> Synchronization is needed when writing something multithreaded to the same memory areas
* <!-- .element: class="fragment fade-in" --> Synchronization is needed if at least one thread is writing and others are reading the same area of ​​memory
* <!-- .element: class="fragment fade-in" --> No synchronization when required == race == undefined behavior
* <!-- .element: class="fragment fade-in" --> <code>const</code> implies multi-threaded security because it only guarantees read

[C ++ Memory model on cppreference.com](https://en.cppreference.com/w/cpp/language/memory_model)
<!-- .element: class="fragment fade-in" -->

___

## Is Synchronization Needed Here?

```cpp
struct S {
    char a;
    int b;
} obj;
thread t1([&]{ obj.a = 'a'; });
thread t2([&]{ obj.b = 4; });
```

* <!-- .element: class="fragment fade-in" --> Race is possible in old POSIX threads
* <!-- .element: class="fragment fade-in" --> As of C++11, the code is valid and does not need to be synchronized
* <!-- .element: class="fragment fade-in" --> Despite the same structure, the memory areas in which we save data are disjoint

___

## Is Synchronization Needed Here?

```cpp
vector<int> v(10, 0);
for (int = 0; i < 10; i++)
   thread t([&]{ v[i] = i;
});
```

* <!-- .element: class="fragment fade-in" --> No, despite the same structure, the memory areas in which we save data are disjoint

___

## Is Synchronization Needed Here?

```cpp
vector<int> v;
for (int = 0; i < 10; i++)
    thread t([&]{ v.emplace_back(i);
});
```

* <!-- .element: class="fragment fade-in" --> YES
* <!-- .element: class="fragment fade-in" --> You have to increment the iterator when throwing in a new object <code>end()</code> - possible race
* <!-- .element: class="fragment fade-in" --> When uploading a new object, the vector may be reallocated. Some threads may have iterators on the deprecated vector position.

___

## Ways of synchronization

<div style="display: flex">

<div class="fragment fade-in" style="width: 50%">

### `std::mutex`

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

</div>

<div class="fragment fade-in" style="width: 50%">

### `std::atomic<T>`

```cpp
atomic<int> a = 0;
thread t1([&]{ a = 1; });
thread t2([&]{ a = 2; });
```

</div>

</div>

___

## Type `std::atomic<T>`

* <!-- .element: class="fragment fade-in" --> <code>#include <atomic></code>
* <!-- .element: class="fragment fade-in" --> <code>std::atomic</code>
* <!-- .element: class="fragment fade-in" --> Light synchronization
* <!-- .element: class="fragment fade-in" --> Allows simple arithmetic and bitwise operations: ++, -, + =, - =, & =, | =, ^ =
* <!-- .element: class="fragment fade-in" --> Typically: numbers, pointers
* <!-- .element: class="fragment fade-in" --> The most important operations:
  * <code>operator=()</code> - saves a value in an atomic variable
  * <code>store()</code> - saves a value in an atomic variable, you can also specify <code>std::memory_order</code>
  * <code>operator T()</code> - reads a value from an atomic variable
  * <code>load()</code> - reads a value from an atomic variable, you can also specify <code>std::memory_order</code>

`std::atomic<T>` will be touched on in a separate training course
<!-- .element: class="fragment fade-in" -->
