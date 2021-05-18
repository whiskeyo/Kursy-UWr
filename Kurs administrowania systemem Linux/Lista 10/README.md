# Kurs administrowania systemem Linux (L10)

## Lista zadań nr 10

### Zadanie 1

```shell=
$ touch disk.img 
$ truncate disk.img -s 500008960000
# fdisk -l disk.img
    Disk disk.img: 465.7 GiB, 500008960000 bytes, 976580000 sectors
    Units: sectors of 1 * 512 = 512 bytes
    Sector size (logical/physical): 512 bytes / 512 bytes
    I/O size (minimum/optimal): 512 bytes / 512 bytes
# fdisk -c=dos disk.img
    n p 1 63 67108863           <- partycja główna nr 1 typu 0x0c (FAT32 LBA)
    n p 2 67108864 486539263    <- partycja główna nr 2 typu 0x83 (Linux)
    n e 4 629145600 976579999   <- partycja główna nr 4 typu 0x05 (rozszerzona)
    
    t 1 0x0c    <- zmiana typu na FAT32 LBA
    t 2 0x83    <- zmiana typu na Linux (domyślna po utworzeniu poprzez n p)
    t 4 0x05    <- zmiana typu na Extended (domyślna po utworzeniu poprzez n e)

    w           <- zapisanie danych na dysk i wyjście
# fdisk disk.img
    n l 629147648 713033727     <- partycja logiczna nr 5 typu 0x83 (Linux)
    n l 975175680 976199679     <- partycja logiczna nr 6 typu 0x82 (Linux swap)
    
    t 5 0x83    <- zmiana typu na Linux (domyślna po utworzeniu poprzez n l)
    t 6 0x82    <- zmiana typu na Linux swap / Solaris
    
    w           <- zapisanie danych na dysk i wyjście
# fdisk -l disk.img
    Disk disk.img: 465.7 GiB, 500008960000 bytes, 976580000 sectors
    Units: sectors of 1 * 512 = 512 bytes
    Sector size (logical/physical): 512 bytes / 512 bytes
    I/O size (minimum/optimal): 512 bytes / 512 bytes
    Disklabel type: dos
    Disk identifier: 0xe1e1402c

    Device     Boot     Start       End   Sectors   Size Id Type
    disk.img1              63  67108863  67108801    32G  c W95 FAT32 (LBA)
    disk.img2        67108864 486539263 419430400   200G 83 Linux
    disk.img4       629145600 976579999 347434400 165.7G  5 Extended
    disk.img5       629147648 713033727  83886080    40G 83 Linux
    disk.img6       975175680 976199679   1024000   500M 82 Linux swap / Solaris
# sudo losetup -fP disk.img
$ lsblk
    ...
    loop9       7:9    0 465.7G  0 loop 
    ├─loop9p1 259:0    0    32G  0 part 
    ├─loop9p2 259:1    0   200G  0 part 
    ├─loop9p4 259:2    0     1K  0 part 
    ├─loop9p5 259:3    0    40G  0 part 
    └─loop9p6 259:4    0   500M  0 part 
    ...
```

Po kolei:

1. Tworzymy obraz dysku.
2. Zmieniamy jego rozmiar na taki, który podaliśmy w bajtach (odpowiada on liczbie sektorów z zadania).
3. Sprawdzamy, czy rzeczywiście `truncate` zmienił rozmiar dysku na taki, jaki chcemy.
4. Tworzymy dysk z flagą DOS, żeby móc ignorować alignment (domyślnie możemy tworzyć sektory od 2048, a z `-c=dos` od 63), następnie wywołujemy wypisane polecenia.
5. Wywołujemy kolejny raz `fdisk`, musieliśmy zapisać poprzednią tabelę partycji, aby umożliwić sobie edycję partycji rozszerzonej i tworzenie partycji logicznych.
6. Sprawdzamy czy rzeczywiście stworzone zostały wszystkie partycje.
7. Wymuszamy jądro do zeskanowania tablicy partycji na nowo utworzonym urządzeniu `loop`.
8. Wyszukujemy urządzenia `loopX` odpowiadającego naszemu dyskowi.

Główna różnica pomiędzy `fdisk` a `parted` jest taka, że `fdisk` wspiera dyski tylko do 2TB, a `parted` jest w stanie partycjonować dyski większe. Spowodowane jest to przez to, że `fdisk` obsługuje tylko MBR, który nie pozwala na utworzenie więcej niż czterach partycji głównych, a GPT jest znacznie nowszym rozwiązaniem.

Polecenie `losetup` służy do sterowania urządzeniami `loop`, a dokładniej kojarzenia ich z plikami zwykłymi lub urządzeniami blokowymi, do odłączania urządzeń `loop` i sprawdzania ich stanu. `--find` szuka pierwszego nieużywanego urządzenia `loop`, `--partscan` wymusza na jądrze skanowanie tablic partycji na nowo dodanym urządzeniu loop, a `--show` (w użyciu z `--find` i nazwą pliku będącą argumentem) wyświetla nazwę przypisanego urządzenia `loop`.

