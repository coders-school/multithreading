<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### No policies = Undefined Behavior

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### No policies = Undefined Behavior

* <!-- .element: class="fragment fade-in" --> If both the <code>std::launch::async</code> and <code>std::launch::deferred</code> flags are set in policy, it is up to the implementation whether to perform asynchronous execution or lazy evaluation.
* <!-- .element: class="fragment fade-in" --> (Since C++14) If neither <code>std::launch::async</code> nor <code>std::launch::deferred</code>, nor any implementation-defined policy flag is set in policy, <span>the behaviour is undefined</span>.
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/thread/async">Source</a>
* <!-- .element: class="fragment fade-in" --> Always use <code>std::async()</code> with strictly defined policy
  * <!-- .element: class="fragment fade-in" --> <code>std::launch::async</code>
  * <!-- .element: class="fragment fade-in" --> <code>std::launch::deferred</code>
  * <!-- .element: class="fragment fade-in" --> <code>std::launch::async | std::launch::deferred</code>