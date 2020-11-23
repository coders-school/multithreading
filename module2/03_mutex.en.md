<!-- .slide: data-background="#111111" -->

# Mutex

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Mutex - avoiding races

* <!-- .element: class="fragment fade-in" --> Mutex (Mutual Exclusion) - mutual exclusion
* <!-- .element: class="fragment fade-in" --> Lock implementation
* <!-- .element: class="fragment fade-in" --> <code>#include &lt;mutex&gt;</code>
* <!-- .element: class="fragment fade-in" --> <code>std::mutex</code>
* <!-- .element: class="fragment fade-in" --> The most important operations:
  * <!-- .element: class="fragment fade-in" --> <code>void lock()</code> - mutex blocking. Blocking operation. If the mutex is locked by another thread, we wait to be unlocked.
  * <!-- .element: class="fragment fade-in" --> <code>void unlock()</code> - Mutex unlocking.
  * <!-- .element: class="fragment fade-in" --> <code>bool try_lock()</code> - mutex blocking. Non-blocking operation. If the mutex is already locked by another thread, it continues with further thread execution. Returns true if it succeeds in locking the mutex

___

### Task 2: write protection to stream

<div style="display: flex;">

<div style="font-size: .8em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    cout << "Thread [" << id << "]: "
         << "Job done!" << endl;
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 99%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Secure your code so that each thread can safely enter its full text into the stream.
* <!-- .element: class="fragment fade-in" --> Cases like the following should not be possible

```bash
$> g++ 02_threads_in_collection.cpp -lpthread
$> ./a.out
...
Thread [10]: Job done!
Thread [9]: Thread [Job done!11]: Job done!
Thread [6]: Job done!
...
$> ./a.out
...
Thread [Thread [5]: Job done!
Thread [13]: Job done!
17]: Job done!
...
```
<!-- .element: class="fragment fade-in" style="width: 100%;" -->

</div>

</div>

___

### Task 2 - solution using a mutex

<div style="display: flex;">

<div style="font-size: .8em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

using namespace std;

mutex m;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    m.lock();
    cout << "Thread [" << id << "]: "
         << "Job done!" << endl;
    m.unlock();
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Works ... in this case.
* <!-- .element: class="fragment fade-in" --> The code blocked with mutex is called critical section.
* <!-- .element: class="fragment fade-in" --> The duration of the blockage must be as short as possible
  * <!-- .element: class="fragment fade-in" --> the error is blocking the function <code>sleep_for()</code>.
* <!-- .element: class="fragment fade-in" --> Global mutex
  * <!-- .element: class="fragment fade-in" --> usually mutexes are placed in classes whose operations must be blocked.
* <!-- .element: class="fragment fade-in" --> What if an exception occurs during the lockout period?
  * <!-- .element: class="fragment fade-in" --> will not be called <code>unlock()</code>
  * <!-- .element: class="fragment fade-in" --> other threads will never finish
  * <!-- .element: class="fragment fade-in" --> no RAII.
* <!-- .element: class="fragment fade-in" --> Can it be better?

</div>

</div>

___

### Task 2 - solution with the use of `lock_guard`

<div style="display: flex;">

<div style="font-size: .75em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

using namespace std;

void do_work(ind id, mutex & m) {
    this_thread::sleep_for(100ms);
    lock_guard<mutex> lock(m);
    cout << "Thread [" << id << "]: "
         << "Job done!" << endl;
}

int main() {
    mutex m;
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i ,ref(m)));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> It works.
* <!-- .element: class="fragment fade-in" --> Safe in case of an exception - it is RAII.
* <!-- .element: class="fragment fade-in" --> Mutex passed by reference.
* <!-- .element: class="fragment fade-in" --> Formatting and composing text while a lock is in progress - streams are not famous for their performance.
* <!-- .element: class="fragment fade-in" --> Can it be faster?

</div>

</div>

___

### Task 2 - solution with the use of `stringstream`

<div style="display: flex;">

<div style="font-size: .75em;">

