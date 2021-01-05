<!-- .slide: data-background="#111111" -->

# Multithreading

## `std::promise`/`std::future` in C++

### Znajdź niepoprawny fragment kodu

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Znajdź niepoprawny fragment kodu

<div class="fragment fade-in" style="display: flex; transform: scale(0.8); margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#2 promise.set_value(10);
#1 future.get();

```

</div>

</div>

<div class="fragment fade-in" style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#2 promise.set_value(10);
#1 auto future = promise.get_future();
#1 future.get();

```

</div>

</div>

<div class="fragment fade-in" style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 promise.set_value(10);

```

</div>

</div>

<div class="fragment fade-in" style="display: flex; transform: scale(0.8); margin-top: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 // set wasn't called

```

</div>

</div>

___

### Znajdź niepoprawny fragment kodu

<div style="display: flex; transform: scale(0.8); margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#2 promise.set_value(10);
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#2 promise.set_value(10);
#1 auto future = promise.get_future();
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 promise.set_value(10);

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 // set wasn't called

```

</div>

</div>

___

### Znajdź niepoprawny fragment kodu

<div style="display: flex; transform: scale(0.8); margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#2 promise.set_value(10);
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#2 promise.set_value(10);
#1 auto future = promise.get_future();
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 promise.set_value(10);

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 // set wasn't called

```

</div>

</div>

___

### Znajdź niepoprawny fragment kodu

<div style="display: flex; transform: scale(0.8); margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#2 promise.set_value(10);
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#2 promise.set_value(10);
#1 auto future = promise.get_future();
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 promise.set_value(10);

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px;">

<div style="width: 13%;">

<img data-src="img/qmark.gif" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 // set wasn't called

```

</div>

</div>

___

### Znajdź niepoprawny fragment kodu

<div style="display: flex; transform: scale(0.8); margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="question mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#2 promise.set_value(10);
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#2 promise.set_value(10);
#1 auto future = promise.get_future();
#1 future.get();

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px; margin-bottom: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 promise.set_value(10);

```

</div>

</div>

<div style="display: flex; transform: scale(0.8); margin-top: -50px;">

<div style="width: 13%;">

<img data-src="img/ok-icon-9.png" alt="ok mark" class="plain">

</div>

<div style="width: 87%;">

```c++

#1 auto future = promise.get_future();
#1 future.get();
#2 // set wasn't called

```

</div>

</div>
