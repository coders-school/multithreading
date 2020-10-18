<!-- .slide: data-background="#111111" -->

# `join()` or `detach()`?

___
<!-- .slide: style="font-size: 0.8em" -->

## `join()` or `detach()`?

<div class="multicolumn">
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        this_thread::sleep_for(1s);
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    return 0;
}
```

```output
$> g++ 04a_join.cpp –lpthread
$> ./a.out
Doing something in casualJob
Thread job done
```

</div>
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        this_thread::sleep_for(1s);
        cout << "Thread job done" << endl;
    });
    t.detach();
    casualJob();
    return 0;
}
```

```output
$> g++ 04b_detach.cpp –lpthread
$> ./a.out
Doing something in casualJob
```

</div>
</div>

___
<!-- .slide: style="font-size: 0.8em" -->

## `join()` or `detach()`?

<div class="multicolumn">
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    this_thread::sleep_for(1s);
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    return 0;
}
```

```output
$> g++ 04c_join.cpp –lpthread
$> ./a.out
Thread job done
Doing something in casualJob
```

</div>
<div class="col">

```cpp
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

void casualJob() {
    this_thread::sleep_for(1s);
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    t.detach();
    casualJob();
    return 0;
}
```

```output
$> g++ 04d_detach.cpp –lpthread
$> ./a.out
Thread job done
Doing something in casualJob
```

</div>
</div>

___
<!-- .slide: style="font-size: 0.8em" -->

## `join()` or `detach()`?

<div class="multicolumn">
<div class="col">

```cpp
#include <thread>
#include <iostream>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    // no join() or detach()
    casualJob();

    return 0;
}
```

```output
$> g++ 05_no_join_no_detach.cpp –lpthread
$> ./a.out
Thread job done
Doing something in casualJob
terminate called without an active exception
Aborted (core dumped)
```

</div>
<div class="col">

```cpp
#include <thread>
#include <iostream>
using namespace std;

void casualJob() {
    cout << "Doing something in casualJob" << endl;
}

int main() {
    thread t([] {
        cout << "Thread job done" << endl;
    });
    casualJob();
    t.join();
    t.detach();
    return 0;
}
```

```output
$> g++ 05_join_and_detach.cpp –lpthread
$> ./a.out
Doing something in casualJob
Thread job done
terminate called after throwing an instance of
'std::system_error'
    what():  Invalid argument
Aborted (core dumped)
```

</div>
</div>

___

## `join()` or `detach()`?

* <!-- .element: class="fragment fade-in" --> A thread must always be joined - <code>join()</code> or detached - <code>detach()</code>. Always.
* <!-- .element: class="fragment fade-in" --> Thread destructor does not join nor detach it (no RAII 😔)
* <!-- .element: class="fragment fade-in" --> Failure to join or detach a thread will result in a <code>std::terminate()</code> call which quits the application
* <!-- .element: class="fragment fade-in" --> Thread method <code>joinable()</code> returns <code>true</code> if you can do <code>join()</code>
* <!-- .element: class="fragment fade-in" --> <code>join()</code> can only be done once and is exclusive with <code>detach()</code>. Either one or the other should be used
* <!-- .element: class="fragment fade-in" --> When we detach a thread, we usually do so immediately after creating it. After detaching, we can no longer refer to the thread using its name
* <!-- .element: class="fragment fade-in" --> If we join a thread, we have to choose the right place to do it. <code>join()</code> is a blocking operation that waits for the thread to finish, so this is usually done at the end of the thread's starting function. If this function returns a thread, it can be joined even later.
