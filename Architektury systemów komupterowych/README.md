# Architektury systemów komputerowych

Cały dokument widoczny poniżej znajduje się [na HackMD](https://hackmd.io/@whiskeyo/B1KrUn6sI) w znacznie czytelniejszej wersji (niestety GitHub nie uwzględnia formatowania MathMode).

## Kilka słów ode mnie
Wszystkie notatki w dużej mierze opierają się na CS:APP, tłumaczę najważniejsze fragmenty poszczególnych rozdziałów - z tego powodu mogą się znaleźć tu pewne braki, jednak nie powinno być ich za dużo.

Staram się unikać kopiowania przykładów z ww. książki, jako że duża część list zadań jest na nich oparta. Rozwiązania poszczególnych list z semestru letniego 2019/2020 znajdziesz tutaj:


| Numery list |                           Pokrywane tematy                            |                   Linki do rozwiązań                    |
|:-----------:|:---------------------------------------------------------------------:|:-------------------------------------------------------:|
|    0 - 3    |                 reprezentacje liczb, operacje bitowe                  | [Rozwiązania](https://hackmd.io/kkZXyywDSgWvQOxPk79yLQ) |
|    4 - 9    |                       machine-level programming                       | [Rozwiązania](https://hackmd.io/1hSJrFY0SRKpojY5Uh6s-Q) |
|   10 - 12   | pamięć operacyjna, pamięć dyskowa, pamięć podręczna, pamięć wirtualna | [Rozwiązania](https://hackmd.io/JQTlC9GbRZW-_MFXZhgtWA) | 
|      -      |        optymalizacja programów (z CS:APP, Ch. 5)                      | [Rozwiązania](https://hackmd.io/SqwUGdHtRuG3rKC6yr_11w) |

Znaczna większość rozwiązań powinna być poprawna, jednak do kilku z nich miałem pewne wątpliwości - dlatego zastanówcie się dwa razy, czy to co widzicie ma sens. 

Rozwiązania pracowni znajdują się poniżej (kody wrzucę za jakiś czas):

|     Pracownia     |                    Raport/Wyjaśnienie/Kod                |
|:-----------------:|:--------------------------------------------------------:|
|     `bitrev`      |                            -                             |
|      `addsb`      |                            -                             |
|       `clz`       |                            -                             |
|      `mod17`      |                            -                             |
|      `cubef`      |                            -                             |
|     `matmult`     | [sprawozdanie](https://hackmd.io/h8Qgt3ElR6yPOrhrCYSxgg) |
|    `transpose`    | [sprawozdanie](https://hackmd.io/jDVScT24SgS3Kyb5u2JjwQ) |
|    `randwalk`     | [sprawozdanie](https://hackmd.io/pAK9cgkVQ3WfUzbp0T3AbQ) |
|    `binsearch`    | [sprawozdanie](https://hackmd.io/HC1zb03bQ-KRb03PUDZPhg) |
|     `texture`     | [sprawozdanie](https://hackmd.io/d1WmmKNlSWyfj86d85w6JQ) |
| rozbrajanie bomby |    [how to](https://hackmd.io/YUY1mM0ZTDio1JjI87w76g)    |
|   atak hakerski   |    [how to](https://hackmd.io/4baJaicYTCKGNYz_JGU5jQ)    |

Gdybyście zauważyli jakieś błędy, to skontaktujcie się ze mną w jakikolwiek sposób, jednak najlepiej będzie mailowo: duchowe50k@gmail.com.

Miłego czytania :)

## Linkowanie
**Linkowanie** *(ang. linking)* to proces wybierania i łączenia różnych części kodu i danych do jednego pliku, który może zostać załadowany do pamięci i wykonany. Może być ono przeprowadzane w trakcie:
* kompilacji ***(ang. compile time)***, czyli w momencie gdy kod jest tłumaczony na kod maszynowy,
* ładowania programu ***(ang. load time)***, czyli w momencie ładowaniu programu do pamięci i uruchamiania go,
* uruchamiania programu ***(ang. run time)***.

Linkowanie jest przeprowadzane automatycznie przez programy zwane **linkerami** *(ang. linkers)*. Grają one dużą rolę w software developmencie, gdyż umożliwiają tworzenie programów złożonych z modułów, zamiast tworzenia go w jednym dużym pliku.

### Działanie kompilatora
Załóżmy, że mamy dwa pliki, `main.c` oraz `sum.c`:
```c=
/* main.c */
int sum(int *a, int n);

int array[2] = {1, 2};

int main() {
    int val = sum(array, 2);
    return val;
}
```
```c=
/* sum.c */
int sum(int *a, int n) {
    int i, s = 0;
    for (i = 0; i < n; i++)
        s += a[i];
        
    return s;
}
```

Aby skompilować taki program, musimy wywołać kompilator GCC poleceniem:
```
$ gcc -Og -o prog main.c sum.c
```
Kompilator w pierwszym kroku uruchamia preprocessor C (`cpp`), który tłumaczy plik `main.c` na pośredni plik ASCII `main.i`, a następnie kompilator (`cc1`) tłumaczy ten plik na plik w języku assembly `main.s`. Później uruchamiany jest assembler (`as`), który tłumaczy `main.s` do binarnego **pliku relokowalnego** *(ang. relocatable object file)* `main.o`. W taki sam sposób generowany jest plik `sum.o`. Na samym końcu uruchamiany jest linker (`ld`), który łączy pliki `*.o` tworząc **plik wykonywalny** *(ang. executable object file)*. Taki plik uruchamiamy poleceniem
```
$ ./prog
```
Powłoka systemu *(ang. shell)* wywołuje systemową funkcję zwaną loaderem, która kopiuje kod i dane do wykonywalnego pliku `prog` do pamięci, a następnie przenosi kontrolę na początek programu.

### Linkowanie statyczne
**Linkowanie statyczne** *(ang. static linking)* polega na wzięciu przez linker (taki jak `ld`) zbioru plików relokowalnych i argumentów, a następnie wygenerowaniu w pełni zlinkowanego pliku wykonywalnego, który może zostać uruchomiony. 

Wejściowe pliki relokowalne składają się z kodu i różnych sekcji danych, przy czym każda z nich ze sobą sąsiaduje i jest ciągiem bajtów. Instrukcje są w jednej sekcji, zainicjalizowane zmienne globallne w innej, a niezainicjalizowane zmienne globalne w jeszcze innej sekcji.

Zbudowanie pliku wykonywalnego składa się z dwóch kroków:
1. **analiza symboli** *(ang. symbol resolution)* - pliki obiektowe definiują i odwołują się do symboli, a każdy z nich może być powiązany z nazwą funkcji, zmiennej globalnej lub zmiennej `static`. Celem analizy symboli jest jednoznaczne powiązanie referencji z definicją.
2. **relokacja** *(ang. relocation)* to proces polegający na łączeniu modułów wejściowych i nadawaniu adresów wykonania wszystkim symbolom. Przebiega on w dwóch krokach: relokacja sekcji i definicji symboli a następnie relokacja referencji do innych symboli.

### Pliki obiektowe
**Pliki obiektowe** *(ang. object files)* występują w jednej z trzech form:
* **plik relokowalny** *(ang. relocatable object file)* - zawiera kod binarny i dane w formie umożliwiającej dalsze połączenie z innymi plikami relokowalnymi w celu stworzenia pliku wykonywalnego,
* **plik wykonywalny** *(ang. executable object file)* - zawiera kod binarny i dane w formie umożliwiającej skopiowanie kodu bezpośrednio do pamięci i wykonanie go,
* **biblioteka współdzielona** *(ang. shared object file)* - specjalny rodzaj pliku relokowalnego, który może być ładowany do pamięci w trakcie ładowania, jak i uruchamiania programu.

Kompilatory i assemblery generują pliki relokowalne (włącznie z bibliotekami współdzielonymi), linkery generują pliki wykonywalne. 

Pliki obiektowe są zorganizowane zgodnie ze specyfikacją, które mogą różnić się w zależności od systemu. Nowoczesne systemy x86-64 Linux i Unix używają **ELF** *(ang. Executable and Linkable Format)* i na tym formacie skupimy się najwięcej, jednak pomysł jest podobny we wszystkich przypadkach, np. Windowsowym **PE** *(ang. Portable Executable)*, czy OS-X'owym **Mach-O**.

### Pliki relokowalne
**Plik relokowalny** *(ang. relocatable object file)* w formacie ELF wygląda w taki sposób:
![](https://i.imgur.com/CO7tIjt.png)
Rozpoczyna się on 16-bajtową sekwencją zwaną **ELF header** opisującą **wielkość słowa** i **kolejność bajtów** *(ang. endianness)* systemu, który wygernerował plik. ELF header zawiera również informacje umożliwiające linkerowi spasowanie i zinterpetowanie pliku obiektowego - są to między innymi typ pliku obiektowego, rodzaj architektury, czy rozmiar i liczbę sekcji.

Zwykle plik ELF składa się z następujących sekcji:


| Sekcja      | Przeznaczenie |
| ----------- | ------------- |
| `.text`     | Kod maszynowy skompilowanego programu. |
| `.rodata`   | Dane tylko do odczytu, np. sekwencje znaków niezmienialnych `string` używanych w funkcji `printf` oraz tabele skoków dla instrukcji `switch`.              |
| `.data`     | Zainicjalizowane zmienne globalne oraz `static`. Lokalne zmienne są trzymane na stosie w trakcie uruchomienia programu (run time) i nie pojawiają się ani w sekcji `.data`, ani w `.bss`.              |
| `.bss`      | Niezainicjalizowane zmienne globalne oraz `static`, włącznie ze zmiennymi zainicjalizowanymi na 0, ta sekcja nie zajmuje miejsca w pliku relokowalnym, jest jedynie placeholderem.              |
| `.symtab`   | Tabela symboli z informacją o funkcjach i zmiennych globalnych, które są zdefiniowane i do których odwołuje się w programie.              |
| `.rel.text` | Lista lokalizacji z sekcji `.text`, która zostanie zmodyfikowana w trakcie linkowania tego pliku relokowalnego z innymi. W ogólności instrukcje, które wywołują "zewnętrzne" *(ang. external)* funkcje lub odwołują się do zmiennych globalnych, będą musiały być zmodyfikowane, jednak funkcje wywoływane lokalnie nie muszą być modyfikowane.            |
| `.rel.data` | Informacje o relokacji dla zmiennych globalnych, do których występują odwołania lub są zdefiniowane w tym module. W ogólności są to zainicjalizowane zmienne globalne, których wartością początkową jest adres oraz funkcje zdefiniowane "zewnętrznie".              |
| `.debug`    | Lista symboli do debugowania, zawiera lokalne zmienne i definicje typów (`typedef`), zmienne globalne zdefiniowane przez program i te, do których się odwołuje, jak i oryginalny kod źródłowy w języku C. Aby sekcja ta powstała, należy skompilować program z flagą `-g`.              |
| `.line`     | Mapowanie pomiędzy liniami z pliku źródłowego a instrukcjami kodu maszynowego z sekcji `.text`, podobnie pojawia się po użyciu flagi `-g`.              |
| `.strtab`   | Tablica `string`ów dla sekcji tabeli symboli `.symtab` oraz `.debug`, jak i nazw sekcji w nagłówkach.               |

### Symbole i tabele symboli
Każdy **relokowalny moduł** *(ang. relocatable object module)* $m$ posiada tabelę symboli zawierającą informacje o symbolach, które są definiowane i do których odwołuje się $m$. W kontekście linkera, są trzy rodzaje symboli:
* **symbole globalne definiowane przez $m$**, do których mogą się odnośić inne moduły, odpowiadają temu zmienne globalne i funkcje `nonstatic`
* **symbole globalne do których odwołuje się $m$**, ale są zdefiniowane przez inne moduły, są to symbole zewnętrzne, a odpowiadają im zmienne globalne i funkcje zdefiniowane w innych modułach
* **symbole lokalne** które są zdefiniowane i do których odwołuje się $m$, odpowiadają temu funkcje, jak i zmienne globalne z atrybutem `static` - zmienne te są widoczne wszędzie w $m$, jednak nie mogą się do nich odwoływać inne moduły.

Ważne jest rozróżnienie symboli od zmiennych programu. Tabela symboli `.symtab` nie zawiera symboli, które współdziałają z lokalnymi zmiennymi `nonstatic`, gdyż są one tworzone w trakcie uruchamiania programu na stosie.

Lokalne zmienne funkcji, które są zdefiniowane z atrybutem `static` o dziwo nie są wpisywane na stos, kompilator alokuje miejsce dla nich w sekcji `.data` lub `.bss` i dla każdego lokalnego symbolu linker tworzy symbol z unikalną nazwą. Załóżmy, że mamy dwie funkcje:
```c=
int f() {
    static int x = 0;
    return x;
}

int g() {
    static int x = 1;
    return x;
}
```
Kompilator w takiej sytuacji eksportuje do assemblera dwa symbole lokalne z różnymi nazwami, np. może użyć `x.1` dla definicji `f` oraz `x.2` dla definicji `g`.

Tabele symboli tworzone są przez assembler, używając symboli z pliku `.s` wygenerowanego przez kompilator. **Tabela symboli ELF** zawarta jest w sekcji `.symtab`, a ona sama zawiera tablicę wpisów:
```c=
typedef struct {
    int    name;        // Przesunięcie bajtów kierujące do nazwy symbolu
    char   type:4,      // Funkcja czy dane (4 bity)
           binding:4;   // Lokalny czy globalny (4 bity)
    char   reserved;    // Wartość nieużywana
    short  section;     // Indeks sekcji nagłówkowej 
    long   value;       // Adres symbolu 
    long   size;        // Rozmiar obiektu w bajtach 
} Elf64_Symbol;
```
Każdy symbol jest przypisany do którejś sekcji w pliku obiektowym.

Istnieją również trzy pseudosekcje, które nie mają wpisów: **`ABS`** jest przeznaczona dla symboli, które nie powinny być relokowane, **`UNDEF`** dla niezdefiniowanych symboli, tzn. symboli do których odwołuje się moduł, ale nie będących zdefiniowanymi oraz **`COMMON`** dla niezainicjalizowanych obiektów, które nie są jeszcze zaalokowane.

Pomiędzy `COMMON` a `.bss` jest delikatna różnica: `COMMON` zawiera niezainicjalizowane zmienne globalne, a `.bss` niezainicjalizowane zmienne `static` oraz zmienne globalne i `static` zainicjalizowane na 0.

Sekcje można przeglądać za pomocą programu (jak i polecenia) `readelf` z odpowiednimi flagami.

### Analiza symboli
Linker analizuje symbole poprzez przypisanie dokładnie jednej definicji symbolu z tabeli symboli do odpowiadającego pliku relokowalnego.

Analiza symboli jest prosta w przypadku referencji do symboli lokalnych zdefiniowanych w tym samym module, w którym znajduje się ta referencja. Kompilator zezwala na tylko jedną definicje każdego symbolu lokalnego na moduł, upewnia się również czy zmienne `static` mają różne nazwy.

Rozstrzygnięcie odwołań do symboli globalnych jest jednak bardziej zawiłe. W trakcie gdy kompilator napotyka symbol (zmienną lub funkcję), która nie jest zdefiniowana w aktualnym module, zakłada że jest on zdefiniowany w jakimś innym module, generując przy tym wpis w tabeli symboli i zostawiając go dla linkera. Gdy linker nie jest w stanie znaleźć definicji dla tego symbolu, zwraca on błąd. Przykład kodu, polecenia kompilacji oraz zwracanego błędu można zobaczyć poniżej:
```c=
/* linkerror.c */
void foo();

int main() {
    foo();
    return 0;
}
```
```
$ gcc -Wall -Og -o linkerror linkerror.c
/tmp/ccSz5uti.o: In function ‘main’:
/tmp/ccSz5uti.o(.text+0x7): undefined reference to ‘foo’
```

Analiza dla symboli globalnych jest również utrudniona, jako że kilka modułów może definiować zmienne globalne o tej samej nazwie. W tym przypadku linker musi zgłosić błąd lub w jakiś sposób wybrać najbardziej odpowiednią definicję. Zwykle do linkera wysyłany jest zbiór plików relokowalnych, każdy z nich definiuje zbiór symboli, przy czym niektóre z nich są lokalne, a niektóre globalne. 

W przypadku powielonej definicji symbolu globalnego, systemy kompilujące pod Linuxem rozstrzygają czy symbol jest silny, czy słaby. **Symbolem silnym** są funkcje i zainicjalizowane zmienne globalne, a **symbolami słabymi** są niezainicjalizowane zmienne globalne. Wybór symbolu bardziej odpowiedniego opiera się na trzech zasadach:
1. Nie dopuszcza się wielu silnych symboli o tej samej nazwie.
2. W przypadku jednego silnego symbolu i kilku słabych o tej samej nazwie, wybierz silny.
3. W przypadku wielu słabych symboli wybierz dowolny.

### Linkowanie bibliotek statycznych
Wszystkie systemy kompilacji zapewniają mechanizm pakowania pokrewnych modułów do jednego pliku zwanego **biblioteką statyczną**. W trakcie budowania pliku wykonywalnego, linker kopiuje TYLKO wybrane moduły z biblioteki, dokładniej te, do których odnosi się program. Jedną z takich bibliotek jest `libc.a`, zapewnia ona funkcje `atoi`, `printf`, `scanf` i wiele innych, a inną `libm.a` z wieloma funkcjami matematycznymi.

Jednym ze sposobów linkowania jej byłoby wydzielenie kompilatorowi zadania "wyłuskania" wywołań funkcji i wygenerowanie kodu bezpośrednio po tym - taki sposób jednak nie sprawdziłby się w przypadku tak rozwiniętego języka jak C, gdyż każda zmiana biblioteki wymagałaby nowej wersji kompilatora.

Innym sposobem byłoby zebranie wszystkich funkcji w jeden plik relokowalny `libc.o`, dzięki czemu moglibyśmy linkować go za każdym razem, gdy mamy taką potrzebę. Dużą wadą tego rozwiązania jest duży rozmiar nawet bardzo małych programów, gdyż za każdym razem kopiowanoby około 5MB danych (nawet w przypadku "Hello, world!"). Kolejną wadą jest konieczność rekompilacji całości pliku, nawet przy minimalnej zmianie w kodzie, co jest ogromną stratą czasu.

Biblioteki statyczne powstały po to, aby rozwiązać problemy płynące z powyższych sposobów: w trakcie linkowania, linker kopiuje jedynie moduły potrzebne programowi, dzięki czemu zmniejsza rozmiar pliku wykonywalnego. Są one przechowywane w archiwach będących kolekcją plików relokowalnych, a ich rozszerzeniem jest `.a`. Do operacji na archiwach można użyć programu `ar`.

Załóżmy, że z plików `addvec.c` oraz `multvec.c` tworzymy bibliotekę statyczną `libvector.a`:
```
$ gcc -c addvec.c multvec.c
$ ar rcs libvector.a addvec.o multvec.o
```
Następnie tworzymy program w pliku `main2.c`:
```c=
#include <stdio.h>
#include "vector.h" // zawiera definicje prototypów z libvector.a

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int main() {
    addvec(x, y, z, 2); // dodaj wektory x, y rozmiaru 2, zapisz w z
    printf("z = [%d %d]\n", z[0], z[1]);
}
```
Kompilujemy go poleceniem (druga i trzecia linijka są równoznaczne):
```
$ gcc -c main2.c
$ gcc -static -o prog2c main2.o ./libvector.a
$ gcc -static -o prog2c main2.o -L. -lvector
```
Schemat linkowania biblioteki statycznej w tym przypadku wygląda następująco:
![](https://i.imgur.com/sFd617e.png)

### Relokacje
**Relokacja** *(ang. relocation)* jest kolejnym krokiem po analizie symboli podejmowanym przez linker. Proces ten polega na połączeniu modułów wejściowych i przypisaniu adresów wykonania wszystkim symbolom. Relokacja składa się z dwóch kroków:
1. **Relokacja sekcji i definicji symboli**: linker łączy wszystkie sekcje tego samego typu w jedną sekcję tego typu, następnie przypisuje adresy pamięci do nowych sekcji. Po ukończeniu tego kroku, każda instrukcja i zmienna globalna w programie ma unikalny adres wykonania.
2. **Relokacja referencji do innych symboli**: linker modyfikuje wszystkie referencje do symboli w ciele kodu i sekcjach danych, aby wskazywały poprawne adresy wykonania. Aby przeprowadzić ten krok, linker polega na strukturach danych w modułach plików relokowalnych znanych jako **wpisy relokacji** *(ang. relocation entries)*.

W trakcie gdy assembler napotka na referencję do obiektu, którego ostateczna lokalizacja nie jest znana, stworzy **wpis relokacji** mówiący linkerowi o tym, jak zmodyfikować tę referencję w trakcie łączenia pliku obiektowego w plik wykonywalny. Wpisy relokacji dla kodu są umieszczone w sekcji `.rel.text`, a dla danych w `.rel.data`. Poniżej przedstawiono strukturę wpisu relokacji ELF:
```c=
typedef struct {
    long offset;    // offset referencji do relokacji
    long type:32,   // typ relokacji
         symbol:32; // indeks z tabeli symboli
    long addend;    // stała część wyrażenia relokacji
} Elf64_Rela;
```
Mimo, że ELF rozróżnia 32 typy relokacji, skupimy się tylko na dwóch podstawowych:
* **R_X86_64_PC32** relokuje referencje do adresów, które są obliczane na podstawie przesunięcia aktualnej wartości PC o dany offset, a aktualna wartość PC to zawsze adres następnej instrukcji w pamięci.
* **R_X86_64_32** relokuje referencje do adresów, które są absolutne, czyli bierzemy surowy adres zakodowany w instrukcji i przekształcamy na efektywny bez modyfikowania go w żaden sposób.  

Relokowanie referencji symboli odbywa się na podstawie poniższego algorytmu. Zakładamy, że podczas algorytmu linker już wybrał adresy dla każdej sekcji `(ADDR(s))` i każdego symbolu `(ADDR(r.symbol))`.
```text=
// s to tablica bajtów, r to struktura typu Elf64_rela
foreach section s {
    foreach relocation entry r {
        refptr = s + r.offset; /* wskaźnik do referencji do relokacji */

        /* relokacja referencji PC-relative */
        if (r.type == R_X86_64_PC32) {
            refaddr = ADDR(s) + r.offset; /* adres wykonania ref’a */
            *refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr);
        }

        /* relokacja referencji absolutnej */
        if (r.type == R_X86_64_32)
            *refptr = (unsigned) (ADDR(r.symbol) + r.addend);
    }
}
```

Dla zdefiniowanych wcześniej plików `main.c` oraz `sum.c` (sekcja *Działanie kompilatora*) i wygenerowanego pliku `main.o`, możemy za pomocą programu `objdump` sprawdzić, jak został wykonany powyżej przedstawiony algorytm w zdesasemblowanym kodzie. Użyjmy polecenia:
```
objdump -dx main.o
```
i przeanalizujmy uzyskany wydruk:
```text=
0000000000000000 <main>:
    0: 48 83 ec 08         sub $0x8,%rsp
    4: be 02 00 00 00      mov $0x2,%esi
    9: bf 00 00 00 00      mov $0x0,%edi             %edi = &array
                        a: R_X86_64_32 array         wpis relokacji
    e: e8 00 00 00 00      callq 13 <main+0x13>      wywołanie sum()
                        f: R_X86_64_PC32 sum-0x4     wpis relokacji
   13: 48 83 c4 08         add $0x8,%rsp
   17: c3                  retq
```

### Ładowanie plików wykonywalnych
Uruchomienie pliku wykonywalnego `prog` odbywa się przy użyciu polecenia:
```
$ ./prog
```
Powłoka systemu nie wykryje żadnego powiązania z wbudowanym poleceniem, a więc założy, że `prog` jest plikiem wykonywalnym, który za pomocą loadera zostanie wczytany. Każdy program Linuxowy może wywołać loader wywołując funkcję `execve`. Loader kopiuje kod i dane z dysku do pamięciu, a następnie rozpoczyna program poprzez wykonanie skoku do pierwszej instrukcji lub do **punktu wejścia** *(ang. entry point)*.

Każdy program działa na pamięci podobnej do przedstawionej na poniższym schemacie. Na systemach Linux x86-64, segment kodu zaczyna się na adresie `0x400000`, a następnie mamy segment danych. Sterta run-time jest jeszcze "wyżej", nad segmentem danych i rośnie w ramach alokacji przy użyciu funkcji `malloc`. Następnie jest miejsce przeznaczone na współdzielone moduły. Stos użytkownika zaczyna się pod największym dostępnym (dla użytkownika) adresem $2^{48}-1$ i rośnie w dół, w stronę mniejszych adresów. Region ponad stosem, zaczynający się od adresu $2^{48}$ jest przeznaczony na kod oraz dane w jądrze systemu.
![](https://i.imgur.com/eLzhANY.png)

## Pamięć operacyjna i dyskowa, pamięć podręczna
### Random Access Memory (RAM)
Pamięć RAM dzieli się na dwa rodzaje: statyczną ***(ang. Static RAM)*** i dynamiczną ***(ang. Dynamic RAM)***. SRAM jest znacznie szybsza i droższa, wykorzystywana jest w pamięci podręcznej procesora, jak i poza nią. DRAM używa się w pamięci głównej, jak i w buforze ramki w grafice. Zwykle w komputerze jest kilka MB pamięci SRAM (L1, L2, L3 Cache) i kilka lub kilkanaście GB pamięci DRAM.

 | Pamięć | Tranzystory na bit | Względny czas dostępu | Trwała? | Czuła (na zakłócenia)? | Względny koszt |
 | ------ | ------------------ | --------------------- | ------- | ---------- | -------------- |
 | SRAM   | $6$                | $1 \times$            | Tak     | Nie        |   $1000\times$ |
 | DRAM   | $1$                | $10 \times$           | Nie     | Tak        |   $1 \times$   |

SRAM przechowuje każdy bit w komórkach pamięci w jednym ze stanów (na podstawie napięcia), jego budowa opiera się na obwodzie stworzonym z 6 tranzystorów, a więc przechowywane dane będą niezmienne dopóki obwód jest włączony, nie trzeba odświeżać przechowywanych danych.

DRAM przechowuje każdy bit w kondensatorze, gdy jego napięcie zostanie zmienione, dane tracimy na zawsze. Pamięć musi być odświeżana średnio co $10$ do $100$ milisekund, a więc najpierw dane są zczytywane, a następnie ponownie zapisywane. Pamięć DRAM jest czuła na zakłócenia elektryczne i świetlne.

### Budowa DRAM, rodzaje ulepszonej pamięci DRAM
Standardowo chip pamięci DRAM jest podzielony w $d$ **superkomórek** *(ang. supercell)*, a każdy z nich składa się z $w$ komórek DRAM. DRAM rozmiaru $d \times w$ przechowuje łącznie $dw$ bitów. Superkomórki są ułożone w prostokątną tablicę z $r$ wierszy i $c$ kolumn, gdzie $rc = d$. Każda superkomórka ma adres postaci $(i, j)$, gdzie $i$ jest numerem wiersza, a $j$ numerem kolumny. 

Dane przesyłane są przez łączniki zwane **pinami**, każdy z nich przenosi jednobitowy sygnał. Każdy chip DRAM jest podłączony do zespołu obwodów zwanego **kontrolerem pamięci** *(ang. memory controller)*, który może przesyłać $w$ bitów z i do chipa w jednym czasie. 

Aby odczytać zawartość superkomórki $(i, j)$, kontroler pamięci wysyła adres wiersza i kolumny. DRAM odpowiada wysyłając zawartość $(i, j)$ z powrotem do kontrolera. Adres wiersza $i$ jest zwany **RAS (row access strobe) request**, a adres kolumny $j$ **CAS (column access strobe) request** - dzielą one te same piny adresowe DRAMu. Odczyt danych z superkomórki DRAM $(2, 1)$ jest przedstawiony na schemacie poniżej:
![](https://i.imgur.com/JPhECwl.png)


Wszystkie wymienione poniżej pamięci bazują na DRAM, jednak każda z nich w określony sposób optymalizuje czas dostępu do komórek pamięci:
* **Fast page mode DRAM (FPM DRAM)** umożliwa odczyt i zapis komórek z tego samego wiersza co poprzednio odczytywana/zapisywana podając jedynie adres kolumny bez podawania adresu wiersza, skracając czas odczytów/zapisów pamięci z komórek położomnych w tym samym wierszu. Porównajmy działanie FPM DRAM do standardowej pamięci DRAM: przy odczycie czterech superkomórek, kontroler pamięci musiałby wysłać cztery sygnały RAS/CAS, pomimo chęci dostępu do informacji w tym samym wierszu. W pamięci FPM DRAM wysyłane jest jedno wywołanie RAS/CAS, a następnie trzy wywołania CAS, co skraca czas dostępu.
* **Extended data out DRAM (EDO DRAM)** działa podobnie do FPM DRAM, jednak skraca czas pomiędzy wywołaniami CAS.
* **Synchronous DRAM (SDRAM)** wykorzystuje synchroniczne działanie, a więc zapytania są wykonywane szybciej niż w FPM DRAM oraz EDO DRAM.
* **Double Data-Rate Synchronous DRAM (DDR SDRAM)** jest ulepszoną wersją SDRAM zwięszkającą dwukrotnie szybkość DRAM. Jest to możliwe dzięki temu, że dane przesyłane są w czasie trwania zarówno rosnącego, jak i opadającego zbocza zegara. Różne typy DDR SDRAM są scharakteryzowane przez rozmiar bufora: DDR ($2$ bity), DDR2 ($4$ bity), DDR3 ($8$ bitów), DDR4 ($16$ bitów).
* **Video RAM (VRAM)** używana jest w buforze ramki w kartach graficznych, podobna jest do FPM DRAM, jednak różnią je dwa czynniki - wyjście VRAM jest spowodowane przesunięciem całej zawartości bufora w ciągu oraz pamięć VRAM umożliwa współbieżne odczyty i zapisy do bufora ramki.

### Pamięć nieulotna, trwała
**Pamięć nieulotna** *(ang. nonvolatile memory, NVM)* jest rodzajem pamięci przechowującą dane nawet po odłączeniu jej od zasilania. Zwykle są kojarzone z pamięcią **ROM** *(read only memory)*, pomimo tego, że część pamięci ROM obsługuje również zapisy. Pamięci te rozróżnia się poprzez ilość i mechanizm możliwych zapisów:
* **Programmable ROM (PROM)** może zostać zaprogramowane jedynie raz, programowanie polega na podaniu między linię bitu, a słowa napięcia od kilku do kilkunastu woltów, które przepala element łączący te dwa elementy.
* **Erasable Programmable ROM (EPROM)** może zostać zaprogramowane do około tysiąca razy. Można je rozpoznać po przeźroczystym okienku ze szkła kwarcowego na górze układu, przez które widać kość krzemową i które umożliwia dostęp do światła ultrafioletowego w razie konieczności skasowania. Programowanie odbywa się jedynie przy pomocy specjalnego urządzenia.
* **Electrically Erasable PROM (EEPROM)** może zostać zaprogramowane do około stu tysięcy razy, jednak w przeciwieństwie do EPROM nie wymaga specjalnego urządzenia do zaprogramowania, a więc może być przeprogramowana "w miejscu" na płytkach drukowanych.
* **Pamięci flash** *(ang. flash memory)* bazują na EEPROM, jednak dzięki zastosowaniu buforów zwiększono szybkość zapisu do pamięci.

Przykładowym zastosowaniem pamięci ROM jest zapis programów typu **BIOS** *(Basic Input/Output System)*, a więc zestaw podstawowych procedur pomiędzy systemem operacyjnym a sprzętem.


### Dostęp do pamięci głównej, magistrala
Dane przepływają w obie strony pomiędzy procesorem i pamięcią główną DRAM poprzez magistrale. **Transakcja odczytu** *(ang. read transaction)* przenosi dane z pamięci głównej do procesora, a **transakcja zapisu** *(ang. write transaction)* z procesora do pamięci głównej.

**Magistrala** *(ang. bus)* to zbiór równoległych przewodów przenoszących adres, pamięć i sygnały kontrolne, w zależności od ich budowy mogą współdzielić te same przewody lub korzystać z innych. Kilka urządzeń może korzystać z tej samej magistrali. Na poniższym schemacie przedstawione są magistrale łączące procesor z pamięcią DRAM.
![](https://i.imgur.com/z5Z16u0.png)
**Arithmetic and Logical Unit (ALU)** to jednostka arytmetyczno-logiczna prowadząca proste operacje na liczbach całkowitych, np. operacje logiczne, dodawanie, przesunięcia bitowe, odejmowanie, negacje, mnożenie i czasami dzielenie. **I/O bridge** zawiera w sobie kontroler pamięci i łączy się z magistralami pamięci (do pamięci głównej) i systemową.

### Pamięć dyskowa, budowa dysku Hard-Disk Drive (HDD)
**Dyski** *(ang. disks)* są przystosowane do przechowywania ogromnej ilości danych (w przeciwieństwie do pamięci RAM), jednak odczyt danych zajmuje około sto tysięcy więcej czasu niż z DRAM i około milion więcej niż z SRAM.

![](https://i.imgur.com/TQS3bKk.png)
**Dyski HDD** *(ang. hard-disk drive)* zbudowane są z ułożonych na sobie **talerzy** *(ang. platter)*, każdy z nich ma dwie **powierzchnie** *(ang. surface)* magnetyczne. **Piasta silnika** *(ang. spindle)* w środku dysku obraca talerze w stałym tempie, zwykle pomiędzy $5400$ a $15000$ obrotów na minutę **(RPM)**. Każda powierzchnia składa się z pierścieni nazywanych **ścieżkami** *(ang. track)* i każda z nich podzielona jest na **sektory** *(ang. sector)*. Każdy sektor zawiera taką samą ilość danych (zwykle $512$ bajtów), a oddzielają je przerwy, w których nie ma żadnych informacji. 

Pojemność dysku jest określana przez następujące czynniki:
* **gęstość zapisu** *(ang. recording density)* ($\text{bits/in}$) - ilość bitów, które mogą zostać "ściśnięte" na jednocalowym segmencie ścieżki,
* **gęstość ścieżki** *(ang. track density)* ($\text{tracks/in}$) - ilość ścieżek, które mogą zostać "ściśnięte" na jednocalowym segmencie promienia rozciągającym się od środka talerza 
* **gramatura** *(ang. areal density)* ($\text{bits/in}^2$) - iloczyn gęstości zapisu i gęstości ścieżki

Możemy ją wyliczyć ze wzoru:
$$
\frac{\text{bajty}}{\text{sektor}} \cdot 
\frac{\text{śr. liczba sektorów}}{\text{ścieżka}} \cdot 
\frac{\text{liczba ścieżek}}{\text{powierzchnia}} \cdot 
\frac{\text{liczba powierzchni}}{\text{talerz}} \cdot 
\frac{\text{liczba talerzy}}{\text{dysk}}
$$

### Odczyt danych z sektora dysku HDD
Procesor rozpoczyna odczyt z dysku poprzez wysłanie komendy, numeru bloku logicznego i docelowego adresu pamięci do zmapowanego adresu pamięci powiązanego z dyskiem, następnie kontroler dysku czyta sektor (procesor zawsze czyta całe sektory) i wykonuje transfer DMA do pamięci głównej, a gdy ten się zakończy, kontroler dysku informuje procesor przerwaniem.

### Solid State Disk (SSD)
Dysk **SSD** jest kolejnym nośnikiem danych, bazuje on na pamięci flash. Dysk SSD składa się z jednego lub więcej chipów pamięci flash, które zastępują mechaniczny dysk w standardowym HDD oraz z **warstwy translacji flash** *(ang. flash translation layer)*, która spełnia rolę kontrolera dysku (a więc tłumaczy zapytania do bloków logicznych). Zwykle czasy odczytów są szybsze od czasów zapisów.
![](https://i.imgur.com/Tu7Bq0a.png)

### Hierarchia pamięci
![](https://i.imgur.com/rMrLEdF.png)

### Pamięć podręczna, organizacja pamięci podręcznej
**Pamięć podręczna** *(ang. cache)* jest mechanizmem, w którym część danych zgormadzonych w źródłach o długim czasie dostępu i niższej przepustowości jest dodatkowo przechowywana w pamięci o lepszych parametrach.

![](https://i.imgur.com/dc7pot6.png)

**Trafienie** *(ang. cache hit)* to określenie sytuacji, w której program potrzebuje informacji o obiekcie $d$ z poziomu $k+1$ i szuka jej najpierw na poziomie $k$. Jeżeli $d$ okaże się być zapisane na poziomie $k$, to mamy do czynienia z trafieniem.

**Chybienie** *(ang. cache miss)* jest sytuacją odwrotną do trafienia, a więc informacji o obiekcie $d$ nie znajdujemy na poziomie $k$. Gdy dochodzi do chybienia, to cache na poziomie $k$ zapisuje blok zawierający $d$ z poziomu $k+1$, potencjalnie nadpisując już istniejący blok. 

Proces ten nazwany jest często **zastępywaniem** *(ang. replacing)* lub **eksmitowaniem** *(ang. evicting)* bloku. Taki blok określany jest jako **victim block**. 

Decyzję o tym, który blok zastąpić podejmuje **polityka wymian** *(ang. replacement policy)*, na przykład cache z **random replacement policy** wybierze losowy blok, a cache z **least recently used (LRU)** wybierze blok, który był używany najdawniej.

Rozróżniamy kilka rodzajów chybień:
* **Chybienia przymusowe** *(ang. compulsory miss, cold miss)* spowodowane są próbą odczytu cache'a na poziomie $k$, który jest pusty.
* **Chybienie spowodowane kolizją na danym adresie** *(ang. conflict miss)* występują wtedy, gdy cache jest wystarczający do pomieszczenia danych obiektów, jednak z powodu odwołania do tego samego bloku pamięci podręcznej, brakuje danych w cache'u.
* **Chybienia spowodowane ograniczoną pojemnością** *(ang. capacity miss)* występuje w przypadku, gdy rozmiar danych przekracza rozmiar cache'u.

Pamięć podręczna zorganizowana jest w następujący sposób:

![](https://i.imgur.com/aOObdHt.png)

System komputerowy posiadający w każdym adresie pamięci $m$ bitów będzie składał się z $2^m$ unikalnych adresów. Cache dla takiego systemu jest ułożony w tablicę $S = 2^s$ **zbiorów pamięci podręcznej** *(ang. cache set)*. Każdy z nich składa się z $E$ **linii** *(ang. cache lines)*, a każda linia z **bloku pamięci** o pojemności $B = 2^b$ bajtów, **"ważnego bitu"** *(ang. valid bit)* określającego czy linia zawiera znaczące informacje oraz z $t = m - (b + s)$ **tag bitów** *(ang. tag bits)* unikalnie identyfikujących blok przechowywany w linii cache.

W ogólności organizacja cache może być scharakteryzowana przez krotkę $(S, E, B, m)$ o łącznej pojemności $C = S \times E \times B$.

Aby odczytać dane z adresu $A$ z głównej pamięci, wysyłany jest adres $A$ do pamięci podręcznej - jeżeli kopia słowa jest przechowywana w adresie $A$, to słowo jest wysyłane natychmiastowo do procesora. Cache jest stworzony w taki sposób, że łatwo może znaleźć żądane słowo poprzez podglądanie bitów adresu, podobnie do działania tablicy hashującej z bardzo prostą funkcją hashującą.

Działa to w następujący sposób: parametry $S$ oraz $B$ wywołują podział $m$ bitów adresu na trzy pola pokazane na schemacie poniżej: $t$ tag bitów, $s$ **bitów indeksu zbioru** *(ang. set index bits)* oraz $b$ **bitów przesunięcia bloku** *(ang. block offset bits)*. Bity $s$ w adresie $A$ tworzą tablicę tablicę $S$ zbiorów, pierwszy z nich jest zbiorem o indeksie $0$, drugi o indeksie $1$, aż do ostatniego o indeksie $S-1$. 
![](https://i.imgur.com/zVUaNHH.png)
Gdy odczytywana wartość jest typu *unsigned*, bity indeksu zbioru mówią o tym, w którym zbiorze jest przechowywane słowo. Wiedząc w którym zbiorze jest szukane słowo, $t$ tag bitów mówi o tym, w której linii w zbiorze przechowywane jest słowo - a może się ono znaleźć tylko w tym, gdzie valid bit jest ustawiony na $1$ i tag bity linii pokrywają się z tag bitami adresu $A$. Po zlokalizowaniu linii, bity przesunięcia bloku $b$ mówią nam o tym, w którym miejscu zaczyna się słowo w bloku $B$.

### Pamięć podręczna z mapowaniem bezpośrednim
**Pamięć podręczna z mapowaniem bezpośrednim** *(ang. direct-mapped cache)* to pamięć podręczna w której w każdym zbiorze $S$ znajduje się tylko jedna linia $E$ (a więc $E=1$). 

![](https://i.imgur.com/SQatMML.png)

Załóżmy, że mamy system z procesorem, zestawem rejestrów, pamięcią podręczną L1 oraz pamięcią główną. W trakcie wykonywania przez procesor instrukcji odczytu słowa $w$ z pamięci, następuje wyszukanie słowa w L1 cache - gdy słowo się tam znajduje, to mamy do czynienia z trafieniem L1 i pamięć podręczna wydobywa $w$ i zwraca je do procesora. W przypadku chybienia, procesor musi chwilę poczekać na zapytanie L1 o wykonaniu kopii bloku przechowującego $w$ w pamięci głównej. Gdy oczekiwany blok przyjdzie z pamięci głównej, L1 przechowuje go w jednej z linii, wydobywa słowo $w$ z tego bloku i zwraca je procesorowi. Cały ten proces składa się z trzech kroków:
1. **wybór zbioru** *(ang. set selection)* - pamięć podręczna wybiera zbiór $s$ bitów indeksu zbioru ze środka adresu dla słowa $w$, są one interpretowane jako liczba całkowita typu *unsigned*. Możemy myśleć o cache'u jak o jednowymiarowej tablicy zbiorów, w którym $s$ jest wybranym indeksem.
![](https://i.imgur.com/OMyV1bT.png)

2. **dopasowanie linii** *(ang. line matching)* - po wyborze zbioru $i$ w poprzednim kroku, należy sprawdzić czy w wybranym zbiorze znajduje się kopia słowa $w$ (dla mapowania bezpośredniego jest to łatwe, gdyż jest tylko jedna linia w zbiorze). Kopia może znajdować się wtedy i tylko wtedy, gdy valid bit jest ustawiony na $1$ i tag w linii cache pokrywa się z tagiem adresu słowa $w$.
![](https://i.imgur.com/JGaKTvR.png)

3. **wydobycie słowa** *(ang. word extraction)* - w przypadku trafienia wiemy, że $w$ znajduje się gdzieś w bloku $B$. Ostatnim krokiem jest sprawdzenie, w którym miejscu dane słowo się zaczyna, a możemy to łatwo odczytać z $b$ bitów przesunięcia.

W przypadku chybienia, pamięć podręczna musi uzyskać oczekiwany blok z kolejnego poziomu pamięci i przetrzymać go w linii jednego ze zbiorów. W ogólnym przypadku, gdy zbiór jest przepełniony (bity valid są ustawione), jedna z linii musi zostać zastąpiona. W przypadku pamięci podręcznej mapowanej bezpośrednio jest to proste, gdyż nowa linia zastępuje jedną z aktualnie istniejących linii.

### Sekcyjno-skojarzeniowa (wielodrożna) pamięć podręczna
**Wielodrożna pamięć podręczna** *(ang. set-associative cache)* to pamięć podręczna, w której każdy zbiór przechowuje więcej niż jedną linię, dokładniej $1 < E < C/B$. Wybór zbioru w takiej pamięci przebiega tak samo jak w pamięci z mapowaniem bezpośrednim. 

![](https://i.imgur.com/8ahM8GR.png)


Dopasowanie linii w tej pamięci musi sprawdzić tag bity i valid bit wielu linii, aby dowiedzieć się, czy słowo z zapytania jest w wybranym zbiorze. **Pamięć asocjacyjna CAM** *(ang. content-addressable memory)* jest tablicą par $\text{(klucz, wartość)}$, która przyjmuje na wejściu klucz, a zwraca wartość z jednej par, w których pokrywa się wprowadzony klucz.

W razie chybienia w wielodrożnej pamięci podręcznej największy priorytet zapisu ma pusta linia, a gdy takiej nie znajdziemy, musimy wybrać jedną z linii, w których przechowywane są już dane. Najłatwiejszym podejściem jest wybranie losowej linii, jednak istnieją polityki wymian (wspomniane wyżej) takie jak LRU.

### Pamięć całkowicie skojarzeniowa
**Pamięć całkowicie skojarzeniowa** *(ang. fully associative cache)* to pamięć podręczna składająca się z jednego zbioru, który przechowuje wszystkie linie, a więc $E = C / B$.
![](https://i.imgur.com/O11qVFP.png)

Wybór zbioru w tym przypadku jest trywialny, jako że istnieje tylko jeden. Dopasowanie linii jak i wydobycie słowa działa w taki sam sposób jak w wielodrożnej pamięci podręcznej.

## Optymalizacja
Programy oprócz tego, że powinny spełniać swoje zadania, wykonując poprawne obliczenia, powinny działać szybko, efektywnie. Sposoby wymienione w poniższych akapitach będą o optymalizacji programów.

### Najprostszy sposób optymalizacji programów
Pierwszym krokiem, który powinniśmy podjąć w trakcie optymalizacji programu, jest pozbycie się niepotrzebnych wywołań funkcji, instrukcji warunkowych, czy odwołań do pamięci - dzięki temu w łatwy sposób zwiększymy wydajność naszego programu.

### Możliwości i ograniczenia w optymalizacji kompilatora
Kompilując kod w GCC, możemy wykorzystać zbiór kilku poziomów optymalizacji, aby ustalić jak bardzo kod powinien zostać zoptymalizowany. 

Flaga `-Og` spowoduje, że kod zostanie zoptymalizowany bardzo delikatnie, wykorzystując zbiór podstawowych optymalizacji.

Wywołanie kompilatora z flagami `-O1`, `-O2`, `-O3` spowoduje wykorzystanie bardziej złożonych optymalizacji, które przyspieszą działanie programu, jednak mogą przyczynić się do zwiększenia rozmiarów programu i utrudnienia debugowania (przy użyciu standardowych narzędzi). Flaga `-O2` została przyjęta za standard przy większości projektów oprogramowania, które używają kompilatora GCC.

### Memory aliasing
Rozpatrzmy dwie funkcje zaimplementowane w następujący sposób:
```c=
void twiddle1(long *xp, long *yp) {
    *xp += *yp;
    *xp += *yp;
}

void twiddle2(long *xp, long *yp) {
    *xp += 2 * *yp;
}
```
Mogłoby się wydawać, że takie funkcje zwrócą takie same wyniki, jako że obie dodają dwa razy wartość przechowywaną we wskaźniku `yp` do wartości ze wskaźnika `xp`, jednak funkcja `twiddle2` jest bardziej wydajna, jako że wykorzystuje tylko 3 odwołania do pamięci - odczytanie `*xp`, odczytanie `*yp`, zapisanie `*xp`, w przeciwieństwie do `twiddle1`, które wkonuje powyższe instrukcje po dwa razy. 

Spójrzmy jednak na sytuację, w której `xp` oraz `yp` są równe. Wtedy funkcja `twiddle1` wykona instrukcje:
```c=3
    *xp += *yp; // podwojenie wartości w xp
    *xp += *yp; // podwojenie wartości w xp
```
w trakcie, gdy funkcja `twiddle2` wykona:
```c=9
    *xp += 2 * *yp; // potrojenie wartości w xp
```
Da nam to zupełnie inne wyniki, jako że wartość `xp` po wywołaniu funkcji `twiddle1` zostanie zwiększona czterokrotnie, a po wywołaniu `twiddle2` tylko trzykrotnie. Z tego powodu `twiddle2` nie zostanie wygenerowane jako ulepszona wersja `twiddle1`, gdyż kompilator musi rozpatrzyć między innymi przypadek, gdy dwa wskaźniki wskazują na to samo miejsce w pamięci - taką sytuację określa się jako **memory aliasing**. Jest to jedno z większych ograniczeń optymalizacji, jako że w sytuacji, gdy kompilator nie może zdecydować, czy dwa wskaźniki mogą być aliasowane, musi on założyć, że każda możliwość jest prawdopodobna, ograniczając zbiór poprawek.

### Wywołania funkcji
Rozważmy funkcje:
```c=
long f();

long func1() {
    return f() + f() + f() + f();
}

long func2() {
    return 4 * f();
}
```
W tym przypadku również funkcje sprawiają wrażenie, iż zwracany wynik jest taki sam, jednak funkcja `func2` wywołuje `f` tylko raz, w trakcie gdy `func1` wywołuje `f` cztery razy. Gdyby funkcja `f` była zdefiniowana w następujący sposób, otrzymalibyśmy różne wyniki:
```c=
long counter = 0;

long f() {
    return counter++;
}
```
Zmiana liczby wywołań funkcji zmienia zachowanie programu, np. wywołanie `func1` zwróciłoby $0+1+2+3=6$, w trakcie gdy `func2` zwróciłoby $4\cdot 0 = 0$, zakładając, że zmienna `counter` ustawiona by była na zero.

### Opisywanie wydajności programu

Wprowadźmy jednostkę **CPE** *(ang. cycles per element)*, będzie nam ona służyć w opisywaniu wydajności programów. 

Zwykle procesory mogą wykonać miliardy cykli w trakcie sekundy, określa to ich taktowanie, np. $4.2\text{GHz}$, a więc wykonanie jednego cyklu (w przypadku takiego taktowania) zajmie około $0.238 \text{ns}$.

Weźmy dla przykładu procedury `psum1` oraz `psum2` obliczające sumę prefiksową wektora `a`. Zaimplementowane są one w taki sposób:
```c=
void psum1(float a[], float p[], long n) {
    long i; 
    p[0] = a[0];
    for (i = 1; i < n; i++)
        p[i] = p[i-1] + a[i];
}

void psum2(float a[], float p[], long n) {
    long i;
    p[0] = a[0];
    for (i = 1; i < n-1; i += 2) {
        float mid_val = p[i-1] + a[i];
        p[i] = mid_val;
        p[i+1] = mid_val + a[i+1];
    }
    // dla parzystego n dokoncz jak wyzej:
    if (i < n)
        p[i] = p[i-1] + a[i];
}
```

Funkcja `psum1` oblicza jeden element wektora na iteracje, w trakcie gdy funkcja `psum2` używa techniki **odwijania pętli** *(ang. loop unrolling, loop unwinding)* do obliczenia dwóch elementów na iterację. Dzięki temu mniej razy sprawdzamy warunek pętli, co przyspiesza czas działania funkcji.

Czas wymagany przez obie funkcje można zobaczyć na wykresie:
![](https://i.imgur.com/TchGF9P.png)

Dla dużych wartości $n$ czas działania funkcji będzie w dużej mierze zależny od czynnika liniowego, a nie czasu potrzebnego na zainicjowanie procedury, ustawienie pętli.

### Eliminowanie nieefektywności pętli

Przy pracy z zaimplementowanym przez nas wektorem mamy funkcję, która przegląda wszystkie elementy wektora. 

Głównym warunkiem pętli jest `i < vec_length(v)`, co jest niezbyt efektywnym wyborem, jako że za każdym razem musimy wywołać funkcję zwracającą niezmienną wartość. 

Możemy to rozwiązanie ulepszyć, deklarując zmienną `int length = vec_length(v)` przed pętlą, a następnie sprawdzając warunek pętli `i < length`. 

Taka operacja zmniejsza wskaźnik CPE z $10.12$ do $7.02$.

Drugim przykładem, zbliżonym do powyższego, jest zamiana elementów tablicy `char` na małe litery. Warunkiem pętli w wolniejszej wersji `lower1` jest `i < strlen(s)`, a w szybszej (`lower2`) - `i < len`, gdzie `len = strlen(s)`. 

Aby obliczyć długość `string` lub `char*` musimy przejść po całej tablicy znaków, aż do momentu natrafienia na znak `\0`. Oznacza to, że złożoność algorytmu `strlen` to $O(n)$, gdzie $n$ to długość wejścia.

Czas działania obu funkcji przedstawia wykres poniżej: 
![](https://i.imgur.com/4WnMDXC.png)

### Inne sposoby optymalizacji
#### Zmniejszenie ilości wywołań funkcji
Zaskakująco, operacja ta nie ma wpływu na wydajność, jednak umożliwia dodatkowe optymalizacje.

#### Ograniczenie niepotrzebnych odwołań do pamięci

Rozważmy funkcję `combine1` sumującą wartość wszystkich elementów naszego wektora:
```c=
void combine1(vec_ptr v, int *dest) {
    long i;
    long length = vec_length(v);
    int *data = get_vec_start(v);
    
    *dest = 0;
    for (i = 0; i < length; i++) {
        *dest = *dest + data[i];
    }
}
```

Widzimy, że w tak napisanej funkcji niepotrzebnie za każdym razem czytamy i zapisujemy wartość `*dest`. Możemy to w łatwy sposób zastąpić, dodając przed pętlą akumulator `acc`:
```c=
void combine2(vec_ptr v, int *dest) {
    long i;
    long length = vec_length(v);
    int *data = get_vec_start(v);
    int acc = 0;

    for (i = 0; i < length; i++) {
        acc = acc + data[i];
    }
    *dest = acc;
}
```
W taki sposób zmniejszamy czas działania prawie sześciokrotnie.

### Uproszczona budowa procesora
Budowa procesora przedstawionego na schemacie bazuje na procesorach Intela, są one opisywane jako **superskalarne** *(ang. superscalar)*, co oznacza, że mogą przeprowadzać wiele operacji na każdy cykl zegarowy i kolejność ich wykonywania nie musi się pokrywać z kolejnością programu na poziomie maszynowym. Ogólnie procesor składa się z **jednostki kontrolującej instrukcje** *(ang. instruction control unit, ICU)*, która odpowiada za odczytywanie ciągu instrukcji z pamięci podręcznej (L1 i-cache) i generowania ich ze zbioru podstawowych operacji do działania na danych programu, oraz z **jednostki wykonującej** *(ang. execution unit, EU)*, która wykonuje te instrukcje. 

![](https://i.imgur.com/SVNbhdY.png)

Nowoczesne procesory używają techniki **przewidywania skoków** *(ang. branch prediction)*, która zgaduje czy wybrana instrukcja powinna być przeprowadzona. Już przed określeniem, czy instrukcja powinna być przeprowadzona, wykonywane są instrkucje, które w razie błędnego założenia predyktora skoków zostają odrzucane.

### Odwijanie pętli i obliczanie równoległe
**Odwijanie pętli** *(ang. loop unrolling, loop unwinding)* to proces polegający na redukcji liczby iteracji pętli poprzez zwiększenie liczby operacji w każdej iteracji. 

Może to usprawnić program na dwa sposoby: 
* zmniejsza to liczbę operacji, które nie wpływają bezpośrednio na wynik (indeksy pętli, skoki warunkowe).
* umożliwia to kolejne usprawnienia kodu poprzez eliminowanie liczby operacji

Powinniśmy wykorzystać potencjał procesora, wiedząc o tym, że wiele operacji może być wykonywanych jednocześnie. 

Pierwszym i najłatwiejszym sposobem jest wykonanie kilku instrukcji w jednej iteracji, takie odwijanie pętli to odwijanie $k \times 1$. Funkcja wtedy ma postać:
```c=
void combine(vec_ptr v, int *dest) {
    long i;
    long length = vec_length(v);
    long limit  = length - 1;
    int *data   = get_vec_start(v);
    int acc = 0;

    // połączenie dwóch (lub k) elementów naraz
    for (i = 0; i < limit; i += 2) {
        acc = acc + data[i] + data[i + 1];
    }
    // dokończenie dla pozostałych elementów
    for (; i < length; i++) {
        acc = acc + data[i];
    }
    *dest = acc;
}
```

Drugim sposobem jest wykorzystanie kilku akumulatorów dla operacji łącznych i przemiennych, takich jak dodawanie czy mnożenie liczb całkowitych, taki zabieg to odwijanie pętli $k \times k$ (w jednej iteracji obliczamy wartość $k$ akumulatorów, wykorzystując do tego $6$ elementów jednocześnie):
```c=
void combine(vec_ptr v, int *dest) {
    long i;
    long length = vec_length(v);
    long limit  = length - 1;
    int *data   = get_vec_start(v);
    int acc1 = acc2 = 0;

    // połączenie dwóch elementów naraz
    for (i = 0; i < limit; i += 2) {
        acc1 = acc1 + data[i];
        acc2 = acc2 + data[i];
    }
    
    // dokończenie dla pozostałych elementów
    for (; i < length; i++) {
        acc1 = acc1 + data[i];
    }
    *dest = acc1 + acc2;
}
```
Dzięki temu prawie dwukrotnie zwiększamy szybkość działania funkcji. Graficznie możemy wewnętrzną pętlę `combine` przedstawić w następujący sposób, gdzie każda iteracja ma dwie operacje `vmulsd` (przykład z CS:APP operował na iloczynie, a nie sumie, stąd taka operacja):
![](https://i.imgur.com/3wdPgq8.png)

Trzecim sposobem jest transformacja polegająca na zmianie kolejności *(ang. reassociation transformation)*, takie odwijanie pętli określa się jako $k \times 1a$:
```c=
acc = (acc + data[i]) + data[i+1];
```
```c=
acc = acc + (data[i] + data[i+1]);
```
Dla dodawania czynnik CPE jest identyczny, jednak dla obliczania iloczynu zmniejsza się on dwukrotnie w drugim przypadku.


## Pamięć wirtualna

Procesy w systemie dzielą procesor i pamięć główną z innymi procesami, jednak jest to dosyć trudne - wraz ze wzrostem zapotrzebowania na zasoby CPU, procesy zwalniają, w przypadku gdy procesy wymagają zbyt dużej ilości pamięci, część z nich przestanie prawidłowo działać. Część z tych procesów zacznie się zachowywać w nieokreślony sposób, a pamięć będzie podatna na zmiany z zewnątrz, co może spowodować błędy w innych procesach.

Aby wydajniej zarządzać pamięcią i uniknąć wielu błędów, systemy używają abstrakcję pamięci głównej znaną jako **pamięć wirtualna** *(ang. virtual memory, VM)*. 
