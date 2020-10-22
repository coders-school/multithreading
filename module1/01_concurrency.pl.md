<!-- .slide: data-background="#111111"-->
# Współbieżność

___

## Czym jest współbieżność?

* <!-- .element: class="fragment fade-in" --> Dwa procesy są współbieżne jeżeli jeden z nich rozpoczyna się przed zakończeniem drugiego

<img class="plain fragment fade-in" data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> Współbieżność polega na jednoczesnym wykonywaniu co najmniej dwóch czynności

___

## Współbieżność w systemach komputerowych

* <!-- .element: class="fragment fade-in" --> Komputery wieloprocesorowe/wielordzeniowe obsługujące wiele aplikacji jednocześnie
* <!-- .element: class="fragment fade-in" --> Symulacja współbieżności w systemach jednoprocesorowych przy wykorzystaniu mechanizmu przełączania zadań (ang. Task switching)

<img class="plain fragment fade-in" data-src="img/02.png" src="img/02.png" alt="'Concurency in action', Anthony Williams 2019" title="'Concurency in action', Anthony Williams 2019">

Note: Show `top`

___

## Modele współbieżności

| &nbsp;        | Wątki                                                                              | Procesy                                                                                                |
| :------------ | :----------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------- |
| Stos         | Oddzielne <!-- .element: class="fragment fade-in" --> | Oddzielne <!-- .element: class="fragment fade-in" --> |
| Sterta          | Wspólna <!-- .element: class="fragment fade-in" -->   | Oddzielne <!-- .element: class="fragment fade-in" --> |
| Tworzenie      | Szybkie <!-- .element: class="fragment fade-in" -->     | Wolne <!-- .element: class="fragment fade-in" --> |
| Komunikacja | Szybka (sterta) - wskaźniki, struktury danych  <!-- .element: class="fragment fade-in" --> | Wolniejsza (IPC) - gniazdka, pliki, sygnały, strumienie, pamięć współdzielona <!-- .element: class="fragment fade-in" --> |

___

## Proces

* <!-- .element: class="fragment fade-in" --> Proces służy do organizowania wykonywania programu. W skład jednego programu wchodzi jeden lub więcej procesów. Zatem proces jest to cały kontekst niezbędny do wykonania programu.
* <!-- .element: class="fragment fade-in" --> Zmianie w wyniku wykonywania procesu ulega między innymi segment danych, segment stosu, stan rejestrów procesora.
* <!-- .element: class="fragment fade-in" --> W momencie wykonywania procesu system operacyjny przydziela procesowi niezbędne zasoby (pamięć, czas procesora itp.).
* <!-- .element: class="fragment fade-in" --> Synchronizacja, sposób obsługi procesów itp. Kontrolowana jest przez system operacyjny.
* <!-- .element: class="fragment fade-in" --> W obrębie każdego procesu istnieje jeden lub więcej wątków.

___

## Wątek

* <!-- .element: class="fragment fade-in" --> Wątki tego samego procesu współdzielą większość przestrzeni adresowej (segment kodu i danych, otwarte pliki Itp.).
* <!-- .element: class="fragment fade-in" --> Przełączanie kontekstu wątku jest stosunkowo szybkie i nie obciążające systemu operacyjnego.
* <!-- .element: class="fragment fade-in" --> Tworzenie wątku wymaga mniej zasobów do działania i jest szybsze niż tworzenie procesu.
* <!-- .element: class="fragment fade-in" --> Łatwa (ale również niebezpieczna) komunikacja pomiędzy wątkami w obrębie jednego procesu.
* <!-- .element: class="fragment fade-in" --> Każdy wątek posiada odrębny stos (adres powrotu z funkcji oraz zmienne lokalne).
