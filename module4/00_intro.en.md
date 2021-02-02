## Something about you

* One of the most interesting things from pre-work?
* Have you ever used condition variable?

___
<!-- .slide: data-background="img/lukasz.jpg" -->

<h2 style = "text-shadow: 2px 2px black;"> Łukasz Ziobroń </h2>
<div class="box fragment" style="width: 45%; left: 0; top: 100px;">

### Experience not only in programming

* Trainer and DevOps @ Coders School
* C ++ and Python developer @ Nokia and Credit Suisse
* Team leader and trainer @ Nokia
* Scrum Master @ Nokia & Credit Suisse
* Code Reviewer @ Nokia
* Web developer (HTML, PHP, CSS) @ StarCraft Area

</div>

<div class="box fragment" style="width: 45%; right: 0; top: 100px;">

### Experience as a trainer

* C ++ courses @ Coders School
* Practical Aspects of Software Engineering @ PWr & UWr
* Nokia Academy
* Internal corporate training

</div>

<div class="box fragment" style="width: 45%; left: 0; top: 400px;">

### Experience in public speaking

* code::dive conference
* code::dive community
* Polish Academic Championship in Team Programming

</div>

<div class="box fragment" style="width: 45%; right: 0; top: 400px;">

### Interests

* StarCraft Brood War & StarCraft II
* Motorcycles
* Photography
* Archery
* Marketing
* Andragogy

</div>

___

## Contract

* <!-- .element: class="fragment fade-in" --> 🎰 The Vegas rule
* <!-- .element: class="fragment fade-in" --> 🗣 Discussion, not lecture
* <!-- .element: class="fragment fade-in" --> ☕️ Additional breaks on demand
* <!-- .element: class="fragment fade-in" --> ⌚️ Punctuality

___
<!-- .slide: style="font-size: .65em" -->

## Pre-test 🤯

<div class="multicolumn">

<div style="width: 60%">

```cpp
// assume all necessary includes are here

int main() {
    std::mutex m;
    std::condition_variable cv;
    std::vector<int> v;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    auto consume = [&] {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul);
        std::cout << v.back();
        v.pop_back();
    };
    for (int i = 0; i < 10; i++) consumers.emplace_back(consume);

    auto produce = [&](int i) {
        {
            std::lock_guard<std::mutex> lg(m);
            v.push_back(i);
        }
        cv.notify_all();
    };
    for (int i = 0; i < 10; i++) producers.emplace_back(produce, i);

    for (auto && p : producers) p.join();
    for (auto && c : consumers) c.join();
}
```

</div>

<div class="col" style="margin-top: 20px">

1. there may be an Undefined Behavior in this code
2. the output is guaranteed to always be <code>0123456789</code>
3. <code>v</code> is always an empty vector at the end of this program
4. if some producers threads started before some consumers, we would have a deadlock because of lost notifications
5. a change from <code>notify_all()</code> to <code>notify_one()</code> guarantees that each consumer thread will receive a different number
6. this code can be improved by providing a predicate to <code>wait()</code> to disallow getting elements when the vector is empty

Note: 1, 4, 5, 6

</div>

</div>
