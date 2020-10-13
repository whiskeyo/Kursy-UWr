# Raport do zadania 2

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
Korzystając z polecenia `make test` przeprowadziłem kilkukrotnie testy, a następnie czasy i współczynniki chybień uśredniłem, wyniki zapisałem w tabeli poniżej:

|  BLOCK SIZE  | TIME  | L1 miss % | L2 miss % | L3 miss % | TLB miss % |
|:------------:|:-----:|:---------:|:---------:|:---------:|:----------:|
| `transpose0` | 2.533 |  53.143   |  111.110  |  110.955  |   0.001    |
|      1       | 2.559 |  53.142   |  111.236  |  111.010  |   0.001    |
|      2       | 1.548 |  22.521   |  49.055   |  48.911   |   0.001    |
|      4       | 0.734 |  12.500   |  25.440   |  25.262   |   0.000    |
|      8       | 0.407 |   9.598   |  20.588   |  17.101   |   0.002    |
|      16      | 1.387 |  39.980   |  136.980  |  21.543   |   0.001    |
|      32      | 1.060 |  51.570   |  133.381  |  15.098   |   0.001    |
|      64      | 1.060 |  52.390   |  129.512  |  11.982   |   0.002    |
|     128      | 2.284 |  53.191   |  110.060  |  96.918   |   0.001    |
|     256      | 2.592 |  53.141   |  112.006  |  109.878  |   0.002    |
|     512      | 2.587 |  53.141   |  111.241  |  110.352  |   0.001    |

Wynik dla funkcji `transpose0` jest wprowadzony jednokrotnie, jako że jej implementacja nie korzysta w żaden sposób z kafelków, kolejne wartości odpowiadają już funkcji `transpose1` działającej na kafelkach. Z pomiarów łatwo zobaczyć, że najbardziej optymalną wielkością kafelka jest 8, jako że czas transponowania macierzy jest wtedy ponad sześciokrotnie krótszy, niż w przypadku najmniej optymalnych wyborów. Spowodowane jest to stosunkowo rzadkimi dostępami do pamięci w stosunku do rozmiarów mniejszych, jak i większych.
![wykres1](plot1.png)

Eksperymentalnie możemy również określić złożoność algorytmu transpozycji macierzy, jest to $O(n^2)$, wyniki przedstawiłem poniżej:

|       n        |    8     |    16    |    32    |    64    |   128    | 256       |
|:--------------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------: |
|    **time**    | 0.000037 | 0.000037 | 0.000038 | 0.000061 | 0.000088 | 0.000142  |
| **L1 miss %**  |  12.879  |  10.302  |  7.618   |  6.589   |  8.781   | 8.547     |
| **L2 miss %**  |  37.689  |  25.598  |  14.827  |  12.147  |  11.322  | 20.340    |
| **L3 miss %**  |  14.394  |  8.221   |  3.642   |  1.426   |  0.424   | 0.159     |
| **TLB miss %** |  7.675   |  4.579   |  1.672   |  0.541   |  0.521   | 0.306     |
|     **n**      | **512**  | **1024** | **2048** | **4096** | **8192** | **16384** |
|    **time**    | 0.000652 | 0.002412 | 0.007344 | 0.027459 | 0.113813 | 0.414101  |
| **L1 miss %**  | 8.810    | 25.647   | 11.301   | 10.286   | 9.738    | 9.586     |
| **L2 miss %**  | 30.531   | 27.456   | 17.725   | 17.482   | 19.138   | 20.628    |
| **L3 miss %**  | 3.974    | 5.097    | 15.671   | 16.739   | 18.080   | 17.167    |
| **TLB miss %** | 0.524    | 0.245    | 0.051    | 0.022    | 0.007    | 0.042     |

![wykres2](plot2.png)

Tabelka oraz ostatni wykres mogą posłużyć do aproksymacji rozmiaru poszczególnych poziomów pamięci podręcznej, wyraźnie widzimy, że wartości na wykresie tworzą schodki, które w przybliżeniu pokazują, jak duże są rozmiary pamięci podręcznych.