```c++
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>

using namespace std;

void do_work(ind id) {
    this_thread::sleep_for(100ms);
    stringstream ss;
    ss << "Thread [" << id << "]: "
       << "Job done!" << endl;
    cout << ss.rdbuf();
}

int main() {
    vector<thread> threads;
    for (int i = 0; i < 20; i++) {
        threads.emplace_back(thread(do_work, i));
    }
    for (auto && t : threads) {
        t.join();
    }
    return 0;
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> It works.
* <!-- .element: class="fragment fade-in" --> Format text outside the output stream.
* <!-- .element: class="fragment fade-in" --> Mutex is unnecessary
* <!-- .element: class="fragment fade-in" --> A single write to a stream is atomic.

</div>

</div>

___

## Critical section

```c++
class SafeList {
    mutex m;
public:
    void remove_safely(Node* node) {
        lock_guard<mutex> lock(m);
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    } // automatic unlocking
    // other methods
};
```

* <!-- .element: class="fragment fade-in" --> A critical section is a piece of a program that can only be executed by 1 thread at a time
* <!-- .element: class="fragment fade-in" --> It is usually implemented using a mutex as an access block
* <!-- .element: class="fragment fade-in" --> Always use a lock manager (e.g. <code>lock_guard<mutex></code>) to provide the RAII mechanism
* <!-- .element: class="fragment fade-in" --> Usually, the entire critical section is separated into a separate function

___
<!-- .slide: style="font-size: .85em" -->
## Types of mutexes

* <!-- .element: class="fragment fade-in" --> <code>mutex</code>
  * <!-- .element: class="fragment fade-in" --> <code>void lock()</code> - the current thread is suspended until a lock is acquired
  * <!-- .element: class="fragment fade-in" --> <code>void unlock()</code> - if the current thread is the holder of the lock, it is released
  * <!-- .element: class="fragment fade-in" --> <code>bool try_lock()</code> - attempt to obtain a lock without pausing the current thread. Returns true if the lock has been acquired, false otherwise
* <!-- .element: class="fragment fade-in" --> <code>timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> has methods to define the maximum waiting time for a thread to acquire a lock
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_until(timeout_time)</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_for(timeout_duration)</code>
* <!-- .element: class="fragment fade-in" --> <code>recursive_mutex</code>
  * <!-- .element: class="fragment fade-in" --> the same thread can obtain a mutex multiple times by calling a method <code>lock()</code> or <code>try_lock()</code>
  * <!-- .element: class="fragment fade-in" --> to release the mutex the thread has to call it a number of times <code>unlock()</code>
* <!-- .element: class="fragment fade-in" --> <code>recursive_timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> owns property <code>timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> owns property <code>recursive_mutex</code>
* <!-- .element: class="fragment fade-in" --> <code>shared_mutex</code>
  * <!-- .element: class="fragment fade-in" --> the possibility of obtaining shared locks using the methods:
    * <!-- .element: class="fragment fade-in" --> <code>void lock_shared()</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared()</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared_for(rel_time)</code>
    * <!-- .element: class="fragment fade-in" --> <code>bool try_lock_shared_until(abs_time)</code>
    * <!-- .element: class="fragment fade-in" --> <code>void unlock_shared()</code>

___

## Shared lock - `shared_mutex`

<div style="display: flex;">

<div style="font-size: .8em; width: 99%">

```c++
#include <deque>
#include <shared_mutex>
std::deque<int> ids;
std::shared_mutex mtxIds;
int getIdsIndex() { /* ... */ }
void process(int) { /* ... */ }
int newValue() { /* ... */ }
void reader() {
    int index = getIdsIndex();
    std::shared_lock<std::shared_mutex> lock(mtxIds);
    int value = ids[index];
    lock.unlock();
    process(value);
}
void writer() {
    int index = getIdsIndex();
    std::lock_guard<std::shared_mutex> lock(mtxIds);
    ids[index] = newValue();
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Shared locks are used when reading data
  * <!-- .element: class="fragment fade-in" --> <code>shared_lock<shared_mutex></code>
* <!-- .element: class="fragment fade-in" --> You must obtain an exclusive lock to write data
  * <!-- .element: class="fragment fade-in" --> <code>lock_guard<shared_mutex></code>
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock<shared_mutex></code>

</div>

</div>
