<!-- .slide: data-background="#111111" -->

# Multithreading

## Współdzielenie danych

### Zadanie 3: zabezpieczenie programu za pomocą odpowiednich blokad

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

### Zadanie 3: zabezpieczenie programu za pomocą odpowiednich blokad

<div style="display: flex; margin-top: -65px;">

<div>

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
```
<!-- .element: style="transform: scale(.85) translateX(-25%); width: 100%;" -->
</div>

<div>

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
<!-- .element: style="transform: scale(.85) translateX(-40%); width: 100%;" -->
</div>

<div style="margin-top: 65px; transform: translateX(-80%);">

* <!-- .element: class="fragment fade-in" --> Użyj blokad współdzielonych i/lub zwykłych
* <!-- .element: class="fragment fade-in" --> Użyj odpowiednich menadżerów blokad
* <!-- .element: class="fragment fade-in" --> Skompiluj w C++17 i z Tsanem

</div>

</div>

```bash
$> g++ 03_shared_mutex.cpp -lpthread -std=c++17 -fsanitize=thread
```
<!-- .element: style="margin-top: -50px;" class="fragment fade-in" -->

___

### Zadanie 3: zabezpieczenie programu za pomocą odpowiednich blokad - wskazówki

* <!-- .element: class="fragment fade-in" --> Współdzielone blokady używane są w trakcie czytania danych
    * <!-- .element: class="fragment fade-in" --> <code>shared_lock<shared_mutex></code>
* <!-- .element: class="fragment fade-in" --> Do zapisu danych trzeba pozyskać wyłączną blokadę
    * <!-- .element: class="fragment fade-in" --> <code>lock_guard<shared_mutex></code>
    * <!-- .element: class="fragment fade-in" --> <code>unique_lock<shared_mutex></code>

___

### Zadanie 3: rozwiązanie

<div style="display: flex;">

<div style="font-size: .76em">

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
        shared_lock<shared_mutex> lock(numbersMtx);
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

<div style="font-size: .85em">

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
