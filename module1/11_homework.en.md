<!-- .slide: style="font-size: 0.8em" -->

## Template functions in threads

```cpp
#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

template <typename Time>
void daemon(int number, Time time) {
    for (int i = 0 ; i < number; ++i) {
        std::cout << "Hi I'm thread with id: " << std::this_thread::get_id()
                  << " Number: " << number << '\n';
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
<!-- .slide: data-background="#111111" -->

# Summary

___

## What is concurrency?

* <!-- .element: class="fragment fade-in" --> Two processes are concurrent if one of them starts before the end of the other

<img class="plain fragment fade-in" data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> Concurrency is the process of performing two or more activities simultaneously

___

## When to use concurrency?

* <!-- .element: class="fragment fade-in" --> Distribution of tasks
* <!-- .element: class="fragment fade-in" --> Performance:
  * <!-- .element: class="fragment fade-in" --> Parallel tasks (division of a task into parts),
  * <!-- .element: class="fragment fade-in" --> Data parallelization (performing the same tasks on different pieces of data)

___

## When not to use concurrency?

* <!-- .element: class="fragment fade-in" --> When too many threads can reduce performance instead of increasing it (cost of starting a thread, cost of resources - each thread typically uses 8MB of memory),
* <!-- .element: class="fragment fade-in" --> When the performance gain is not proportional to the effort involved and the complexity of the code (code maintenance cost is equally important).

___
<!-- .slide: data-background="#111111" -->

## Exercise 6 / Homework

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

* <!-- .element: class="fragment fade-in" --> Calculate the number of possible threads
* <!-- .element: class="fragment fade-in" --> Create a container for storing the results

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
  * <!-- .element: class="fragment fade-in" --> The variable holding the result should also be passed by reference

```cpp
std::thread([](IT first, IT last, T& result) {
                result = std::accumulate(first, last, T{});
            },
            begin,
            end,
            std::ref(results[i]));
```
<!-- .element: class="fragment fade-in" -->

___

## Tip 3

* <!-- .element: class="fragment fade-in" --> You should also use the current thread on which the function is called, so that it does not wait idly for the other threads to finish.

```cpp
auto begin = first;
for (size_t i = 0; i < neededThreads - 1; ++i) {
    auto end = std::next(begin, chunkSize);
    threads[i] = std::thread([](IT first, IT last, T& result) {
                                 result = std::accumulate(first, last, T{});
                             },
                             begin,
                             end,
                             std::ref(results[i]));
    begin = end;
}
results[neededThreads - 1] = std::accumulate(begin, last, T{});
```
<!-- .element: class="fragment fade-in" -->

___

## Was the task difficult?

* <!-- .element: class="fragment fade-in" --> What was the biggest problem?
* <!-- .element: class="fragment fade-in" --> Can you simplify the algorithm?
* <!-- .element: class="fragment fade-in" --> Can other mechanisms be used to facilitate implementation?

___

## Homework

Implement the <code>count_if</code> algorithm using multithreading

___

## Useful links

* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/thread/thread><code>std::thread</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://en.cppreference.com/w/cpp/utility/functional/ref><code>std::ref</code> (cppreference.com)</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cp-concurrency-and-parallelism>C ++ Core Guidelines on Concurrency and Parallelism</a>
* <!-- .element: class="fragment fade-in" --> <a href=https://www.acodersjourney.com/top-20-cplusplus-multithreading-mistakes/>Top 20 C ++ multithreading mistakes and how to avoid them</a>