Program `partx` służy do poinformowania jądra o obecności i numeracji partycji na dysku. Po podaniu urządzenia lub obrazu dysku, `partx` próbuje sprasować tablicę partycji i wypisać ich zawartość, ponadto może polecić jądru dodanie/usunięcie partycji. Przykładowym użyciem `partx` jest np. `sudo partx --show disk.img`, co wyświetli początek i koniec dysku, ilość sektorów, rozmiar, nazwę, jak i UUID partycji.

Program `partprobe` służy do poinformowania systemu operacyjnego o zmianach w tabeli partycji, niestety dokumentacja w `man` jest strasznie uboga, podobnie po użyciu `--help` niewiele się dowiemy. Jeśli nie podamy żadnego urządzenia jako argument do `partprobe`, to wszystkie partycje zostaną "zbadane".

Program `kpartx` służy do utworzenia map urządzeń z tabeli partycji, jest "potężniejszym" narzędziem od `partx`, jako że umożliwia tworzenie wpisów, których jądro domyślnie nie wspiera przy partycjonowaniu, np. `multipath`.

### Zadanie 2

To rozwiązanie jest błędne, nie powinienem tworzyć dwóch partycji Extended, tylko skorzystać z EBR i zrobić to poprawnie.

```=
0000000000    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
*
00000001b0    00 00 00 00  00 00 00 00    46 3f f9 21  00 00 00 82
00000001c0    03 00 0c 2d  07 66 00 20    00 00 00 00  00 05 00 01
00000001d0    e2 a0 83 b6  8a 2c 00 00    00 60 00 00  a0 00 00 2d
00000001e0    08 66 05 57  f3 64 00 20    00 05 00 00  80 0c 00 56
00000001f0    42 15 83 01  e1 a0 00 00    00 40 00 00  00 20 55 aa
0000000200    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
*
0a004001b0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 4d
0a004001c0    28 66 83 d7  2f e8 00 08    00 00 00 00  20 00 00 62
0a004001d0    78 6b 05 98  a8 70 00 08    40 00 00 08  40 00 00 00
0a004001e0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
0a004001f0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 55 aa
0a00400200    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
*
0a805001b0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 83
0a805001c0    59 6b 83 98  a8 70 00 08    00 00 00 00  40 00 00 00
0a805001d0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
*
0a805001f0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 55 aa
0a80500200    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
*
fffffffff0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
```

Pierwszym krokiem jest znalezienie identyfikatora dysku. Znajduje się on w przedziale `00000001b9` do `00000001bc` i jest zapisany w formacie little-endian. W naszym przypadku jest to `46 3f f9 21`, a więc identyfikatorem dysku jest `0x21f93f46`.

Drugim krokiem jest znalezienie `55 aa`, a więc "magicznej liczby" kończącej informacje o partycjach. W naszym przypadku występuje ona kilka razy, dlatego możemy spodziewać się, że jakaś partycja jest rozszerzona, a w niej znajdują się partycje logiczne. Pozostałe interesujące nas informacje znajdują się zawsze w następujących pozycjach: cały wiersz przed `55 aa` oraz dwa ostatnie bajty w wierszu powyżej to jedna partycja. Warto wspomnieć, że MBR *(ang. master boot record)* może zawierać maksymalnie 4 partycje, dlatego dane przed 4 partycjami i `55 aa` nas niezbyt interesują.

Mając znalezione wszystkie partycje, skorzystamy z poniższej "budowy" wpisów. Dla ułatwienia zrozumienia, `AA` oznacza jedną informację, podobnie `BB BB BB BB` oznacza jedną informację (a więc jeden symbol powtórzony niezależnie ile razy = jedna informacja). Wtedy wpisy jednej partycji są następujące:

```
xx xx xx xx  xx xx xx xx    xx xx xx xx  xx xx AA BB
CC DD EE FF  GG HH II II    II II JJ JJ  JJ JJ xx xx
```

i oznaczają (niewypisane nie są aż tak ważne):

* `AA` - flaga bootowania (`0x00` = non bootable, `0x80` = bootable)
* `EE` - typ partycji
* `II II II II` - początek adresu LBA *(relative sectors)*
* `JJ JJ JJ JJ` - rozmiar partycji (liczba sektorów w little-endian)

Korzystając z tych informacji, rozpiszmy najpierw pierwszy podział:

```=
00000001b0    00 00 00 00  00 00 00 00    46 3f f9 21  00 00 00 82 
00000001c0    03 00 0c 2d  07 66 00 20    00 00 00 00  00 05 00 01 
00000001d0    e2 a0 83 b6  8a 2c 00 00    00 60 00 00  a0 00 00 2d 
00000001e0    08 66 05 57  f3 64 00 20    00 05 00 00  80 0c 00 56
00000001f0    42 15 83 01  e1 a0 00 00    00 40 00 00  00 20 55 aa
```

