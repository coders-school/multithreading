<!-- .slide: data-background="#111111" -->

# Thread-safe singletons

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___
<!-- .slide: style="font-size: .85em" data-auto-animate -->

## Example: thread-safe singleton

```cpp []
class Singleton {
    static std::unique_ptr<Singleton> instance_;
    static std::mutex mutex_;
    Singleton() = default;
public:
    static Singleton& getInstance() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!instance_) {
                instance_.reset(new Singleton{});
            }
        }
        return *instance_;
    }
};
```
<!-- .element: class="fragment fade-in" data-id="singleton" -->

* <!-- .element: class="fragment fade-in" --> Slow (mutexes)
* <!-- .element: class="fragment fade-in" --> Safe
* <!-- .element: class="fragment fade-in" --> Long Code

___
<!-- .slide: style="font-size: .85em" data-auto-animate -->

## Example: thread-safe singleton

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
<!-- .element: data-id="singleton" -->

* <!-- .element: class="fragment fade-in" --> Slow (once_flag)
* <!-- .element: class="fragment fade-in" --> Safe
* <!-- .element: class="fragment fade-in" --> Less code

___
<!-- .slide: style="font-size: .85em" data-auto-animate -->

## Example: thread-safe singleton

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
<!-- .element: data-id="singleton" -->

* <!-- .element: class="fragment fade-in" --> The fastest one
* <!-- .element: class="fragment fade-in" --> Safe - static initialization is thread-safe since C++11
* <!-- .element: class="fragment fade-in" --> Short
* <!-- .element: class="fragment fade-in" --> Lovely
