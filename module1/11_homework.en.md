<!-- .slide: data-background="#111111" -->

# Homework

___

## Task 1

* <!-- .element: class="fragment fade-in" --> Implement a program where 4 threads will display theirs <code>std::this_thread::id()</code> a certain number of times

___
<!-- .slide: style="font-size: 0.8em" -->

## Task 1 - solution

```cpp
#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

template <typename TIME>
void daemon(int number, TIME time) {
    for (int i = 0 ; i < number; ++i) {
        std::cout << "Hi I'm thread with id: " << std::this_thread::get_id() << " Number: " << number << std::endl;
        std::this_thread::sleep_for(time);
    }
}

int main() {
    std::thread t1(daemon<decltype(1s)>, 20, 1s);
    std::thread t2(daemon<decltype(1500ms)>, 15, 1500ms);
    std::thread t3(daemon<decltype(2700ms)>, 10, 2700ms);

    t1.detach();
    t2.detach();
    t3.detach();

    daemon(4, 7s);
    return 0;
}
```
<!-- .element: class="fragment fade-in" -->

___

## Exercise 2

* Implement the <code>std::accumulate</code> algorithm using multithreading

```cpp
template< class InputIt, class T>
T accumulate( InputIt first, InputIt last, T init);
```

```cpp
template<class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init)
{
    for (; first != last; ++first) {
        init = std::move(init) + *first; // std::move since C++20
    }
    return init;
}
```

___

## Tip 1

* <!-- .element: class="fragment fade-in" --> Calculation of the number of threads required
* <!-- .element: class="fragment fade-in" --> Creating a container for storing the results

```cpp
const size_t hardwareThread = std::thread::hardware_concurrency();
const size_t neededThreads = std::min(size / minimumSize, hardwareThread);
const size_t chunkSize = size / neededThreads;

std::cout << "NeededThreads: " << neededThreads << std::endl;
std::cout << "ChunkSize: " << chunkSize << std::endl;
std::vector<std::thread> threads(neededThreads - 1);
std::vector<T> results(neededThreads);
```
<!-- .element: class="fragment fade-in" -->

___

## Tip 2

* <!-- .element: class="fragment fade-in" --> Implementation of a functor for threads
  * <!-- .element: class="fragment fade-in" --> The functor accepts successive portions of data
  * <!-- .element: class="fragment fade-in" --> Compute and pass iterators to the beginning and end of the range on which the thread is to operate.
  * <!-- .element: class="fragment fade-in" --> The variable holding the result should also be passed by reference.

```cpp
std::thread([](IT first, IT last, T& result)
            {
                result = std::accumulate(first, last, T{});
            }, begin, end, std::ref(results[i]));
```
<!-- .element: class="fragment fade-in" -->

___

## Tip 3

* <!-- .element: class="fragment fade-in" --> You should also use the current thread on which the function is called, so that it does not wait idly for the other threads to be called.

```cpp
auto begin = first;
for (size_t i = 0; i < neededThreads - 1; ++i) {
    auto end = std::next(begin, chunkSize);
    threads[i] = std::thread([](IT first, IT last, T& result)
            {
                result = std::accumulate(first, last, T{});
            }, begin, end, std::ref(results[i]));
    begin = end;
}
results[neededThreads - 1] = std::accumulate(begin, last, T{});
```
<!-- .element: class="fragment fade-in" -->

___

## Was the task difficult?

* <!-- .element: class="fragment fade-in" --> What was the biggest problem?
* <!-- .element: class="fragment fade-in" --> Can you simplify the algorithm?
* <!-- .element: class="fragment fade-in" --> Can other mechanisms to facilitate implementation be used?

___

## Homework

* Implement the <code>count_if</code> algorithm using multithreading

___

## Useful links

* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/thread/thread><code>std::thread</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/utility/functional/ref><code>std::ref</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp-concurrency-and-parallelism>C ++ Core Guidelines on Concurrency and Parallelism</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/>Top 20 C ++ multithreading mistakes and how to avoid them</a>
