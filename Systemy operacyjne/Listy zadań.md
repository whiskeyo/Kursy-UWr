# Systemy operacyjne 2020

## Lista zadań nr 0

### Zadanie 1
Opisz różnice między **przerwaniem sprzętowym** *(ang. hardware interrupt)*, **wyjątkiem procesora** *(ang. exception)* i **pułapką** *(ang. trap)*. Dla każdego z nich podaj co najmniej trzy przykłady zdarzeń, które je wyzwalają. W jakim scenariuszu wyjątek procesora nie oznacza błędu czasu wykonania programu? Kiedy pułapka jest generowana w wyniku prawidłowej pracy programu?

**Wyjątek procesora** to klasa, która dzieli się na cztery rodzaje. Są nimi:
* **przerwanie sprzętowe** wywoływane jest przez sygnał z urządzeń wejścia i wyścia (I/O), są one asynchroniczne, a więc nie są spowodowane przez wykonywanie instrukcji w CPU, a wywoływane przez hardware. Wywołują je akcje takie jak ruszenie myszką (na PS/2), naciśnięcie przycisku, otrzymanie danych z karty sieciowej (i wiele innych spowodowanych przez interakcję z urządzeniami I/O).
![](https://i.imgur.com/MyWpO4P.png)
![](https://i.imgur.com/fXxphnL.png)
* **fault** jest wynikiem instrukcji, która może zostać naprawiona przez exception handler - jeśli jest on w stanie ją naprawić, to wraca do instrukcji i wyknouje ją ponownie, w przeciwnym wypadku przekazuje tę instrukcję do aborta. Wywołać go mogą dzielenie przez 0, segmentation fault, page fault.
![](https://i.imgur.com/INrIkZX.png)
* **abort** jest wynikiem instrukcji nienaprawialnej. Wywołać go może machine check na hardware, np. w pamięci, magistrali systemowej lub procesorze. W systemach Windows przy aborcie włącza się BSOD.
![](https://i.imgur.com/cBItijT.png)
* **pułapka** *(ang. trap)* to wyjątek uzyskany celowo (synchroniczny), który jest wynikiem wykonywanej instrukcji. Zdarza się wtedy, gdy user żąda usługi z jądra. Wtedy poprzez specjalne api (wrapper) wywoływany jest system call. Przykładami mogą być `fork`, `execve`, `exit`. Korzysta z nich też każdy debugger, np. dla breakpointów.
![](https://i.imgur.com/z6XPc9q.png)

Różnica między zwykłą funkcją a `syscall` jest taka, że zwykła funkcja wykonywana jest w user mode, a `syscall` w kernel mode. Wyjątki asynchroniczne (a więc przerwania sprzętowe) nie powodują przerwania działania programu.

### Zadanie 2
Opisz mechanizm **obsługi przerwań** bazujący na **wektorze przerwań** *(ang. interrupt vector table)*. Co robi procesor przed pobraniem pierwszej instrukcji **procedury obsługi przerwania** *(ang. interrupt handler)* i po natrafieniu na instrukcję powrotu z przerwania? Czemu procedura obsługi przerwania powinna być wykonywana w **trybie jądra** *(ang. kernel mode)* i używać stosu odrębnego od stosu użytkownika?

Aby **obsłużyć przerwanie**, kontroler umieszcza numer na linii adresowej, okręslając które urządzenie wymaga uwagi i ustawia sygnał mający na celu przerwanie pracy procesora. 

![](https://i.imgur.com/fXxphnL.png)

Sygnał przerwania powoduje, że procesor zatrzymuje operację, którą wykonywał, i zaczyna robić coś innego. Numer na liniach adresowych jest wykorzystywany jako indeks do tabeli znanej jako **wektor przerwań** i służy do pobrania nowej wartości licznika programu (PC). Zwykle przerwania są numerowane od 0 do 255 (używamy więc 1 bajta). Ta wartość PC wskazuje na początek odpowiedniej procedury obsługi przerwania (zwykle `jump`, aby zaoszczędzić liczbę wykonywanych instrukcji). Zazwyczaj od tego momentu rozkazy pułapek i przerwań korzystają z tego samego mechanizmu, a często współdzielą ten sam wektor przerwań. Wektor ten może być zaszyty "na sztywno" w sprzęcie lub może znajdować się w dowolnym miejscu pamięci, a rejest procesora wskazuje na jego początek.

**Procedura obsługi przerwania** to kod mający na celu obsługę konkretnego sygnału żądania przerwania, najczęściej określa je system operacyjny lub BIOS.

Przed uruchomieniem procedury obsługi przerwania sprzęt zawsze zapisuje pewne informacje, ich rodzaj i miejsce zapisu różnią się w zależności od procesora. Całkowitym minimum jest zapisanie PC, dzięki czemu można wznowić przerwany proces. W drugim skrajnym podejściu zapisywane są wszystkie widoczne rejestry, a także wiele rejestrów zewnętrznych.

Wyjątek jest bardzo zbliżony do zwykłej procedury, działa on jednak w trybie jądra, aby mieć dostęp do wszystkich zasobów systemowych. Bez tego np. page fault nie działa, ponieważ wpisuje on na dysk fizyczny, a user mode tego nie może zrobić.

Jeżeli do zapisu wykorzystywany jest stos, może narodzić się problem jakiego stosu użyć. Jeżeli zostanie wykorzystany bieżący stos, może być to stos należący do procesu użytkownika, a wartość wskaźnika stosu może być nieprawidłowa, co doprowadzi do błędu krytycznego, gdy sprzęt podejmie próbę zapisania pewnych słów pod wskazanym adresem. Dlatego korzysta się więc ze stosu jądra, gdyż istnieje wtedy większa szansa na to, że wskaźnik stosu będzie miał prawidłową wartość i będzie wskazywał na stronę dostępną w pamięci. Przełączenie do trybu jądra może jednak wymagać zmiany kontekstu MMU i prawdopodobnie spowoduje utratę ważności dużej części lub całości pamięci podręcznej oraz buforów TLB. Ponowne załadowanie tych danych zwiększa czas obsługi przerwania, przez co przyczyni się do marnotrastwa czasu procesora.

*Kiedyś, np. w procesorach Intel 8080 nie używano stosu, a dwa rejestry: jeden systemowy, a drugi do obsługi przerwań.*

**Tryb jądra** *(ang. kernel mode)* zezwala na zrobienie wszystkiego w systemie, daje dostęp do hardware'u, natomiast **tryb użytkownika** *(ang. user mode)* na to nie pozwala, nie możemy np. czytać z pamięci czy do niej zapisywać, uruchomiony kod musi korzystać z API systemu, aby wykonać wspomniane zadania.

### Zadanie 3
Bazując na formacie ELF *(ang. Executable and Linkable Format)* opisz składowe piku wykonywalnego. Czym różni się **sekcja** od **segmentu**? Co opisują **nagłówki programu**? Skąd system operacyjny wie, pod jakim adresem ma umieścić segmenty programu i gdzie położona jest pierwsza instrukcja programu?

* Uzyskanie pliku `main.o` z pliku `main.c` bez linkowania:
```
$ gcc -c main.c
```
* Wydruk tablicy symboli i nagłówków sesji:
```
$ readelf -t -s [nazwa].o
```

**Plik relokowalny** *(ang. relocatable object file)* w formacie ELF wygląda w taki sposób:
![](https://i.imgur.com/CO7tIjt.png)
Rozpoczyna się on 16-bajtową sekwencją zwaną **ELF header** opisującą wielkość słowa i kolejność bajtów *(ang. endianness)* systemu, który wygernerował plik. ELF header zawiera również informacje umożliwiające linkerowi spasowanie i zinterpetowanie pliku obiektowego - są to między innymi typ pliku obiektowego, rodzaj architektury, czy rozmiar i liczbę sekcji.

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
| `.strtab`   | Tablica `string`ów dla sekcji tabeli symboli `.symtab` oraz `.debug`, jak i nazw sekcji w nagłówkach.

**Sekcje** zawierają informacje potrzebne w trakcie linkowania, a **segmenty** informacje wykorzystywane w trakcie runtime. Często sobie odpowiadają w pamięci wirtualnej procesu.

System operacyjny wie pod jakim adresem ma umieścić segmenty programu dzięki *segment header table*. Adres pierwszej instrukcji znajduje się w headerze ELF.

Przykładowy wydruk polecenia `readelf -t -s swap.o` z jakiegoś zadania z ASKa:
```
There are 9 section headers, starting at offset 0x1f8:
 
Section Headers:
  [Nr] Name
       Type              Address          Offset            Link
       Size              EntSize          Info              Align
       Flags
  [ 0]
       NULL                   NULL             0000000000000000  0000000000000000  0
       0000000000000000 0000000000000000  0                 0
       [0000000000000000]:
  [ 1] .text
       PROGBITS               PROGBITS         0000000000000000  0000000000000040  0
       000000000000001e 0000000000000000  0                 1
       [0000000000000006]: ALLOC, EXEC
  [ 2] .rela.text
       RELA                   RELA             0000000000000000  0000000000000148  6
       0000000000000060 0000000000000018  1                 8
       [0000000000000040]: INFO LINK
  [ 3] .data
       PROGBITS               PROGBITS         0000000000000000  0000000000000060  0
       0000000000000008 0000000000000000  0                 8
       [0000000000000003]: WRITE, ALLOC
  [ 4] .rela.data
       RELA                   RELA             0000000000000000  00000000000001a8  6
       0000000000000018 0000000000000018  3                 8
       [0000000000000040]: INFO LINK
  [ 5] .bss
       NOBITS                 NOBITS           0000000000000000  0000000000000068  0
       0000000000000004 0000000000000000  0                 4
       [0000000000000003]: WRITE, ALLOC
  [ 6] .symtab
       SYMTAB                 SYMTAB           0000000000000000  0000000000000068  7
       00000000000000c0 0000000000000018  5                 8
       [0000000000000000]:
  [ 7] .strtab
       STRTAB                 STRTAB           0000000000000000  0000000000000128  0
       000000000000001b 0000000000000000  0                 1
       [0000000000000000]:
  [ 8] .shstrtab
       STRTAB                 STRTAB           0000000000000000  00000000000001c0  0
       0000000000000036 0000000000000000  0                 1
       [0000000000000000]:
```

### Zadanie 4
Zapoznaj się z rozdziałami 3.4 i A.2 dokumentu *[System V Application Binanry Interface AMD64 Architecture Processor Supplement](https://www.uclibc.org/docs/psABI-x86_64.pdf)* i odpowiedz na następujące pytania:
* W jaki sposób jądro musi przygotować **przestrzeń adresową** procesu? Co musi się znajdować na stosie w momencie wywołania procedury `_start`? Do czego służy *auxiliary vector*? Można go wyświetlić wydając przykładowe polecenie `LD_SHOW_AUXV=1 /bin/true`.
* W jaki sposób wywołać funkcję jądra? W których rejestrach należy umieścić argumenty? Gdzie można spodziewać się wyników i jak jądro sygnalizuje niepowodzenie **wywołania systemowego**?

*Disclaimer:* Warto pamiętać o tym że ABI (Application Binary Interface) i API (Application Programming Interface) to dwie zupełnie różne rzeczy, w ABI są rzeczy takie jak obsługa ramki stosu i inne niskopoziomowe sprawy, a w API są bardziej wysokopoziomowe wywołania, takie jak `syscall` do porozumiewania się z innymi programami. ABI x32 pozwala na pracowanie na architekturze 64-bitowej na adresach 32-bitowych, dzięki temu zwiększamy szybkość działania o około 30%.

Przestrzeń adresowa procesu jest przygotowywana w następny sposób: procesor jest resetowany do "czystego" stanu, tzn. wszystkie flagi (CF, ZF, IF, itp.) są wyzerowane.

Auxiliary vector w trakcie uruchamiania programu otrzymuje od systemu operacyjnego informacje o środowisku, w którym ten program będzie działać.

[Procedura `_start`](https://stackoverflow.com/questions/29694564/what-is-the-use-of-start-in-c) jest entry pointem programu, tzn. adres tego symbolu jest adresem, do którego wykonywany jest skok na początku wykonania programu. Zwykle jest ona zawarta w pliku `crt0.o`, który zawiera kod startowy dla środowiska C runtime.

`syscall` to procedura jądra, działa ona podobnie do standardowych funkcji, jednak zamiast `jump`, używa procedur dla jądra. Jeżeli wynik w `%rax` będzie w przedziale `<-4095, -1>`, to `syscall` zwróci błąd wywołania systemowego.

### Zadanie 5
Przypomnij jak wygląda mechanizm **tłumaczenia adresów** bazujący na wielopoziomowej tablicy stron procesorów z rodziny x86-64. Przedstaw algorytm obliczania **adresu fizycznego** na podstawie **adresu wirtualnego** z uwzględnieniem uprawnień dostępu. Jaką rolę w procesie tłumaczenia odgrywa **pamięć TLB**?

**Tłumaczenie adresów** to proces znajdowania adresu fizycznego z adresu wirtualnego. Architektura x86-64 używa czteropoziomowej tablicy stron:

| `63-48` | `47-39` | `38-29` | `29-21` | `20-12` | `11-0` |
| ------- | ------- | ------- | ------- | ------- | ------ |
|         | level 4 | level 3 | level 2 | level 1 | offset |

16 najbardziej znaczących bitów adresu jest nieużywane.

Algorytm translacji dla pojedynczej tabeli stron wygląda następująco:
![](https://i.imgur.com/hmJlvbK.png)

A dla stron procesów z rodziny x86-64: używamy rejestru `%cr3` przechowującego adres fizyczny czwartego poziomu tabeli stron (aby ostatecznie dostać się do adresu, który chcemy). Algorytm ten przebiega w następujący sposób:
1. Używamy `%cr3` i indeksu z L4 z wirtualnego adresu aby dotrzeć do poziomu L3 tabeli stron.
2. Używamy adresu tabeli stron L3 i indeksu L3 aby dotrzeć do poziomu L2 tabeli stron.
3. Używamy adresu tabeli stron L2 i indeksu L2 aby dotrzeć do poziomu L1 tabeli stron.
4. Używamy adresu tabeli stron L1 i indeksu L1 aby dotrzeć do żądanego adresu fizycznego.
5. Używamy żądanego adresu fizycznego i przesunięcia (offset) do uzyskania rzeczywistego adresu fizycznego.

**Bufor TLB** *(ang. translation lookaside buffer)* (czasami **pamięć asocjacyjna**) to urządzenie sprzętowe służące do odwzorowywania adresów wirtualnych na fizyczne bez konieczności sięgania do tabeli stron. Zwykle jest ono zlokalizowane wewnątrz jednostki MMU i składa się z niewielkiej liczby pozycji - każda pozycja zawiera informacje dotyczące jednej strony.

### Zadanie 6 TODO
Uruchom program `1_ls` pod kontrolą narzędzia `ltrace -S`. Na podstawie śladu wykonania programu zidentyfikuj, które z **wywołań systemowych** są używane przez procedury: `opendir`, `readdir`, `printf` i `closedir`. Do czego służy wywołanie systemowe `brk`? Używając debuggera `gdb` i polecenia `catch syscall brk` zidentyfikuj, która funkcja używa `brk`.

**Wywołanie systemowe** to interfejs pomiędzy wykonywanym programem, a jądrem systemu operacyjnego. Funkcje systemowe wywoływane są przez specjalny mechanizm, wspierany przez dany procesor. Są to między innymi dostęp do systemu plików, komunikacja międzyprocesowa, uruchamianie innych programów, sterowanie urządzeniami systemowymi, obsługiwanie komunikacji sieciowej.

Najpierw robię `make` w folderze, a później wywołuję poniższe polecenie:
```
$ ltrace -S ./1_ls libapue/
```
Na wyjściu otrzymuję mniej więcej taki wydruk (wycięte niepotrzebne linijki):
```
opendir("libapue/" <unfinished ...>
SYS_openat(0xffffff9c, 0x7fff476df52f, 0x90800, 0)               = 3
readdir(0x55ae2a86a260 <unfinished ...>
SYS_getdents64(3, 0x55ae2a86a290, 0x8000, -2630)                 = 144
SYS_write(1, "libapue.a\n", 10libapue.a
)                                  = 10
<... puts resumed> )                                             = 10
readdir(0x55ae2a86a260)                                          = 0x55ae2a86a2b0
puts("error.o" <unfinished ...>
SYS_write(1, "error.o\n", 8error.o
)                                     = 8
<... puts resumed> )                                             = 8
readdir(0x55ae2a86a260)                                          = 0x55ae2a86a2d0
puts(".." <unfinished ...>
SYS_write(1, "..\n", 3..
)                                          = 3
<... puts resumed> )                                             = 3
readdir(0x55ae2a86a260)                                          = 0x55ae2a86a2e8
puts("error.c" <unfinished ...>
SYS_write(1, "error.c\n", 8error.c
)                                     = 8
<... puts resumed> )                                             = 8
readdir(0x55ae2a86a260)                                          = 0x55ae2a86a308
puts("." <unfinished ...>
SYS_write(1, ".\n", 2.
)                                           = 2
closedir(0x55ae2a86a260 <unfinished ...>
SYS_close(3)                                                     = 0
```
Wywnioskować więc można, że przez wywołanie funkcji:
* `opendir` jest wywoływane `SYS_openat`
* `readdir` jest wywoływane `SYS_getdents64`
* `printf` jest wywoływane `SYS_write`
* `closedir` jest wywoływane `SYS_close`

Wywołanie systemowe `SYS_brk` powoduje zmianę wielkości sterty programu, dzięki temu zmieniana jest lokalizacja przerwania programu (koniec segmentu danych procesu). Zwiększenie wartości `brk` powoduje przydzielenie pamięci do procesu, a jej zmniejszenie zwalnia tę pamięć. Funkcje takie jak `malloc`, `realloc`, `free` niejawnie zmieniają wartość `brk`. 

Po włączeniu `1_ls` w `gdb` i ustawieniu `catch syscall brk` otrzymamy:
```
Catchpoint 1 (call to syscall brk), __brk (addr=addr@entry=0x0) 
    at ../sysdeps/unix/sysv/linux/x86_64/brk.c:31
31	../sysdeps/unix/sysv/linux/x86_64/brk.c: No such file or directory.
```


### Zadanie 7 TODO
Pod kontrolą narzędzia `strace` uruchom program `2_cat` korzystający bezpośrednio z wywołań systemowych do interakcji ze **standardowym wejściem i wyjściem**. Pokaż, że program oczekuje naodczyt na deskryptorze pliku 0 i pisze do pliku o deskryptorze 1. Naciśnij kombinację klawiszy `CTRL+D` kończąc wejściowy strumień danych – co zwróciło `read`? Zmodyfikuj program tak, by czytał z plikupodanego w linii poleceń. Co się stanie, jeśli przekażesz **ścieżkę** do katalogu zamiast do pliku regularnego?