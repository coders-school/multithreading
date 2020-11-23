<!-- .slide: data-background="#111111" -->

# Mutex - good practices

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## How to block?

Always use a mutex with the appropriate manager (RAII wrapper):

* <!-- .element: class="fragment fade-in" --> <code>lock_guard</code> - the simplest, main choice
* <!-- .element: class="fragment fade-in" --> <code>unique_lock</code> - delayed blocking, time-limited blocking attempts, ...
* <!-- .element: class="fragment fade-in" --> <code>shared_lock</code> - shared lock for reading variables
* <!-- .element: class="fragment fade-in" --> If threads need to block several mutexes, they should always be blocked in the same order on every thread that uses them. This prevents deadlocks
* <!-- .element: class="fragment fade-in" --> <code>scoped_lock</code> - blocks several mutexes to prevent deadlock
* <!-- .element: class="fragment fade-in" --> Each RAII wrapper must be named!

___

## When to block?

* <!-- .element: class="fragment fade-in" --> Block whenever necessary
* <!-- .element: class="fragment fade-in" --> Avoid blocking wherever possible
* <!-- .element: class="fragment fade-in" --> The duration of the blockage should be as short as possible
  * <!-- .element: class="fragment fade-in" --> If there is a lot of data to be modified and they are not arranged next to each other in memory, the duration of the mutex lock will be very long, so that all data can be downloaded to the cache memory.
  * <!-- .element: class="fragment fade-in" --> I / O operations (input / output) such as reading and writing files, downloading data via the network is a long or generally calling a system function is a long process

___

## Where to block?

* <!-- .element: class="fragment fade-in" --> The mutex should exist along with the locking object it is used for
* <!-- .element: class="fragment fade-in" --> If a mutex blocks access to a class field, it should exist in that class
* <!-- .element: class="fragment fade-in" --> M&M rule - mutable & mutex go together (in classes)
* <!-- .element: class="fragment fade-in" --> Each resource should have its own mutex, as this simplifies the locking logic (we use <code>scoped_lock</code>)
* <!-- .element: class="fragment fade-in" --> In really rare cases, one mutex can block several objects, but you really need to think very carefully about the logic and all timing
