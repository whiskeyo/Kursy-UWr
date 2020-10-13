# Raport do zadania 4

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
Po wielokrotnym uruchomieniu programu poniższym poleceniem z wariantem uruchamiającym jedynie funkcje `binsearch0` oraz z drugim, używającym najpierw funkcji `linearize`, a następnie `binsearch1`, uzyskałem następujące wyniki:
```
./binsearch -S 0x5bab3de5da7882ff -n 23 -t 24 -v ? -p memory
```
| `binsearch0` time | `binsearch0` L1 miss | `binsearch1` time | `binsearch1` L1 miss |
|:-----------------:|:--------------------:|:-----------------:|:--------------------:|
|     7.768515      |       82.597%        |     4.205586      |       39.443%        |
|     8.173084      |       83.029%        |     4.614501      |       39.482%        |
|     7.751434      |       82.720%        |     3.935228      |       39.395%        |
|     7.968169      |       82.864%        |     4.044639      |       39.395%        |
|     7.726586      |       82.594%        |     4.025995      |       39.403%        |
|     7.686544      |       82.665%        |     3.991741      |       39.420%        |
|     7.837241      |       82.683%        |     3.954296      |       39.395%        |
|     7.836710      |       82.686%        |     3.961317      |       39.394%        |
|     7.710297      |       82.718%        |     4.036921      |       39.401%        |
|     7.736745      |       82.818%        |     4.026276      |       39.392%        |
|     7.809438      |       82.584%        |     4.008276      |       39.402%        |
|     7.768493      |       82.786%        |     3.983749      |       39.390%        |
|     7.875437      |       82.837%        |     4.603778      |       39.499%        |
|     8.008497      |       82.846%        |     4.052600      |       39.402%        |
|     7.813215      |       82.705%        |     4.070600      |       39.408%        |
|     7.837456      |       82.695%        |     4.065093      |       39.401%        |
|     7.919109      |       82.660%        |     4.030052      |       39.401%        |
|     7.924794      |       82.684%        |     4.133944      |       39.412%        |
|     7.872445      |       82.622%        |     4.087060      |       39.402%        |
|     7.772880      |       82.572%        |     4.123675      |       39.404%        |
|     7.804444      |       82.591%        |     4.124329      |       39.445%        |
|     7.825818      |       82.609%        |     4.066676      |       39.435%        |
|     7.816928      |       82.629%        |     4.081348      |       39.410%        |
|     7.823159      |       82.507%        |     4.102174      |       39.409%        |
|     7.876315      |       82.555%        |     4.108880      |       39.436%        |
|     7.916667      |       82.590%        |     4.038127      |       39.406%        |
|     7.863818      |       82.618%        |     4.112822      |       39.408%        |
|     7.873652      |       82.663%        |     4.049187      |       39.412%        |
|     7.869413      |       82.773%        |     4.072543      |       39.400%        |
|     7.941776      |       82.599%        |     4.035240      |       39.410%        |
|     7.862077      |       82.739%        |     4.042493      |       39.395%        |
|     7.885559      |       82.689%        |     4.071397      |       39.400%        |
|     7.859903      |       82.881%        |     4.053050      |       39.412%        |
|     7.785656      |       82.798%        |     4.043751      |       39.403%        |
|     8.188886      |       82.979%        |     4.146242      |       39.408%        |
|     7.687717      |       82.622%        |     3.964749      |       39.387%        |
|     7.672423      |       82.667%        |     4.009761      |       39.391%        |
|     7.789842      |       82.572%        |     4.145798      |       39.387%        |
|     7.931540      |       82.751%        |     4.125088      |       39.403%        |
|     7.887219      |       82.762%        |     4.121889      |       39.401%        |
|     7.854677      |       82.697%        |     4.039901      |       39.409%        |
|     7.829657      |       82.712%        |     4.062558      |       39.406%        |
|     7.856532      |       82.838%        |     4.033574      |       39.400%        |
|     7.935443      |       82.716%        |     4.092072      |       39.408%        |
|     7.837738      |       82.646%        |     4.068786      |       39.404%        |
|     7.865419      |       82.709%        |     4.118168      |       39.411%        |
|     7.870503      |       82.725%        |     4.079259      |       39.408%        |
|     7.796498      |       82.796%        |     4.037448      |       39.434%        |
|     7.822900      |       82.689%        |     4.027489      |       39.427%        |
|     7.786519      |       82.759%        |     4.120540      |       39.409%        |

