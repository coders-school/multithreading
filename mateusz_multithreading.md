Wielowątkowość

___

Czym jest współbieżność?
! Dwa procesy są współbieżne jeżeli jeden z nich rozpoczyna się przed 
zakończeniem drugiego.  
! współbieżność polega na jednoczesnym wykonywaniu co najmniej 
dwóch czynności.

___

Współbieżność w systemach 
komputerowych
! „Symulacja” współbieżności w systemach jednoprocesorowych przy 
wykorzystaniu mechanizmu przełączania zadań (ang. Task 
switching), 
! Komputery/ serwery wieloprocesorowe/ wielordzeniowe obsługujące 
wiele aplikacji jednoczenie. 
                                   źródło: „Concurency in action”, Anthony Williams 2019.

___

Modele współbieżności
! Współbieżność z wieloma procesami: 
! Osobne procesy (komunikacja przez sygnały, pliki, potoki, gniazda itd.),
! Osobna przestrzeń pamięci, bezpieczeństwo przetwarzania wielowątkowego,
! Długi i skomplikowany proces uruchamiania nowego procesu.
! Współbieżność z wieloma wątkami: 
! Uruchamianie wielu wątków w obrębie jednego procesu, które współdzielą 
pamięć (współdzielona przestrzeń adresowa),
! Należy samemu zagwarantować bezpieczeństwo dostępu do współdzielonej 
pamięci,
! Uruchomienie wątku jest szybsze niż uruchomienie nowego procesu,
! Prostsze i szybsze metody komunikowania się pomiędzy watkami w obrębie 
procesu.

___

Kiedy stosować współbieżność?
! Podział zagadnień (np. odtwarzacz Blu-ray), 
! Wydajność: 
! Zrównoleglanie zadań (podział zadania na części),
! Zrównoleglanie danych (wykonywanie tych samych zadań, na różnych 
fragmentach danych)

___

Kiedy nie stosować współbieżności?
! Gdy zbyt wiele wątków może obniżyć wydajność zamiast ja 
zwiększyć (koszt uruchomienia wątku, koszt zasobów – każdy watek 
zajmuje pamięć typowo 8MB.), 
! Gdy wzrost wydajności nie jest proporcjonalny do woźnego wysiłku               
i złożoności kodu (koszt utrzymania kodu jest równie istotny).

___

Proces oraz wątek 
! Proces służy do organizowania 
wykonywania programu. W skład 
jednego programu wchodzi jeden lub 
więcej procesów. Zatem proces jest to 
cały kontekst niezbędny do wykonania 
programu. 
! Zmianie w wyniku wykonywania procesu 
ulega między innymi segment danych, 
segment stosu, stan rejestrów procesora. 
! W momencie wykonywania procesu 
system operacyjny przydziela procesowi 
niezbędne zasoby (pamięć, czas 
procesora itp.). 
! Synchronizacja, sposób obsługi procesów 
itp. Kontrolowana jest przez system 
operacyjny.
! W obrębie każdego procesu istnieje jeden lub 
więcej wątków. 
! Wątki tego samego procesu współdzielą 
większość przestrzeni adresowej (segment 
kodu i danych, otwarte pliki Itp.). 
! Przełączanie kontekstu wątku jest stosunkowo 
szybki i nie obciążające system operacyjny. 
! Tworzenie wątku wymaga mniej zasobów do 
działania i jest szybsze niż tworzenie procesu. 
! Łatwa (ale również niebezpieczna) 
komunikacja pomiędzy wątkami w obrębie 
jednego procesu. 
! Każdy wątek posiada odrębny stos (adres 
powrotu z funkcji oraz zmienne lokalne).

___

C++ i obsługa wielowątkowości
! Standard C++11/14/17/20 – Wprowadzenie i rozwój bibliotek 
odpowiadających za obsługę wielowątkowości, 
! Przed C++11 trzeba było korzystać z różnych bibliotek, lub ręcznie 
odwoływać się do interfejsów API udostępniających mechanizmy 
wielowątkowe, 
! C++11 wprowadził również nowy model pamięci przystosowany do 
przetwarzania wielowątkowego na wielu platformach.  
! Wprowadzono zarządzanie wątkami (ang. Thread), ochronę 
współdzielonych danych, synchronizacje operacji wykonywanych 
przez wątki, wykonywanie niskopoziomowych operacji atomowych 
itp.

