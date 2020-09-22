<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

### Thread sanitizer

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Thread sanitizer

```bash
$> g++ 01_threads_write.cpp -lpthread -fsanitize=thread -O2 -g
$> ./a.out
```
<img src="img/tsan.png" class="plain" style="transform: scale(1.125);">