| n-size |   TIME   | L1 miss % | L2 miss % | L3 miss % | TLB miss % |
|:------:|:--------:|:---------:|:---------:|:---------:|:----------:|
|   8    | 0.000039 |  13.068   |  38.447   |  17.235   |   8.523    |
|   16   | 0.000040 |  10.302   |  14.880   |   8.221   |   5.619    |
|   24   | 0.000041 |   8.502   |  20.036   |   8.026   |   3.151    |
|   32   | 0.000039 |   7.621   |  15.779   |   4.349   |   1.710    |
|   40   | 0.000039 |   7.172   |  14.243   |   1.805   |   1.153    |
|   48   | 0.000043 |   7.088   |  13.207   |   1.615   |   0.897    |
|   56   | 0.000045 |   6.890   |  12.409   |   1.934   |   0.886    |
|   64   | 0.000045 |   6.599   |  12.168   |   1.655   |   0.541    |
|   72   | 0.000044 |   6.627   |  11.878   |   1.145   |   0.456    |
|   80   | 0.000049 |   6.280   |  11.600   |   1.055   |   0.466    |
|   88   | 0.000043 |   6.404   |  10.658   |   0.460   |   0.376    |
|   96   | 0.000064 |   6.166   |  10.236   |   0.308   |   0.374    |
|  104   | 0.000085 |   6.138   |  10.828   |   0.485   |   0.174    |
|  112   | 0.000090 |   5.891   |  10.943   |   0.269   |   0.245    |
|  120   | 0.000084 |   6.065   |  10.714   |   0.265   |   0.216    |
|  128   | 0.000089 |   8.835   |  11.161   |   0.242   |   0.556    |
|  136   | 0.000091 |   6.050   |  12.110   |   0.438   |   0.278    |
|  144   | 0.000092 |   5.799   |  10.696   |   0.157   |   0.179    |
|  152   | 0.000096 |   6.009   |  11.518   |   0.162   |   0.223    |
|  160   | 0.000095 |   6.201   |  11.072   |   0.167   |   0.272    |
|  168   | 0.000098 |   5.987   |  12.171   |   0.207   |   0.142    |
|  172   | 0.000100 |   5.762   |  11.553   |   0.180   |   0.057    |
|  184   | 0.000101 |   6.000   |  11.803   |   0.208   |   0.176    |
|  192   | 0.000103 |   8.776   |  12.109   |   0.122   |   0.359    |
|  200   | 0.000109 |   6.144   |  12.233   |   0.158   |   0.236    |
|  208   | 0.000108 |   5.933   |  11.507   |   0.211   |   0.222    |
|  216   | 0.000111 |   6.392   |  12.126   |   0.180   |   0.234    |
|  224   | 0.000128 |   8.035   |  12.276   |   0.171   |   0.312    |
|  232   | 0.000135 |   6.164   |  11.938   |   0.258   |   0.241    |
|  240   | 0.000134 |   6.005   |  11.292   |   0.141   |   0.208    |
|  248   | 0.000137 |   6.156   |  11.174   |   0.181   |   0.212    |
|  256   | 0.000141 |   8.555   |  19.293   |   0.232   |   0.384    |
|  264   | 0.000142 |   6.289   |  11.229   |   0.203   |   0.221    |
|  272   | 0.000155 |   6.161   |  11.401   |   1.303   |   0.222    |
|  280   | 0.000154 |   6.984   |  11.273   |   0.826   |   0.242    |

![wykres3](plot3.png)

Pierwszy, dosyć spory skok między czasami wykonania widzimy między rozmiarem 88x88 (30 KiB) a 96x96 (36 KiB). Spowodowane jest to koniecznością odwoływania się do kolejnego poziomu pamięci, a więc L1 ma 32 KiB. Następny skok jest w pomiędzy macierzami 216x216 (182 KiB) a 264x264 (272 KiB). Dokładne określenie rozmiaru L2 w tym przypadku jest już bardziej problematyczne, gdyż zakres rozmiarów jest znacznie większy, niż w poprzednim przypadku, jednak wiedząc, że bardzo często pamięci podręczne są potęgami dwójki, możemy założyć, iż rozmiar L2 wynosi 256 KiB. Podobne pomiary można przeprowadzić dla większych rozmiarów macierzy, aby wyznaczyć rozmiar L3, jednak zakres czasów prawdopodobnie zwiększyłby się jeszcze bardziej, niż w przypadku L2.

Wnioski
---

W przypadku funkcji `transpose1` rozmiar kafelka ma bardzo duże znaczenie, podobnie jak w zadaniu z mnożeniem macierzy. Jego optymalnym rozmiarem jest 8.

Na podstawie czasów działania programu możemy w przybliżeniu ocenić rozmiar pamięci podręcznej procesora, jednak jest to dosyć żmudny proces, w szczególności dla trzeciego poziomu.