___

„Hello World”

___

Zarządzanie wątkami
! std::thread, 
! std::thread::join(),  
! std::thread::detach(),  
! std::thread::joinable(),

___

Co przekazać do std::thread?
! Funkcje, 
! Funktor (obiekt funkcyjny) – obiekt, który możemy wywołać tak jak 
funkcję (np. lambda), 
! Wskaźnik na funkcje lub wskaźnik do funkcji składowej,  
! Obiekt funkcyjny jest kopiowany do obszaru 
     pamięci należącej do nowo utworzonego wątku.

___

Co przekazać do std::thread?

___

std::thread::detach()
! Zadanie 1: 
! Jakie zagrożenia kryje kod po prawej?
! Jak poprawić kod?

___

std::terminate 
std::thread::~thread 
! If *this has an associated thread 
(joinable() == true), std::terminate() is called. 
! ZAWSZE należy zawołać std::join() lub std::detach() na wątku! 
! Co z wyjątkami, jakieś pomysły?

___

std::exception
! Zadanie 2: 
! Jakie zagrożenia kryje listing poniżej?
! Jak lepiej można rozwiązać poniższy problem?

___

RAII (Resource Acquisition Is 
Initialization)
! Własna Klasa ThreadGuard – zapewniająca  
     bezpieczeństwo złączania wątków.

___

Przekazywanie argumentów do 
funkcji wątku
! Przekazywanie przez wartość

___

Przekazywanie argumentów do 
funkcji wątku
! Przekazywanie przez wskaźnik, referencje.

___

Pułapki podczas przekazywania 
argumentów
! Zadanie 3: Jakie pułapki kryje kod poniżej?

___

Pułapki podczas przekazywania 
argumentów
! Należy zwrócić uwagę na przekazywane argumenty 
! Jeżeli zmienna jest wskaźnikiem/ referencją, należy zadbać aby długość jej 
życia była dłuższa niż wątku, który na niej operuje.
! Jeżeli istnieje ryzyko niejawnej konwersji, najlepiej od razu przekazać 
przekonwertowany argument. 

___

Przenoszenie wątków
! Tak jak np. std::unique_ptr, wątki mogą być jedynie przenoszone (ich 
kopiowanie nie miało by sensu, gdyż 2 obiekty zarządzałyby jednym 
wątkiem) 
! Do przenoszenia wątków wykorzystujemy bibliotekę utility i funkcję 
std::move

___

Wybór liczby wątków podczas 
implementacji
! Zbyt dużo wątków – program działa wolniej, 
! Zbyt mało wątków – brak wykorzystania potencjału, 
! std::thread::hardware_concurrency()

___

Identyfikacja wątków
! std::this_thread::get_id() 
! std::thread::id() 
! Można porównywać id, można je wyświetlać poprzez std::cout 
! Możemy wykorzystać je do identyfikacji poszczególnych wątków

___

Usypianie wątków
! std::this_thread::sleep_until 
! std::this_thread::sleep_for 
! std::chrono 
! std::chrono_literals

___

Zadanie 4
! Zaimplementuj program w którym 4 wątki będą wyświetlać swój 
std::this_thread::id() określoną liczbę razy.

___



___

Zadanie 5
! Zaimplementuj algorytm std::accumulate wykorzystując 
wielowątkowość. 

___

Wskazówka 1
! Obliczenie wymaganych wątków 
! Stworzenie kontenera przechowującego wyniki

___

Wskazówka 2
! Implementacja funktora dla wątków 
! Funktor przyjmuje kolejne porcje danych
! Należy obliczyć i przekazać mu iteratory odnoszące się do początku i końca 
zakresu na którym ma operować wątek.
! Należy przekazać także przez  referencje zmienną przechowującą wynik.

___

Wskazówka 3
! Należy wykorzystać również aktualny wątek, na którym wywoływana 
jest funkcja, aby nie czekał bezczynnie na wywołanie pozostałych 
wątków.

___

Czy zadanie było trudne?
! Co sprawiło największy problem? 
! Czy można uprościć algorytm? 
! Czy można zastosować inne mechanizmy ułatwiające 
implementacje?

___

Zadanie domowe
! 1) Zaimplementuj algorytm count_if wykorzystując wielowątkowość. 
! POWODZENIA ☺

___

Dziękuję za uwagę

___