Średni czas wykonania programu korzystając z funkcji `binsearch0` wynosi 7.848s, a w przypadku funkcji `binsearch1` po zastosowaniu na tablicy `linearize`, jedynie 4.083s. Dwukrotnie spada, podobnie do czasu działania zoptymalizowanej funkcji, współczynnik chybień w L1. Dla łatwiejszego odczytania danych z tabeli sporządziłem dwa wykresy:
![](times.png)
![](misses.png)

Tak dużą poprawę wyników uzyskujemy dzięki zmianie organizacji danych. W przypadku funkcji `binsearch0` brakuje lokalności przestrzennej - środki przedziałów mogą leżeć w takich częściach tablicy, które nie znajdują się w pamięci cache, a duża część pobieranych danych nie zostaje do niczego wykorzystana. Zmiana organizacji funkcją `linearize` sprawia, że dane są przechowywane w tablicy w takiej postaci, jak w kopcu binarnym. Dzięki temu kolejne poziomy drzewa/kopca będą leżeć bardzo blisko siebie, szczególnie te najbliżej korzenia. W każdej iteracji odwołujemy się do poziomu niżej, więc dostępy do pamięci następują sekwencyjnie - w ten sposób zmniejszamy współczynnik chybień.

Kolejnym zadaniem jest sprawdzenie wpływu kolejności instrukcji na IPC. Rozważmy następujące sytuacje (przedstawiony kod zawarty jest w pętli) i porównajmy kody po deasemblacji (polecenie `objdump -r -d binsearch.o`), jak i czasy działania:

```c=
/* Sytuacja pierwsza */
int y = arr[i - 1];
long j = i * 2;

if (x == y)
    return true;

j = j | (x > y);
i = j;
```
```=
00000000000000b0 <binsearch1>:
  b0:	b8 01 00 00 00       	mov    $0x1,%eax
  b5:	eb 17                	jmp    ce <binsearch1+0x1e>
  b7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  be:	00 00 
  c0:	0f 9c c0             	setl   %al
  c3:	0f b6 c0             	movzbl %al,%eax
  c6:	48 09 c8             	or     %rcx,%rax
  c9:	48 39 f0             	cmp    %rsi,%rax
  cc:	7f 12                	jg     e0 <binsearch1+0x30>
  ce:	48 8d 0c 00          	lea    (%rax,%rax,1),%rcx
  d2:	39 54 87 fc          	cmp    %edx,-0x4(%rdi,%rax,4)
  d6:	75 e8                	jne    c0 <binsearch1+0x10>
  d8:	b8 01 00 00 00       	mov    $0x1,%eax
  dd:	c3                   	retq   
  de:	66 90                	xchg   %ax,%ax
  e0:	31 c0                	xor    %eax,%eax
  e2:	c3                   	retq   
```

```c=
/* Sytuacja druga */
int y = arr[i - 1];

if (x == y)
    return true;

long j = i * 2;
j = j | (x > y);
i = j;
```
```=
00000000000000b0 <binsearch1>:
  b0:	b8 01 00 00 00       	mov    $0x1,%eax
  b5:	eb 1c                	jmp    d3 <binsearch1+0x23>
  b7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  be:	00 00 
  c0:	48 01 c0             	add    %rax,%rax
  c3:	39 d1                	cmp    %edx,%ecx
  c5:	0f 9c c1             	setl   %cl
  c8:	0f b6 c9             	movzbl %cl,%ecx
  cb:	48 09 c8             	or     %rcx,%rax
  ce:	48 39 f0             	cmp    %rsi,%rax
  d1:	7f 15                	jg     e8 <binsearch1+0x38>
  d3:	8b 4c 87 fc          	mov    -0x4(%rdi,%rax,4),%ecx
  d7:	39 d1                	cmp    %edx,%ecx
  d9:	75 e5                	jne    c0 <binsearch1+0x10>
  db:	b8 01 00 00 00       	mov    $0x1,%eax
  e0:	c3                   	retq   
  e1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  e8:	31 c0                	xor    %eax,%eax
  ea:	c3                   	retq  
```

