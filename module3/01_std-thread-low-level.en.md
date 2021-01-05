<!-- .slide: data-background="#111111" -->

# Problems with threads

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## `std::thread` is a "low-level" mechanism

* <!-- .element: class="fragment fade-in" --> How to <span style="color:#cf802a">return</span> something from it?
  * <!-- .element: class="fragment fade-in" --> Pass variable by reference (use <code>std::ref</code>)
* <!-- .element: class="fragment fade-in" --> How to forward an <span style="color:#cf802a">exception</span>?
  * <!-- .element: class="fragment fade-in" --> Use <code>std::exception_ptr</code> and <code>std::current_exception</code>
* <!-- .element: class="fragment fade-in" --> Should be manually <span style="color:#cf802a">joined</span>/<span style="color:#cf802a">detached</span>?
  * <!-- .element: class="fragment fade-in" --> Lack of <code>join()</code> or <code>detach()</code> terminates program execution
