# Raport do zadania 3

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
Pomysłem na zoptymalizowanie kodu funkcji `randwalk0` jest zastąpienie instrukcji warunkowych `if` na wyrażenia korzystające z operatora trójargumentowego `boolexpr ? trueexpr : falseexpr`. Dzięki temu zmniejszamy liczbę (długo działających) skoków warunkowych wewnątrz pętli z 7 do 3 i zwiększamy liczbę (szybkich) instrukcji `set*` z 3 do 8. W przypadku długości kodu, funkcja `randwalk1` jest dwa razy krótsza, liczba instrukcji maszynowych jednak się nie zmienia.

Po wielokrotnym sprawdzeniu czasów działania i procentu chybień skoków uzyskałem następujące pomiary, przeprowadzając je następującym poleceniem:
```
./randwalk -S 0x321 -n 7 -s 16 -t 14 -S 0x123
```
Pierwsze dwie kolumny, oznaczone `rw0`, odnoszą się do funkcji `randwalk0`, a dwie kolejne do funkcji `randwalk1`.

| Time `rw0`| Miss % `rw0` | Time `rw1` | Miss % `rw1`| 
|:---------:|:---------:|:---------:|:---------:|
| 6.738352  |  20.2  |   3.768810  |   1.5    |
| 6.738185  |  20.2  |   3.772370  |   1.4    |
| 6.735709  |  20.2  |   3.797467  |   1.5    |
| 6.738812  |  20.2  |   3.935206  |   1.5    |
| 6.893852  |  20.2  |   3.818773  |   1.5    |
| 6.815577  |  20.2  |   3.788178  |   1.4    |
| 6.897638  |  20.2  |   3.778380  |   1.5    |
| 6.847746  |  20.2  |   3.846177  |   1.5    |
| 6.765558  |  20.2  |   3.793622  |   1.5    |
| 6.878584  |  20.2  |   3.774503  |   1.5    |
| 6.730601  |  20.2  |   3.768976  |   1.4    |
| 6.764019  |  20.2  |   3.785110  |   1.4    |
| 6.730081  |  20.2  |   3.765942  |   1.5    |
| 6.740206  |  20.2  |   3.768035  |   1.5    |
| 6.830657  |  20.1  |   3.805945  |   1.5    |
| 6.725921  |  20.2  |   3.763425  |   1.5    |
| 6.716422  |  20.2  |   3.781124  |   1.5    |
| 6.723256  |  20.2  |   3.877020  |   1.5    |
| 6.728123  |  20.2  |   3.762472  |   1.5    |
| 6.732379  |  20.2  |   3.767358  |   1.3    |
| 6.726626  |  20.2  |   3.754204  |   1.5    |
| 6.747848  |  20.2  |   3.769763  |   1.5    |
| 6.781344  |  20.2  |   3.775713  |   1.5    |
| 6.730252  |  20.2  |   3.782054  |   1.4    |
| 6.732588  |  20.2  |   3.801639  |   1.5    |
| 6.731967  |  20.2  |   3.765038  |   1.5    |
| 6.810175  |  20.2  |   3.905231  |   1.5    |
| 6.741014  |  20.2  |   3.771609  |   1.4    |
| 6.809461  |  20.2  |   3.776652  |   1.4    |
| 6.828123  |  20.2  |   3.784548  |   1.5    |
| 6.744593  |  20.2  |   3.776834  |   1.5    |
| 6.732378  |  20.2  |   3.804752  |   1.5    |
| 6.790683  |  20.2  |   3.781512  |   1.5    |
| 6.780351  |  20.2  |   3.893058  |   1.5    |
| 6.810947  |  20.2  |   3.764468  |   1.3    |
| 6.735251  |  20.2  |   3.771179  |   1.5    |
| 6.738756  |  20.2  |   3.825621  |   1.5    |
| 6.770279  |  20.2  |   3.789567  |   1.5    |
| 6.742631  |  20.2  |   3.771778  |   1.5    |
| 6.774459  |  20.2  |   3.769160  |   1.5    |
| 6.884832  |  20.2  |   3.754288  |   1.5    |
| 7.169022  |  20.2  |   3.878324  |   1.4    |
| 6.928068  |  20.2  |   3.777119  |   1.4    |
| 6.738285  |  20.2  |   3.898837  |   1.5    |
| 6.796404  |  20.2  |   3.772343  |   1.5    |
| 6.734739  |  20.2  |   3.780872  |   1.5    |
| 6.807133  |  20.2  |   3.775060  |   1.5    |
| 6.950922  |  20.2  |   3.758746  |   1.5    |
| 6.800994  |  20.2  |   3.766045  |   1.5    |
| 6.744697  |  20.2  |   3.835382  |   1.5    |

![](tests.png)

Uśrednione wyniki: dla funkcji `randwalk0` średni czas działania to 6.78s oraz 20.2% chybień skoków, a dla funkcji `randwalk1` średni czas to tylko 3.79s oraz jedynie 1.47% chybień skoków. Daje to znaczącą poprawę w czasie działania programu.

Uruchamiając program z flagą `-p ipc` możemy sprawdzić liczbę wszystkich wykonanych instrukcji, jak i średnią ilość instrukcji na cykl. W przypadku niezoptymalizowanej funkcji jest to około 231 mld instrukcji, przypada ich 1.006 na cykl. Dla funkcji najlepiej zoptymalizowanej (sprawdzającej wartość `d` po kolei od 0 do 3) jest już tylko około 43.6 mld instrukcji, a na cykl przypada ich około 3.455 - jest to ogromna poprawa w stosunku do wolniejszej i mniej optymalnej wersji.

Możemy sprawdzić jak zmieni się IPC po zmianie kolejności tych instrukcji warunkowych: dla wartości `d` kolejno `1203` będzie 45.7 mld instrukcji oraz IPC 2.402, dla `3120` będzie 46.8 mld instrukcji oraz IPC 2.232, jednak dla kolejności `3210` jest 46.8 mld instrukcji, natomiast IPC wzrasta do 3.454. Oznacza to, że sprawdzając wartości `d` ułożone rosnąco lub malejąco otrzymamy największe IPC ze wszystkich.

Ostatnim zadaniem jest porównanie czasów działania dla różnych rozmiarów tablicy. Wykonane pomiary przedstawia poniższy wykres:
![](times.png)

Jak łatwo zauważyć, czas działania funkcji nie jest zależny od rozmiaru tablicy, ponieważ w każdej iteracji pętli możemy przesunąć się do komórki obok tej, w której aktualnie się znajdujemy - z tego powodu nie potrzebujemy wiele razy odwoływać się do pamięci i lokalność przestrzenna jest duża.

Wnioski
---
Warto używać operatora `arg1 ? arg2 : arg3` zamiast instrukcji warunkowej `if`, gdyż jest on często zamieniany w instrukcję `cmov`, która jest szybsza od skoków. Istotne jest również to, w jakiej kolejności sprawdzamy warunki, najlepsze czasy osiągamy dla warunków, które używają posortowanych wartości, dzięki czemu IPC może przyjąć najwyższe wartości.

Jeśli działamy na danych zbliżonych (w pamięci) do siebie, to zwykle rozmiar tych danych (w tym przypadku tablicy) nie ma dużego znaczenia dla czasu działania programu.