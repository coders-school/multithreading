<!-- .slide: data-background="#111111" -->

# Singletony wątkowo-bezpieczne

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .85em" -->

## Przykład: thread-safe Singleton

```cpp []
class Singleton {
    static std::unique_ptr<Singleton> instance_;
    Singleton() = default;
public:
    static Singleton& getInstance() {
        std::mutex mutex_;
        std::unique_lock<std::mutex> lock(mutex_);
        if (!instance_) {
            instance_.reset(new Singleton{});
        }
        lock.unlock();
        return *instance_;
    }
};
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Wolne (mutexy)
* <!-- .element: class="fragment fade-in" --> Bezpieczne
* <!-- .element: class="fragment fade-in" --> Długi kod

___

## Przykład: thread-safe Singleton

```cpp []
class Singleton {
    static std::unique_ptr<Singleton> instance_;
    static std::once_flag flag_;
    Singleton() = default;
public:
    static Singleton& getInstance() {
        std::call_once(flag_, [&] {
            instance_.reset(new Singleton{});
        });
        return *instance_;
    }
};
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Wolne (once_flag)
* <!-- .element: class="fragment fade-in" --> Bezpieczne
* <!-- .element: class="fragment fade-in" --> Trochę mniej kodu

___

## Przykład: thread-safe Singleton

```cpp []
class Singleton {
    Singleton() = default;

public:
    static Singleton& getInstance() {
        static Singleton instance_;
        return instance_;
    }
};

// Meyers Singleton
```
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Najszybsze
* <!-- .element: class="fragment fade-in" --> Bezpieczne – statyczna inicjalizacja jest thread-safe od C++11
* <!-- .element: class="fragment fade-in" --> Krótkie
* <!-- .element: class="fragment fade-in" --> Śliczne