```c=
/* Sytuacja trzecia */
int y = arr[i - 1];
long j = i * 2;

j = j | (x > y);
i = j;

if (x == y)
    return true;
```
```=
00000000000000b0 <binsearch1>:
  b0:	b8 01 00 00 00       	mov    $0x1,%eax
  b5:	eb 0e                	jmp    c5 <binsearch1+0x15>
  b7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  be:	00 00 
  c0:	48 39 f0             	cmp    %rsi,%rax
  c3:	7f 23                	jg     e8 <binsearch1+0x38>
  c5:	8b 4c 87 fc          	mov    -0x4(%rdi,%rax,4),%ecx
  c9:	45 31 c0             	xor    %r8d,%r8d
  cc:	48 01 c0             	add    %rax,%rax
  cf:	39 d1                	cmp    %edx,%ecx
  d1:	41 0f 9c c0          	setl   %r8b
  d5:	4c 09 c0             	or     %r8,%rax
  d8:	39 d1                	cmp    %edx,%ecx
  da:	75 e4                	jne    c0 <binsearch1+0x10>
  dc:	b8 01 00 00 00       	mov    $0x1,%eax
  e1:	c3                   	retq   
  e2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  e8:	31 c0                	xor    %eax,%eax
  ea:	c3                   	retq  
```

$t_i$ oznacza czas wykonania programu z $i$-tą kolejnością instrukcji, a $ipc_i$ wskazuje ilość wykonanych instrukcji na cykl z $i$-tą kolejnością instrukcji. Ostatni rząd z pogrubionymi wartościami oznacza **średnią wartość** przeprowadzonych pomiarów.
|    $t_1$     |    $t_2$     |    $t_3$     |  $ipc_1$  |  $ipc_2$  |  $ipc_3$  |
|:------------:|:------------:|:------------:|:---------:|:---------:|:---------:|
|   4.408650   |   4.829462   |   4.256254   |   0.232   |   0.256   |   0.295   |
|   4.652591   |   4.761325   |   4.433678   |   0.225   |   0.259   |   0.290   |
|   4.349661   |   5.391079   |   4.525497   |   0.235   |   0.239   |   0.286   |
|   4.424461   |   5.527980   |   4.688947   |   0.232   |   0.235   |   0.280   |
|   4.736931   |   5.901021   |   4.140132   |   0.222   |   0.225   |   0.303   |
|   4.898210   |   6.039138   |   4.128417   |   0.222   |   0.220   |   0.303   |
|   5.228191   |   5.881151   |   4.212471   |   0.211   |   0.226   |   0.298   |
|   4.980397   |   5.385803   |   4.242080   |   0.217   |   0.240   |   0.297   |
|   4.764639   |   5.177456   |   4.234280   |   0.225   |   0.242   |   0.297   |
|   5.072025   |   5.086569   |   4.199244   |   0.213   |   0.247   |   0.299   |
| **4.751576** | **5.398098** | **4.306100** | **0.223** | **0.239** | **0.295** |

Te same dane przedstawione na wykresach:
![](tests.png)
![](testsipc.png)

Pomimo lekkiej rozbieżności danych w pomiarach (przeprowadzanych w takich samych warunkach) można śmiało stwierdzić, że najbardziej wydajną kolejnością instrukcji jest trzecia, a więc ta, w której na samym końcu sprawdzamy warunek `(x == y)`. Prawdopodobnie powodem takiego rozwiązania jest fakt, że instrukcja skoku służąca do opuszczenia pętli znajduje się na początku programu, w przeciwieństwie do pozostałych rozpatrywanych kolejności.

Wnioski
---
Pisząc programy powinniśmy bardzo zwracać uwagę na to, jak ułożone są dane, z których korzystamy. Musimy zachować jak największą lokalność przestrzenną i czasową, dzięki czemu będziemy odwoływać się do danych przechowywanych w pamięci podręcznej.

Czynnik IPC w dużej mierze zależy od tego w jakiej kolejności wykonywane są instrukcje. Na podstawie przeprowadzonych pomiarów możemy stwierdzić, że najlepiej umieszczać (o ile nie zmieni to wyniku programu) instrukcje warunkowe na samym końcu pętli, dzięki czemu instrukcja skoku służąca do opuszczenia pętli będzie umieszczana na początku programu - w ten sposób będzie on "pomijał" jeden skok, co przyczyni się do zoptymalizowania działania.