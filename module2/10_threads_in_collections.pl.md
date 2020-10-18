<!-- .slide: data-background="#111111" -->

# Wątki w kolekcjach

___

## Zadanie: wątki w kolekcjach

* <!-- .element: class="fragment fade-in" --> Napisz krótki program, w którym 20 wątków jest trzymane w wektorze.
* <!-- .element: class="fragment fade-in" --> Każdy wątek ma za zadanie poczekać 1 sekundę, po czym wyświetlić swój numer, który przyjmuje jako parametr oraz znak nowej linii.
* <!-- .element: class="fragment fade-in" --> Tworzenie wątków i ich przyłączanie powinno zostać zrealizowane w 2 oddzielnych pętlach
* <!-- .element: class="fragment fade-in" --> Uruchom program kilka razy i zaobserwuj jakie daje wyniki

___

## Zadanie 5 - rozwiązanie

```cpp
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

void do_work(int id) {
    this_thread::sleep_for(1s);
    cout << id << endl;
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

___

## Zadanie 5 – możliwe rezultaty

```output
$> ./a.out
0 1 3 2 5 6 8 10 12 7 11 9 13 4 14 15 17 16 19 18

$> ./a.out
4 5 7 6 14 15 16 18 13 11 9 2 0 8 10 17 12 1 19 3

$> ./a.out
18 016 2 3 4 5  615 8 9 10 11  12 13 14 19 1 7 17

$> ./a.out
0 1 16 3 199 815 11 5 18 17 4 12 13   7 102 6 14 
```

___

## Zadanie 5 – możliwe rezultaty

* <!-- .element: class="fragment fade-in" --> Dlaczego tak się dzieje?
* <!-- .element: class="fragment fade-in" --> Strumień wyjściowy <code>cout</code> jest tylko jeden. Jest on wspólnym zasobem współdzielonym między wątkami
* <!-- .element: class="fragment fade-in" --> Może dochodzić do przeplotów w trakcie dostępu do strumienia (jeden wątek zacznie coś wpisywać i nie skończy, a już drugi wejdzie mu w paradę i wpisze swój numer)
* <!-- .element: class="fragment fade-in" --> Współdzielenie zasobów to typowy problem wielowątkowości
* <!-- .element: class="fragment fade-in" --> Jak sobie z tym poradzić? To już temat na kolejną lekcję 🙂
