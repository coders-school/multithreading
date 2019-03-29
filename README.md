# Tematy:
01. Teoria Wielowątkowości (multithreading theory)
02. Wątki (threads) - [Test](https://forms.gle/yRATL7rzMfTSnrGa9)
03. Współdzielenie danych (data_sharing) - [Test](https://forms.gle/s517xwB7ZKpMgPTV6)
04. Zmienne warunku (condition_variable) - [Test](https://forms.gle/e9uHQLAcYauHbtPS9)
05. Zmienne atomowe i model pamięci (atomic)
06. Jednokrotne wywołania (call_once)
07. Funkcje asynchroniczne (async, future, promise)

# Praca domowa:
### Zadanie 1:
Zaimplementuj problem ucztujących filozofów z użyciem wątków i mutexów.
- Każdy filozof ma być kontrolowany przez oddzielny wątek.
- Każdy sztuciec ma być chroniony przez 1 mutex
- Postaraj się o wizualizację problemu
- Strzeż się zakleszczeń
- Pobaw się liczbą filozofów i zobacz czy zmienia się zachowanie programu
- Zadanie dodatkowe: Strzeż się zagłodzenia (starvation). Jest to sytuacja w której przynajmniej 1 wątek z powodu implementacji lub swojego niższego priorytetu nigdy nie dostanie wszystkich wymaganych zasobów. Doimplementuj w tym celu pewien mechanizm, który zapobiegnie zagłodzeniu.

<details><summary>Podpowiedź</summary>
<p>
https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/
</p><p>
https://mariusbancila.ro/blog/2017/01/20/dining-philosophers-in-c11-chandy-misra-algorithm/
</p>
</details>

### Zadanie 2:
Napisz równoległą wersję algorytmu std::accumulate, który będzie współbieżnie sumował fragmenty kontenera. Wyniki powinny zostać również zapisane w kontenerze.
  - on empty returns init
  - calculate number of threads - `hardware_threads = hardware_concurrency() != 0 ? hardware_concurrency() : 2`
  - divide on blocks according to the number of threads
  - create vector of results
  - run every thread in parallel
  - put results in vector element passed by reference (threads cannot return a value)
  - join threads
  - accumulate results from the result vector
  - test on 1M elements and compare with standard std::accumulate
  - compare with std::par execution policy in std::accumulate from C++17 ;)
  - templatize algorithm on Iterator (depends on container) and type T (usually int, double)

<details><summary>Podpowiedź</summary>
<p>
Rozwiązanie znajdziesz w książce *C++ Concurrency in Action, Anthony Williams*, listing 2.8
</p>
</details>

### Zadanie 3:
- Poszukaj informacji na temat zjawiska nazywanego **false sharing**. Jest to bardzo niemiłe zjawisko, wynikające z architektury współczesnych komputerów, które znacząco spowalnia równoległe wykonywanie programów.
- Napisz/przepisz/skopiuj sobie kawałek kodu, który ma ten problem i zmierz czas działania programu w zależności od wielkości danych wejściowych i liczby wątków
- Napraw problem false sharingu i zmierz czas działania programu również przy różnej wielkości danych wejściowych i liczbie wątków
- Wyciągnij wnioski jak pisać kod odporny na false sharing :)

<details><summary>Podpowiedź</summary>
<p>
Zjawisko false sharingu możesz napotkać w zadaniu 2.
</p><p>
Zaobserwujesz je, jeśli utworzysz dużo wątków liczących bardzo małe fragmenty kontenera.
</p>
</details>

### Zadanie 4:
- Zagraj w [The Deadlock Empire](https://deadlockempire.github.io) i przejdź wszystkie poziomy :D

### Zadanie 5:
Zaimplementuj grę w ping-ponga (zadanie 03\_ping\_pong.cpp z rozdziału o zmiennych warunku)
- 1 wątek wypisuje "ping" oraz kolejny numer
- 2 wątek wypisuje "pong" oraz kolejny numer
- Zaczyna wątek ping, a kończy zawsze pong. Wątki muszą pracować na przemian. Nie mogą być 2 pingi lub pongi po sobie. Program nie może zakończyć się pingiem, na który nie będzie odpowiedzi – ponga.
- Zakończenie działania programu ma nastąpić albo po wykonanej liczbie odbić albo po limicie czasowym, w zależności które wystąpi pierwsze. Powód zakończenia powinien zostać wyświetlony na ekranie
- Parametry programu:
    - liczba odbić
    - limit czasowy (w sekundach)

### Zadanie 6:
Rozwiąż zadania z użyciem `promise` i `future` od Ihora - [GitHub](https://github.com/ihor-rud/future_promise_homework)

# Dla chętnych:
### Przetwarzanie obrazów
Zainstaluj sobie bibliotekę OpenCV, która służy m.in. do przetwarzania obrazów.
[Instrukcja instalacji](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)

Przerób algorytmy podane w dokumentacji na ich wielowątkowe wersje:
1. [Wykrywanie wzorców](https://docs.opencv.org/master/de/da9/tutorial_template_matching.html).

Każdy wątek sprawdza inny wzorzec. Możemy jednocześnie szukać kilku rzeczy na obrazku. Na koniec wyświetl na obrazku miejsca, gdzie wybrane wzorce najbardziej pasowały. Jeden wzorzec może występować w kilku miejscach.

2. [Wykrywanie linii](https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html). 

Każdy wątek szuka linii pod innym kątem. Musisz stworzyć odpowiednie maski. Na koniec wyświetl na obrazku znalezione linie - każdy kąt innym kolorem. Możesz też dodać wyszukiwanie okręgów. 

--- 

# Projekt 1: Hardkorowi filozofowie
Zaimplementuj trochę inną wersję algorytmu ucztujących filozofów :) Możesz to robić samemu lub w grupie.
- Chłop zadaje ważne egzystencjalnie pytanie 5 filozofom (wprowadź je z klawiatury)
- Każdy filozof posiada gotowy zestaw 10 odpowiedzi (każdy filozof ma inne odpowiedzi)
- Każdy filozof podczas myślenia ma zaimplementowany inny algorytm obliczający pewne wartości
- Algorytmy filozofów jako parametr wejściowy przyjmują pytanie (`std::string`) oraz odpowiedź (`std::string`) i zwracają liczbę `int`. Jeśli nie masz pomysłu na różne algorytmy oblicz kilkukrotny `std::hash` z zestawu pytanie + odpowiedź.
- Wszyscy filozofowie podczas myślenia korzystają także z bardzo mądrej księgi, która stanowi dla nich inspirację. Wielu filozofów może odczytywać księge jednocześnie. Wartości zapisane w księdze także mogą być brane pod uwagę w algorytmie.
- Każdy filozof musi przemyśleć każdą z 10 swoich odpowiedzi, która stanowi parametr wejściowy do jego algorytmu myślenia.
- Zadania do przemyślenia przez filozofów możesz zaimplementować jako pulę wątków (Thread Pool).
- Każdy filozof, aby przemyśleć pojednynczą odpowiedź musi być najedzony. Nie może więc rozpocząć myślenia bez jedzenia.
- Każdy filozof potrzebuje 2 sztućców, aby móc się najeść. Liczba szućców jest równa liczbie filozofów.
- Każdy filozof po przemyśleniu odpowiedzi zapisuje swoje rozważania w księdze. Tylko 1 filozof na raz może zapisywać rozważania i żaden inny nie może tym momencie korzystać z księgi.
- Przemyślenia w księdze mają następujący format:
```cpp
struct {
    string philosopher,
    string answer,
    int result,
    chrono::duration period,
    bool chosen = false,
}
```
- Żaden filozof nie może się przejeść. Po posiłku musi on poczekać 3 sekundy zanim zacznie kolejny posiłek.
- Jeśli filozof nie jadł przez 20 sekund to umiera śmiercią głodą. Nie dopuść do tego!
- W czasie poszukiwania odpowiedzi, możesz usypiać filozofów. W tym celu musisz z klawiatury podać odpowiednią komendę.
- Uśpiony filozof nie może myśleć ani jeść. Umiera on z głodu dopiero po 60 sekundach.
- Uśpionego filozofa można obudzić podając odpowiednią komendę z klawiatury.
- Na koniec każdy filozof odczytuje odpowiedź która została przez niego wybrana (oblicza max z działań `result * period`). W tym celu musi odczytać księgę, gdyż tam zapisał swoje rozważania. Przy wybranej odpowiedzi ustawia `chosen = true`.

MEGA PODPOWIEDŹ: Pracuj w TDD i używaj Thread Sanitizera :)

# Projekt 2: Wielowątkowy raytracer
TBD.
- Zapisywanie obrazów w plikach .ppm.
- Podział obrazu pomiędzy wiele zadań asynchronicznych - badanie szybkości przetwarzania w zależności od rodzaju podziału
- Unikanie false sharingu
