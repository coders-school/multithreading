<!-- .slide: data-background="#111111" -->

# Lock managers

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Lock managers

<div style="display: flex;">

<div>

* <!-- .element: class="fragment fade-in" --> <code>lock_guard&lt;Mutex></code>
  * <!-- .element: class="fragment fade-in" --> the simplest one, primary choice
  * <!-- .element: class="fragment fade-in" --> constructor calls <code>lock()</code> on the mutex
  * <!-- .element: class="fragment fade-in" --> the destructor calls <code>unlock()</code>
  * <!-- .element: class="fragment fade-in" --> non-copyable
* <!-- .element: class="fragment fade-in" --> <code>unique_lock&lt;Mutex></code>
  * <!-- .element: class="fragment fade-in" --> delayed blocking
  * <!-- .element: class="fragment fade-in" --> time-limited locking attempts
  * <!-- .element: class="fragment fade-in" --> recursive locking
  * <!-- .element: class="fragment fade-in" --> making non-blocking attempts to obtain a lock - <code>try_lock()</code>
  * <!-- .element: class="fragment fade-in" --> cooperation with <code>timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> cooperation with condition variables
  * <!-- .element: class="fragment fade-in" --> non-copyable
  * <!-- .element: class="fragment fade-in" --> movable

</div>

<div>

* <!-- .element: class="fragment fade-in" --> <code>shared_lock&lt;SharedMutex></code>
  * <!-- .element: class="fragment fade-in" --> shared lock manager for reading variables
  * <!-- .element: class="fragment fade-in" --> several threads can share a lock on <code>SharedMutex</code>
  * <!-- .element: class="fragment fade-in" --> another thread may acquire an exclusive lock on <code>SharedMutex</code> through the <code>unique_lock</code>
  * <!-- .element: class="fragment fade-in" --> the same properties as <code>unique_lock</code>
* <!-- .element: class="fragment fade-in" --> <code>scoped_lock&lt;Mutexes...></code>
  * <!-- .element: class="fragment fade-in" --> locks multiple mutexes
  * <!-- .element: class="fragment fade-in" --> prevents deadlock
  * <!-- .element: class="fragment fade-in" --> the constructor locks all mutexes in a safe order
  * <!-- .element: class="fragment fade-in" --> the destructor unlocks them in reversed order
  * <!-- .element: class="fragment fade-in" --> non-copyable

</div>

___

### Task 3: securing the program with appropriate locks

<div style="display: flex;">

<div style="font-size: 0.8em; width: 100%">

```c++
#include <vector>
#include <iostream>
#include <thread>
using namespace std;
vector<int> numbers = {};

int getNextValue() {
    static int i = 0;
    return i+=10;
}

void read(int index) {
    for(int i = 0; i < 10; i++) {
        int value = numbers[index];
        cout << value << " ";
    }
}

void write() {
    for(int i = 0; i < 3; i++) {
        int newValue = getNextValue();
        numbers.emplace_back(newValue);
    }
}

int main() {
    vector<thread> writers;
    for(int i = 0; i < 10; i++)
        writers.emplace_back(write);

    cout << "Writers produced: ";
    for(const auto & n : numbers)
        cout << n << " ";
    cout << endl;

    cout << "Readers consumed: ";
    vector<thread> readers;
    for(int i = 0; i < 10; i++)
        readers.emplace_back(read, i);

    for(auto && writer : writers)
        writer.join();
    for(auto && reader : readers)
        reader.join();

    cout << endl;
    return 0;
}
```

</div>

<div class="fragment fade-in" style="font-size: .8em; background-color: #8B3536; padding: 20px 10px; margin: 22px 0;">

* <!-- .element: class="fragment fade-in" --> Use shared and / or regular locks
* <!-- .element: class="fragment fade-in" --> Use the appropriate lock managers
* <!-- .element: class="fragment fade-in" --> Compile with TSan

```bash
$> g++ 03_shared_mutex.cpp -lpthread \
   -std=c++17 -fsanitize=thread
```
<!-- .element: class="fragment fade-in" -->

#### Tips
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Shared locks are used when reading data
  * <!-- .element: class="fragment fade-in" --> <code>shared_lock<shared_mutex></code>
* <!-- .element: class="fragment fade-in" --> You must obtain an exclusive lock to write data
  * <!-- .element: class="fragment fade-in" --> <code>lock_guard<shared_mutex></code>
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock<shared_mutex></code>

</div>

</div>

___

### Task 3: proposed solution

* <!-- .element: class="fragment fade-in" --> What problems can this code encounter?
* <!-- .element: class="fragment fade-in" --> How can this solution be improved?

<div class="fragment fade-in" style="display: flex;">

<div style="font-size: .7em">

```c++
// #includes ...
using namespace std;
vector<int> numbers = {};
shared_mutex numbersMtx;
mutex coutMtx;

int getNextValue() {
    static int i = 0;
    return i+=10;
}
void read(int index) {
    for(int i = 0; i < 10; i++) {
        shared_lock<shared_mutex> lock{numbersMtx};
        int value = numbers[index];
        lock.unlock();
        lock_guard<mutex> coutLock(coutMtx);
        cout << value << " ";
    }
}
void write() {
    for(int i = 0; i < 3; i++) {
        lock_guard<shared_mutex> lock(numbersMtx);
        int newValue = getNextValue();
        numbers.emplace_back(newValue);
    }
}
```
<!-- .element: style="width: 100%;" -->
</div>

<div style="font-size: .7em">

```c++
int main() {
    vector<thread> writers;
    for(int i = 0; i < 10; i++)
        writers.emplace_back(write);

    cout << "Writers produced: ";
    for(const auto & n : numbers)
        cout << n << " ";
    cout << endl;

    cout << "Readers consumed: ";
    vector<thread> readers;
    for(int i = 0; i < 10; i++)
        readers.emplace_back(read, i);

    for(auto && writer : writers)
        writer.join();
    for(auto && reader : readers)
        reader.join();

    cout << endl;
    return 0;
}
```
<!-- .element: style="width: 100%; margin-left: 100px;" -->
</div>

</div>

___

### Task 3: problems in the proposed solution

* <!-- .element: class="fragment fade-in" --> Is vector <code>numbers</code> always filled in before readers read it?
  * <!-- .element: class="fragment fade-in" --> First, we create writers
  * <!-- .element: class="fragment fade-in" --> <code>join()</code> is called on writers first
  * <!-- .element: class="fragment fade-in" --> But that doesn't guarantee anything
  * <!-- .element: class="fragment fade-in" --> Use of <code>at()</code> will throw an exception. How do we want to handle it?
  * <!-- .element: class="fragment fade-in" --> You can reserve memory in the vector in advance, but what is the default behavior?
  * <!-- .element: class="fragment fade-in" --> There is no unambiguous answer, you have to accept some convention or ...
  * <!-- .element: class="fragment fade-in" --> You can use a synchronized queue (covered in another training)
* <!-- .element: class="fragment fade-in" --> No need to lock <code>cout</code> if there is a single << operation
  * <!-- .element: class="fragment fade-in" --> With the proper design, we do not have to use locks at all (so-called lock-free programming), but it is absolute expert knowledge
  * <!-- .element: class="fragment fade-in" --> We gain in efficiency, but we lose in program flexibility
  * <!-- .element: class="fragment fade-in" --> Any change of the lock-free code must be preceded by a reliable code-review of many experts
