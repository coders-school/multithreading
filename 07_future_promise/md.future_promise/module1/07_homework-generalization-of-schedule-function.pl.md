<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Zadanie domowe dla chętnych: generalizacja funkcji `schedule` (level hard)

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie domowe dla chętnych: generalizacja funkcji `schedule`

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
std::future<int> schedule(std::function<int()> func) {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  auto wrapped_func = [func] (std::promise<int> p) {
    try {
      p.set_value(func());
    } catch(...) {
      p.set_exception(std::current_exception());
    }
  };
  std::thread t(wrapped_func, std::move(p));
  t.detach();
  return f;
}
```

</div><!-- .element: class="fragment fade-in" -->

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px;">

* <!-- .element: class="fragment fade-in" --> Zadanie 1. Zmień funkcję <code>schedule()</code> tak, aby mogła przyjąć funkcję każdego typu, a więc zachowywała się podobnie do <code>std::async()</code>
* <!-- .element: class="fragment fade-in" --> Zadanie 2. Dodaj dodatkowy parametr - <code>std::launch policy</code>, który określi czy funkcja ma zostać wykonana od razu, czy dopiero na żądanie
* <!-- .element: class="fragment fade-in" --> Wzoruj się na: <a href="https://en.cppreference.com/w/cpp/thread/async"> cppreference-async</a>

</div><!-- .element: class="fragment fade-in" -->

<div>