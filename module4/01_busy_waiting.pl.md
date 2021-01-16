<!-- .slide: data-background="#111111" -->

# Multithreading

## Moduł 4

### Aktywne czekanie

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie 1: kolejka FIFO

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
template <typename T>
class ThreadsafeQueue {
    deque<T> queue_;
    // TODO: Make it thread-safe :)

public:
    void push(const T & element) {
        queue_.push_front(element);
    }
    T pop() {
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        return queue_.empty();
    }
};

```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> W pliku <code>01_threadsafe_queue.cpp</code> znajduje się wybrakowana implementacja kolejki FIFO
* <!-- .element: class="fragment fade-in" --> Napraw wątek <code>textProducer</code>, aby generował prawidłowe napisy:
  * <!-- .element: class="fragment fade-in" --> This is random text number 0
  * <!-- .element: class="fragment fade-in" --> This is random text number 1
  * <!-- .element: class="fragment fade-in" --> …
  * <!-- .element: class="fragment fade-in" --> This is random text number n
* <!-- .element: class="fragment fade-in" --> Zabezpiecz operacje na kolejce przed dostępem z wielu wątków (make it thread-safe 🙂)
* <!-- .element: class="fragment fade-in" --> Jaki problem widzisz?

</div> <!-- .element: class="fragment fade-in" -->

</div>

___

#### Zadanie 1 – rozwiązanie: naprawione wyświetlanie tekstu

```c++
void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number "
                + i);
}

void produceText(StringQueue & sq, int number) {
    for (int i = 0; i < number; i++)
        sq.push("This is random text number " +
                to_string(i));
}

```

___
<!-- .slide: style="font-size: .9em" -->

### Zadanie 1 – rozwiązanie: threadsafe queue

```c++
template <typename T>
class ThreadsafeQueue {
    deque<T> queue_;
    mutable mutex m_;
    using Lock = lock_guard<mutex>;

public:
    void push(const T & element) {
        Lock l(m_);
        queue_.push_front(element);
    }
    T pop() {
        Lock l(m_);
        auto top = queue_.back();
        queue_.pop_back();
        return top;
    }
    bool empty() const {
        Lock l(m_);
        return queue_.empty();
    }
};

```
<!-- .element: class="fragment fade-in" -->

___

### Problem – grzanie CPU i efekt cieplarniany

<img data-src="img/efekt_cieplarniany.jpg" alt="efekt cieplarniany" class="plain">

___

### Aktywne czekanie

<div style="display: flex;">

<div style="width: 60%; font-size: .9em;">

```c++
void saveToFile(StringQueue & sq) {
    ofstream file("/tmp/sth.txt");
    while (file) {
        while (sq.empty()) { /* nop */ }
        file << sq.pop() << endl;
    }
}
```
<!-- .element: class="fragment fade-in" -->
</div>

<div style="width: 40%; background-color: #8B3536; padding: 5px 10px; font-size: .9em;">

* <!-- .element: class="fragment fade-in" --> Aktywne czekanie (busy waiting) to stan, w którym wątek ciągle sprawdza, czy został spełniony pewien warunek
* <!-- .element: class="fragment fade-in" --> Inna nazwa tego problemu to wirująca blokada (spinlock)
* <!-- .element: class="fragment fade-in" --> Problem rozwiązuje zmienna warunku (condition variable)

</div> <!-- .element: class="fragment fade-in" -->

</div>

<img data-src="img/aktywne_czekanie.png" alt="aktywne_czekanie" class="plain">
<!-- .element: class="fragment fade-in" -->
