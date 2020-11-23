<!-- .slide: data-background="#111111" -->

# Summary

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .81em" -->

### Summary

* <!-- .element: class="fragment fade-in" --> Sharing read-only data is secure and does not require locking. Word <code>const</code> guarantees data security (read-only).
* <!-- .element: class="fragment fade-in" --> When a thread locks a mutex, any other thread wishing to lock it must wait for it to be unlocked. Blocking too large critical sections is a mistake.
* <!-- .element: class="fragment fade-in" --> The deadlock problem occurs when 2 mutexes (or more) are blocked by threads in different order, or we have 1 mutex that will not be unlocked due to an exception, for example, and the program will not go to the unlock () function.
* <!-- .element: class="fragment fade-in" --> Mutexes significantly slow down program execution, but keep your data safe if used correctly.
* <!-- .element: class="fragment fade-in" --> In small programs, mutexes can be kept as global objects, but in larger programs they should be kept in an appropriate class whose operations are to be blocked. If such a class has several resources that can be modified independently, then you should have several mutexes (1 for each resource). You have to watch out for jamming.
* <!-- .element: class="fragment fade-in" --> Despite the blocked mutexes, if we use pointers or references to objects somewhere that we modify in the critical section, all protection is for nothing. Therefore, data protection with mutexes requires careful interface modeling.
* <!-- .element: class="fragment fade-in" --> Always use mutexes via lock managers (RAII wrappers)
* <!-- .element: class="fragment fade-in" --> Use <code>std::atomic<T></code> (atomic variables). They provide a convenient way to lightly synchronize data without the use of mutexes, but more on that in the next section

___

### Post-work

* Game - [The Deadlock Empire](https://deadlockempire.github.io)
* [Post-test](https://forms.gle/fXYA14PZaGKX5k1x5)
* [Training evaluation](https://forms.gle/ADXRttpAaZgW8KwM6)

___

### Homework

* <!-- .element: class="fragment fade-in" --> Implement the feasting philosophers problem using threads and mutexes.
  * <!-- .element: class="fragment fade-in" --> Each philosopher is to be controlled by a separate thread.
  * <!-- .element: class="fragment fade-in" --> Each cutlery is to be protected by 1 mutex
  * <!-- .element: class="fragment fade-in" --> Try to visualize the problem
  * <!-- .element: class="fragment fade-in" --> Beware of jams
  * <!-- .element: class="fragment fade-in" --> Play around with the number of philosophers and see if the behavior of the program changes
  * <!-- .element: class="fragment fade-in" --> Additional task: Beware of starvation. This is a situation where at least 1 thread will never get all the required resources due to implementation or its lower priority. To do this, implement some mechanism to prevent starvation.

___

### Useful links

* <!-- .element: class="fragment fade-in" --> <a href="https://people.freebsd.org/~lstewart/articles/cpumemory.pdf">What every programmer should know about memory</a> - Ulrich Drepper
* <!-- .element: class="fragment fade-in" --> <a href="https://en.cppreference.com/w/cpp/language/memory_model">The C ++ memory model</a>
* <!-- .element: class="fragment fade-in" --> <a href="https://en.wikipedia.org/wiki/Dining_philosophers_problem">The dining philosophers problem</a>
