<!-- .slide: data-background="#111111" -->

# Recap

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Post-work

* Game - [The Deadlock Empire](https://deadlockempire.github.io)
* Implementation - The dining philosophers problem (next slide)
* [Post-test](https://forms.gle/fXYA14PZaGKX5k1x5)
* [Training evaluation](https://forms.gle/ADXRttpAaZgW8KwM6)

___

### Homework

Implement the dining philosophers problem using threads and mutexes.

* <!-- .element: class="fragment fade-in" --> Each philosopher is controlled by a separate thread
* <!-- .element: class="fragment fade-in" --> Each fork is protected by 1 mutex
* <!-- .element: class="fragment fade-in" --> Try to visualize the problem
* <!-- .element: class="fragment fade-in" --> Beware of deadlocks
* <!-- .element: class="fragment fade-in" --> Play around with the number of philosophers and see if the behavior of the program changes
* <!-- .element: class="fragment fade-in" --> Additional task: prevent starvations. This is a situation where at least 1 thread will never get all the required resources due to implementation or its lower priority.

___

## What was new for you?

## What was the most difficult for you?

___
<!-- .slide: style="font-size: .81em" -->

### Summary

* <!-- .element: class="fragment fade-in" --> Sharing read-only data is secure and does not require locking. Word <code>const</code> guarantees data security.
* <!-- .element: class="fragment fade-in" --> When a mutex is locked in a thread, any other thread wishing to lock it must wait for unlocking. Blocking too large critical sections is a mistake.
* <!-- .element: class="fragment fade-in" --> The deadlock problem occurs when 2 mutexes (or more) may be blocked in a different order by different threads, or we have 1 mutex that will not be unlocked (eg. due to an exception) and the program will not call <code>unlock()</code> function.
* <!-- .element: class="fragment fade-in" --> Mutexes slow down the program execution, but keep your data safe, if used correctly. The main cost is the waiting time on <code>lock()</code> function. Mutex lock/unlock operations are delegated to kernel-space, but usually, their cost may be neglected.
* <!-- .element: class="fragment fade-in" --> In small programs, mutexes can be kept as global objects, but in larger programs, they should be kept in an appropriate class whose operations are to be blocked. If such a class has several resources that can be modified independently, then you should have several mutexes (1 for each resource). You have to watch out for deadlocks.
* <!-- .element: class="fragment fade-in" --> Despite the blocked mutexes, if we use pointers or references to objects somewhere outside the critical section, all protection is for nothing. Therefore, data protection with mutexes requires careful interface modelling.
* <!-- .element: class="fragment fade-in" --> Always use mutexes via lock managers (RAII wrappers)
* <!-- .element: class="fragment fade-in" --> Use <code>std::atomic<T></code> (atomic variables). They provide a convenient way to lightly synchronize data without the use of mutexes, but more on that will be covered in the next training

___

### Useful links

* <!-- .element: class="fragment fade-in" --> <a href="https://people.freebsd.org/~lstewart/articles/cpumemory.pdf">What every programmer should know about memory</a> - Ulrich Drepper
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">The C ++ memory model</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.wikipedia.org/wiki/Dining_philosophers_problem">The dining philosophers problem</a>