|Start|End|Size (in sectors)|Zawartość|
|---|---|---|---|
|`0x00002000`|`0x05002000`|`0x05000000`|`0x0c`|
|`0x60000000`|`0x60a00000`|`0x00a00000`|`0x83`|
|`0x05002000`|`0x11802000`|`0x0c800000`|`0x05`|
|`0x40000000`|`0x60000000`|`0x20000000`|`0x83`|

Suma wszystkich sektorów to granica ostatniego sektora, czyli łącznie mamy `1 621 098 496 + 1` sektorów, stąd mamy rozmiar całego dysku (który podajemy w `truncate`) wynoszący `1 621 098 497 * 512 = 830 002 430 464` bajtów.

Teraz zapiszmy te dane w sposób bardziej przystępny dla ludzi, dodatkowo numer początkowy będzie o 1 większy niż numer końcowy z wiersza poprzedniego, partycję uporządkuję względem sektorów:

|Start|End|Zawartość|
|---|---|---|
|`8 192`|`83 894 272`|FAT32 LBA|
|`83 894 273`|`293 609 472`|rozszerzona|
|`1 073 741 825`|`1 610 612 736`|Linux|
|`1 610 612 737`|`1 621 098 496`|Linux|

Widzimy jakiś nieprzydzielony obszar między FAT32 LBA a partycją rozszerzoną, ale takie obszary będę pomijał dla wygody czytania. Przechodzimy więc do drugiego fragmentu:

```=
0a004001b0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 4d
0a004001c0    28 66 83 d7  2f e8 00 08    00 00 00 00  20 00 00 62
0a004001d0    78 6b 05 98  a8 70 00 08    40 00 00 08  40 00 00 00
0a004001e0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
0a004001f0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 55 aa
```

Dwie ostatnie partycje przed `55 aa` są wyzerowane, a więc nie istnieją.

|Start|End|Size (in sectors)|Zawartość|
|---|---|---|---|
|`0x00000800`|`0x00200800`|`0x00200000`|`0x83`|
|`0x00400800`|`0x00801000`|`0x00400800`|`0x05`|

I w ludzkim formacie:

|Start|End|Zawartość|
|---|---|---|
|`2 048`|`2 099 200`|Linux|
|`4 196 353`|`8 392 704`|rozszerzona|

Jednak należy do tych wartości dodać początkowy sektor partycji rozszerzonej, stąd będziemy mieli takie przedziały sektorów:

|Start|End|Zawartość|
|---|---|---|
|`83 896 321`|`85 993 473`|Linux|
|`88 090 626`|`92 286 977`|rozszerzona|

Na sam koniec zostaje nam to:

```=
0a805001b0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 83
0a805001c0    59 6b 83 98  a8 70 00 08    00 00 00 00  40 00 00 00
0a805001d0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
* (tutaj 0a705001e0, czyli też wyzerowane)
0a805001f0    00 00 00 00  00 00 00 00    00 00 00 00  00 00 55 aa
0a80500200    00 00 00 00  00 00 00 00    00 00 00 00  00 00 00 00
```

Mamy więc jedną partycję:

|Start|End|Size (in sectors)|Zawartość|
|---|---|---|---|
|`0x00000800`|`0x00040800`|`0x00040000`|`0x83`|

Czyli:

|Start|End|Zawartość|
|---|---|---|
|`2 048`|`264 192`|Linux|

Przechodzimy do tworzenia dysku, podobnie do zadania pierwszego:

```shell=
$ touch disk2.img
$ truncate disk2.img -s 830002430464
# fdisk -l disk2.img
    Disk disk2.img: 773 GiB, 830002429952 bytes, 1621098496 sectors
    Units: sectors of 1 * 512 = 512 bytes
    Sector size (logical/physical): 512 bytes / 512 bytes
    I/O size (minimum/optimal): 512 bytes / 512 bytes
# fdisk disk2.img
    x i 0x21f92f46 r                <- zmiana identyfikatora dysku

    n p 1 8192 83894272             <- pierwsza partycja (FAT32 LBA)
    n e 2 83894273 293609472        <- druga partycja (rozszerzona)
    n p 3 1073741825 1610612736     <- trzecia partycja (Linux)
    n p (4) 1610612737 1621098496   <- czwarta partycja (Linux)

    t 1 0x0c    <- zamiana typu partycji 1. na FAT32 LBA

    n 83896321 85993473 <- piąta partycja (pierwsza logiczna) (Linux)
    n 88090626 92286977 <- szósta partycja (druga logiczna) (rozszerzona)

    t 6 0x5     <- zamiana typu partycji 6. na Extended zwraca błąd:
                   Extended partition already exists.
```

No i tu następuje problem, podejrzeć zawartość tego obrazu można poprzez

```
hexdump -C -n 512 disk2.img
```