<!-- .slide: data-background="#111111" -->

# Menadżery blokad

<a href="https://coders.school">
    <img width="500" data-src="../coders_school_logo.png" alt="Coders School" class="plain">
</a>

___

## Menadżery blokad

<div style="display: flex;">

<div>

* <!-- .element: class="fragment fade-in" --> <code>lock_guard&lt;Mutex></code>
  * <!-- .element: class="fragment fade-in" --> najprostszy, główny wybór
  * <!-- .element: class="fragment fade-in" --> konstruktor wywołuje <code>lock()</code> na mutexie
  * <!-- .element: class="fragment fade-in" --> destruktor woła <code>unlock()</code>
  * <!-- .element: class="fragment fade-in" --> jest niekopiowalny
* <!-- .element: class="fragment fade-in" --> <code>unique_lock&lt;Mutex></code>
  * <!-- .element: class="fragment fade-in" --> opóźnione blokowanie
  * <!-- .element: class="fragment fade-in" --> próby zablokowania ograniczone czasowo
  * <!-- .element: class="fragment fade-in" --> blokowanie rekursywne
  * <!-- .element: class="fragment fade-in" --> podejmowanie nieblokujących prób pozyskania blokady <code>(try_lock)</code>
  * <!-- .element: class="fragment fade-in" --> korzystanie z <code>timed_mutex</code>
  * <!-- .element: class="fragment fade-in" --> korzystanie ze zmiennych warunkowych (condition variable)
  * <!-- .element: class="fragment fade-in" --> jest niekopiowalny
  * <!-- .element: class="fragment fade-in" --> jest przenaszalny (move)

</div>

<div>

* <!-- .element: class="fragment fade-in" --> <code>shared_lock&lt;SharedMutex></code>
  * <!-- .element: class="fragment fade-in" --> menadżer współdzielonych blokad do odczytu zmiennych
  * <!-- .element: class="fragment fade-in" --> kilka wątków może współdzielić blokadę <code>SharedMutex</code>
  * <!-- .element: class="fragment fade-in" --> inny wątek może pozyskać blokadę <code>SharedMutex</code> na wyłączność za pomocą menadżera <code>unique_lock</code>
  * <!-- .element: class="fragment fade-in" --> te same własności co <code>unique_lock</code>
* <!-- .element: class="fragment fade-in" --> <code>scoped_lock&lt;Mutexes...></code>
  * <!-- .element: class="fragment fade-in" --> blokuje kilka mutexów
  * <!-- .element: class="fragment fade-in" --> zapobiega zakleszczeniom (deadlock)
  * <!-- .element: class="fragment fade-in" --> konstruktor blokuje wszystkie mutexy w bezpiecznej kolejności, unikając blokad
  * <!-- .element: class="fragment fade-in" --> destruktor odblokowuje je w kolejności odwrotnej
  * <!-- .element: class="fragment fade-in" --> jest niekopiowalny

</div>

___

### Zadanie 3: zabezpieczenie programu za pomocą odpowiednich blokad

<div style="display: flex;">

<div style="font-size: 0.85em; width: 100%">

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

* <!-- .element: class="fragment fade-in" --> Użyj blokad współdzielonych i/lub zwykłych
* <!-- .element: class="fragment fade-in" --> Użyj odpowiednich menadżerów blokad
* <!-- .element: class="fragment fade-in" --> Skompiluj w C++17 i z Tsanem

```bash
$> g++ 03_shared_mutex.cpp -lpthread \
   -std=c++17 -fsanitize=thread
```
<!-- .element: class="fragment fade-in" -->

#### Wskazówki
<!-- .element: class="fragment fade-in" -->

* <!-- .element: class="fragment fade-in" --> Współdzielone blokady używane są w trakcie czytania danych
  * <!-- .element: class="fragment fade-in" --> <code>shared_lock<shared_mutex></code>
* <!-- .element: class="fragment fade-in" --> Do zapisu danych trzeba pozyskać wyłączną blokadę
  * <!-- .element: class="fragment fade-in" --> <code>lock_guard<shared_mutex></code>
  * <!-- .element: class="fragment fade-in" --> <code>unique_lock<shared_mutex></code>

</div>

</div>

___

### Zadanie 3: proponowane rozwiązanie

* <!-- .element: class="fragment fade-in" --> Jakie problemy mogą wystąpić w tym kodzie?
* <!-- .element: class="fragment fade-in" --> Jak można ulepszyć to rozwiązanie?

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

### Zadanie 3: problemy w zaproponowanym rozwiązaniu

* <!-- .element: class="fragment fade-in" --> Czy vector <code>numbers</code> zawsze będzie wypełniony zanim czytelnicy go odczytają?
  * <!-- .element: class="fragment fade-in" --> Najpierw tworzymy pisarzy
  * <!-- .element: class="fragment fade-in" --> <code>join()</code> jest najpierw wołany na pisarzach
  * <!-- .element: class="fragment fade-in" --> Ale to niczego nie gwarantuje
  * <!-- .element: class="fragment fade-in" --> Użycie <code>at()</code> spowoduje rzucenie wyjątku. Jak chcemy go obsługiwać?
  * <!-- .element: class="fragment fade-in" --> Można zarezerwować pamięć w wektorze wcześniej, ale jakie ma być domyślne zachowanie?
  * <!-- .element: class="fragment fade-in" --> Nie ma jednoznacznej odpowiedzi, trzeba przyjąć jakąś konwencję lub...
  * <!-- .element: class="fragment fade-in" --> Można użyć synchronizowanej kolejki (o tym na innej lekcji)
* <!-- .element: class="fragment fade-in" --> Nie potrzeba blokować <code>cout</code>, gdy zrobimy tam jedną operację <<
  * <!-- .element: class="fragment fade-in" --> Przy odpowiednim zaprojektowaniu programu nie musimy w ogóle stosować blokad (lock-free programming), ale jest to absolutna wiedza ekspercka
  * <!-- .element: class="fragment fade-in" --> Zyskujemy na wydajności, ale tracimy na elastyczności programu
  * <!-- .element: class="fragment fade-in" --> Jakakolwiek zmiana kodu lock-free musi być poprzedzona rzetelnym code-review wielu ekspertów
