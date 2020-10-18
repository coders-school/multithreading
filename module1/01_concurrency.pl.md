<!-- .slide: data-background="#111111"-->
# Współbieżność

___

## Czym jest współbieżność?

* <!-- .element: class="fragment fade-in" --> Dwa procesy są współbieżne jeżeli jeden z nich rozpoczyna się przed zakończeniem drugiego.

<img class="plain fragment fade-in" data-src="img/01.png" src="img/01.png">

* <!-- .element: class="fragment fade-in" --> Współbieżność polega na jednoczesnym wykonywaniu co najmniej dwóch czynności.

___

## Współbieżność w systemach komputerowych

* <!-- .element: class="fragment fade-in" --> „Symulacja” współbieżności w systemach jednoprocesorowych przy wykorzystaniu mechanizmu przełączania zadań (ang. Task switching),
* <!-- .element: class="fragment fade-in" --> Komputery/serwery wieloprocesorowe/wielordzeniowe obsługujące wiele aplikacji jednocześnie.

<img class="plain fragment fade-in" data-src="img/02.png" src="img/02.png" alt="'Concurency in action', Anthony Williams 2019" title="'Concurency in action', Anthony Williams 2019">

___

## Modele współbieżności

* <!-- .element: class="fragment fade-in" --> Współbieżność z wieloma procesami:
  * <!-- .element: class="fragment fade-in" --> Osobne procesy (komunikacja przez sygnały, pliki, potoki, gniazda itd.),
  * <!-- .element: class="fragment fade-in" --> Osobna przestrzeń pamięci, bezpieczeństwo przetwarzania wielowątkowego,
  * <!-- .element: class="fragment fade-in" --> Długi i skomplikowany proces uruchamiania nowego procesu.
* <!-- .element: class="fragment fade-in" --> Współbieżność z wieloma wątkami:
  * <!-- .element: class="fragment fade-in" --> Uruchamianie wielu wątków w obrębie jednego procesu, które współdzielą pamięć (współdzielona przestrzeń adresowa),
  * <!-- .element: class="fragment fade-in" --> Należy samemu zagwarantować bezpieczeństwo dostępu do współdzielonej pamięci,
  * <!-- .element: class="fragment fade-in" --> Uruchomienie wątku jest szybsze niż uruchomienie nowego procesu,
  * <!-- .element: class="fragment fade-in" --> Prostsze i szybsze metody komunikowania się pomiędzy wątkami w obrębie procesu

___

## Kiedy stosować współbieżność?

* <!-- .element: class="fragment fade-in" --> Podział zagadnień (np. odtwarzacz Blu-ray),
* <!-- .element: class="fragment fade-in" --> Wydajność:
  * <!-- .element: class="fragment fade-in" --> Zrównoleglanie zadań (podział zadania na części),
  * <!-- .element: class="fragment fade-in" --> Zrównoleglanie danych (wykonywanie tych samych zadań, na różnych fragmentach danych)

___

## Kiedy nie stosować współbieżności?

* <!-- .element: class="fragment fade-in" --> Gdy zbyt wiele wątków może obniżyć wydajność zamiast ją zwiększyć (koszt uruchomienia wątku, koszt zasobów – każdy wątek zajmuje pamięć typowo 8MB),
* <!-- .element: class="fragment fade-in" --> Gdy wzrost wydajności nie jest proporcjonalny do włożonego wysiłku i złożoności kodu (koszt utrzymania kodu jest równie istotny).
<!-- Oryginalnie to brzmiało: Gdy wzrost wydajności nie jest proporcjonalny do woźnego wysiłku i złożoności kodu (koszt utrzymania kodu jest równie istotny)., zamieniłem "woźnego" na "włożonego", ale nie jestem pewien -->

___

## Proces oraz wątek

* <!-- .element: class="fragment fade-in" --> Proces służy do organizowania wykonywania programu. W skład jednego programu wchodzi jeden lub więcej procesów. Zatem proces jest to cały kontekst niezbędny do wykonania programu.
* <!-- .element: class="fragment fade-in" --> Zmianie w wyniku wykonywania procesu ulega między innymi segment danych, segment stosu, stan rejestrów procesora.
* <!-- .element: class="fragment fade-in" --> W momencie wykonywania procesu system operacyjny przydziela procesowi niezbędne zasoby (pamięć, czas procesora itp.).
* <!-- .element: class="fragment fade-in" --> Synchronizacja, sposób obsługi procesów itp. Kontrolowana jest przez system operacyjny.
* <!-- .element: class="fragment fade-in" --> W obrębie każdego procesu istnieje jeden lub więcej wątków.

___

## Proces oraz wątek

* <!-- .element: class="fragment fade-in" --> Wątki tego samego procesu współdzielą większość przestrzeni adresowej (segment kodu i danych, otwarte pliki Itp.).
* <!-- .element: class="fragment fade-in" --> Przełączanie kontekstu wątku jest stosunkowo szybkie i nie obciążające systemu operacyjnego.
* <!-- .element: class="fragment fade-in" --> Tworzenie wątku wymaga mniej zasobów do działania i jest szybsze niż tworzenie procesu.
* <!-- .element: class="fragment fade-in" --> Łatwa (ale również niebezpieczna) komunikacja pomiędzy wątkami w obrębie jednego procesu.
* <!-- .element: class="fragment fade-in" --> Każdy wątek posiada odrębny stos (adres powrotu z funkcji oraz zmienne lokalne).
