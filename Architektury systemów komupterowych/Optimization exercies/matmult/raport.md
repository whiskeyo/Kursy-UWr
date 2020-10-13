# Raport do zadania 1

### Autor: Tomasz Woszczyński
### Numer indeksu: 307690

Konfiguracja
---

Informacje o systemie:

 * Dystrybucja: Debian GNU/Linux 10 (buster)
 * Jądro systemu: 4.19.0-9-amd64
 * Kompilator: GCC 8.3.0
 * Procesor: Intel(R) Core(TM) i5-7300HQ CPU @ 2.50GHz
 * Liczba rdzeni: 4

Pamięć podręczna:

 * L1d: 32 KiB, 8-drożny (per rdzeń), rozmiar linii 64B
 * L2: 256 KiB, 4-drożny (per rdzeń), rozmiar linii 64B
 * L3: 6 MiB , 12-drożny (współdzielony), rozmiar linii 64B

Pamięć TLB:

 * L1d: 4KiB strony, 4-drożny, 64 wpisy
 * L2: 4KiB strony, 6-drożny, 1536 wpisów

Informacje o pamięciach podręcznych uzyskano na podstawie wydruku programu
`x86info` oraz [z tej strony](http://www.cpu-world.com/CPUs/Core_i5/Intel-Core%20i5%20i5-7300HQ.html).

Wyniki eksperymentów
---
Wyniki otrzymane przy poleceniu `make sim`:

| Funkcja | Time Elapsed | Branch misprediction ratio | L1 miss ratio | LL miss ratio |
| ------- | ------------ | -------------------------- | ------------- | ------------- |
| `matmult0` |  1.709590 | 0.391% | 50.167% | 50.147% |
| `matmult1` |  1.355105 | 0.391% | 6.457%  | 0.404% |
| `matmult2` |  1.930265 | 0.391% | 100%    | 100% |
| `matmult3` |  1.570854 | 6.250% | 2.087%  | 0.441% |

Wyniki czasowe są dosyć bliskie siebie, najbardziej różnią się proporcje chybień, co jest spowodowane sposobem przechodzenia i wydobywania danych.

Po kilkukrotnym przeprowadzeniu testów i uśrednieniu otrzymanych wyników, przedstawiłem je na wykresie:
![wykres1](plot1.png)
Wyniki w przybliżeniu pokrywają się z wynikami z prezentacji "Cache memories", jednak więcej informacji możemy wyciągnąć dla rozmiarów tablicy, które wychodzą poza zakres tych, które przedstawiono na prezentacji. Ostatni test przeprowadzono na dwóch macierzach $2048 \times 2048$, a więc każda z nich zajmuje 32768 KiB.

Możemy zobaczyć, że najlepiej działają funkcje `matmult1` oraz `matmult3`, co zgadza się z zasadą lokalności przestrzennej, przez co procesor może operować na zbliżonych adresach, dzięki czemu ogranicza ilość dostępów do pamięci, a w rezultacie oszczędzając czas. 

Niestety funkcje `matmult0` oraz `matmult2` są bardzo nieoptymalne, pierwsza wspomniana działa około dziesięciokrotnie wolniej od `matmult1` oraz `matmult3`, a druga ponad dwudziestokrotnie wolniej. Spowodowane jest to koniecznością częstego odwoływania się do pamięci, co bardzo spowalnia program przy tak dużych rozmiarach macierzy.

Przeprowadzając testy funkcji `matmult3` z różnymi wielkościami kafelka (polecenie `make test-3`, a więc $n = 1024$) doszedłem do wniosku, iż najlepszym wyborem będzie `BLOCK` równe 16. Zwiększanie rozmiaru kafelka sprawia, że nie jesteśmy w stanie pobrać całego kafelka do pamięci podręcznej L1, przez co trzeba kilkukrotnie pobierać dane z tego samego kafelka. Zmniejszanie rozmiaru sprawia, że częściej odwołujemy się do pamięci, co również wydłuża czas działania programu. Wyniki tego pomiaru przedstawia poniższy wykres:
![wykres2](plot2.png)

Po określeniu odpowiedniego rozmiaru kafelka przechodzę do części zadania, w której porównać należy wpływ wartości `A_OFFSET`, `B_OFFSET`, `C_OFFSET` na czas działania programu. Cztery funkcje okreslaja wartosci offsetu: `default` mowi o standardowym wyborze (bez zmian pliku `matmult.h`, tj. 210), `off` o ustawionych wartosciach na 0 (000), a `reversed` o odwrotnej kolejnosci offsetów w stosunku do domyślnego ustawienia (012) oraz `2covered` o pokrywającym się offsecie dla dwóch macierzy (011). Eksperyment przeprowadziłem na powyższych ustawieniach, najlepsze pod względem wydajności było o dziwo ustawienie `off`, gdzie średni czas testów wynosił $6.6642s$, następnie dla `reversed` - $6.8618s$, później dla `default` - $6.8894s$, a ostatecznie dla `2covered` - $7.0188s$.

Obliczenia te potwórzyłem jeszcze dla macierzy $4096 \times 4096$, wyniki znacząco uległy zmianie: najlepiej sprawdziły się ustawienia `default` oraz `reversed`, które dla każdej macierzy miały przypisany własny zbiór (czasy $72.186s$ oraz $72.372s$). Kolejnym ustawieniem w rankingu było `off` z czasem $73.666s$, a na końcu `2covered` z czasem $75.064s$. 

Wnioski
---
Mnożąc macierze algorytmami o złożoności $O(n^3)$ musimy jak najbardziej zmniejszyć stałą. Funkcje `matmult1` oraz `matmult3` robią to w bardzo dobry sposób, zmniejszając ilość odwołań do pamięci, dzięki czemu czas działania programu znacznie skraca się.

W przypadku funkcji `matmult3` operującej na kafelkach, ważne jest dobranie odpowiedniej wartości, którą dla mojego procesora jest 16. Dzięki temu rzadziej odwołujemy się do pamięci, skracając czas działania.

Wartości `*_OFFSET` nie mają aż tak dużego wpływu na działanie programu jak dobór rozmiaru kafelka, gdyż nawet dla bardzo dużych macierzy wyniki różniły się zaledwie o kilka procent.

Podsumowując, najbardziej optymalnym wyborem jest użycie funkcji `matmult3` z rozmiarem kafelka 16 i domyślnym ustawieniem offsetów.
