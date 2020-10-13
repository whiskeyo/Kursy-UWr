# Raport do zadania 5

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
Do zoptymalizowania programu wykorzystałem krzywą Mortona zaproponowaną jako jedną z możliwości rozwiązania. Zaimplementowałem ją w dwóch wersjach, wydajniejsza okazała się ta z użyciem czterech lookup-table ze wstępnie obliczonymi maskami, aby kosztem pamięci poprawić czas działania programu. 

Wstępnie jednak obliczałem wszystkie wartości na bieżąco - podejście to nie sprawdziło się, pomimo polepszenia lokalności (wskaźnik chybień na L1 spadł z około 70% na 7%, więc poprawa była bardzo zauważalna). Przy takim podejściu czas programu wydłużył się o około 50%. Kod tego podejścia wygląda następująco (jest on również w pliku `texture.c`):
```c=
static inline long index_1(long x, long y)
{
    static const uint64_t masks[6] = {
        0x5555555555555555,
        0x3333333333333333,
        0x0F0F0F0F0F0F0F0F,
        0x00FF00FF00FF00FF,
        0x0000FFFF0000FFFF,
        0x00000000FFFFFFFF
    };
    static const uint64_t shifts[6] = { 
        1, 2, 4, 8, 16, 32 
    };

    //x = (x | (x << shifts[5])) & masks[5];
    //x = (x | (x << shifts[4])) & masks[4];
    x = (x | (x << shifts[3])) & masks[3];
    x = (x | (x << shifts[2])) & masks[2];
    x = (x | (x << shifts[1])) & masks[1];
    x = (x | (x << shifts[0])) & masks[0];

    //y = (y | (y << shifts[5])) & masks[5];
    //y = (y | (y << shifts[4])) & masks[4];
    y = (y | (y << shifts[3])) & masks[3];
    y = (y | (y << shifts[2])) & masks[2];
    y = (y | (y << shifts[1])) & masks[1];
    y = (y | (y << shifts[0])) & masks[0];

    return x | (y << 1);
```

Przy tym podejściu wykonywane jest zbyt wiele operacji, co sprawia że działanie programu wydłuża się, zamiast skracać. 

Postanowiłem więc wykorzystywane maski zapisać w pamięci, aby pozbyć się niepotrzebnych operacji - posłużyły mi do tego 4 przygotowane wstępnie tabele z danymi. W ten sposób zamiast wykonywać aż 12 operacji bitowych na każdą współrzędną oraz dwóch na połączenie współrzędnych w wynik (łącznie 26 instrukcji), program wykonuje ich tylko 7. Współczynnik chybień na L1 w tej implementacji spadł do wartości bliskich 3%.

Dane w tabeli przedstawiają pomiary wykonane na wersji standardowej funkcji (`0`), na wersji bez lookup-table oznaczonej jako `1slow` oraz wersji z lookup-table oznaczonej jako `1fast`. Ostatnia (pogrubiona) linia wyników to średnia wszystkich pomiarów. Pomiary zostały przeprowadzone na parametrach `-S 0xdeadc0de -t 200000`.

|  Czas `0`  | Czas `1slow` | Czas `1fast` |  IPC `0`  | IPC `1slow` | IPC `1fast` |
|:----------:|:------------:|:------------:|:---------:|:-----------:|:-----------:|
|  4.902179  |   5.580350   |   3.987625   |   0.631   |    1.703    |    1.422    |
|  4.541540  |   5.367254   |   3.637588   |   0.645   |    1.712    |    1.450    |
|  4.537808  |   5.314241   |   3.655480   |   0.645   |    1.728    |    1.440    |
|  4.542547  |   5.367655   |   3.610233   |   0.644   |    1.712    |    1.454    |
|  4.539520  |   5.405635   |   3.628406   |   0.645   |    1.714    |    1.451    |
|  4.539755  |   5.317398   |   3.626307   |   0.645   |    1.727    |    1.445    |
|  4.541382  |   5.317069   |   3.637545   |   0.644   |    1.727    |    1.444    |
|  4.534346  |   5.353868   |   3.621871   |   0.645   |    1.715    |    1.450    |
|  4.531676  |   5.366275   |   3.642339   |   0.645   |    1.711    |    1.442    |
|  4.539828  |   5.363408   |   3.590413   |   0.645   |    1.714    |    1.456    |
|  4.532613  |   5.336224   |   3.631827   |   0.645   |    1.721    |    1.447    |
|  4.542537  |   5.342297   |   3.615743   |   0.645   |    1.720    |    1.452    |
|  4.535303  |   5.306759   |   3.651512   |   0.645   |    1.730    |    1.439    |
|  4.538074  |   5.336778   |   3.621979   |   0.645   |    1.720    |    1.444    |
|  4.539215  |   5.288618   |   3.637372   |   0.644   |    1.736    |    1.444    |
|  4.534073  |   5.329055   |   3.620446   |   0.645   |    1.724    |    1.451    |
|  4.535649  |   5.356147   |   3.638785   |   0.645   |    1.713    |    1.443    |
|  4.537542  |   5.305530   |   3.637412   |   0.645   |    1.730    |    1.441    |
|  4.543680  |   5.328209   |   3.635755   |   0.644   |    1.724    |    1.443    |
|  4.543172  |   5.351518   |   3.630656   |   0.644   |    1.715    |    1.446    |
|  4.541172  |   5.311202   |   3.636872   |   0.645   |    1.728    |    1.443    |
|  4.554994  |   5.359288   |   3.615155   |   0.644   |    1.715    |    1.449    |
|  4.537535  |   5.323983   |   3.608381   |   0.645   |    1.725    |    1.453    |
|  4.540126  |   5.355486   |   3.626732   |   0.644   |    1.714    |    1.447    |
|  4.540927  |   5.334238   |   3.636206   |   0.645   |    1.721    |    1.444    |
|  4.541277  |   5.362970   |   3.621554   |   0.644   |    1.712    |    1.449    |
|  4.673747  |   5.712231   |   3.604830   |   0.640   |    1.702    |    1.452    |
|  4.590923  |   5.374809   |   3.642371   |   0.643   |    1.721    |    1.442    |
|  4.586392  |   5.417296   |   3.621215   |   0.643   |    1.707    |    1.450    |
|  4.587531  |   5.408979   |   3.620946   |   0.643   |    1.711    |    1.450    |
| **4.5609** |  **5.3665**  |  **3.6398**  | **0.644** |  **1.718**  |  **1.446**  |

Dane te przedstawiłem również na wykresie:
![](tests.png)

Warto zwrócić uwagę na to, że nie zawsze wyższy współczynnik IPC oznacza szybszy czas działania programu - dla funkcji `index1slow` wykonywane jest wiele instrukcji w trakcie jednego cyklu, jednak samych cykli jest znacznie więcej niż w przypadku pozostałych implementacji. Najlepiej zachowuje się funkcja `index1fast`, która działa o około 25% szybciej od standardowej implementacji i o około 48% szybciej od implementacji niewykorzystującej tablicowania. Wynika stąd, że translacja adresów ma zauważalny wpływ na wydajność przeglądania tekstury.


Wnioski
---
Pisząc programy powinniśmy zwrócić uwagę na sposób odwoływania się do pamięci, gdyż rozsądne przechodzenie po danych może znacznie skrócić czas działania programu. Należy jednak przy tym uważać, gdyż dla dużych danych (tak jak w przypadku tego zadania) naiwna implementacja krzywej Mortona może spowolnić działanie programu, dlatego warto poświęcić trochę pamięci na przechowanie danych o wykorzystywanych maskach, aby nie obliczać tych samych wartości wiele razy.
