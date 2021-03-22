# Kurs administrowania systemem Linux

## Lista zadań nr 1

### Zadanie 1

:::info
Dodaj do pliku `~/.bashrc` definicje (w postaci aliasów bądź funkcji) kilku prostych, ale często przydatnych poleceń:

* `ll` — wypisuje zawartość (podanego lub bieżącego) katalogu korzystając z polecenia `ls` z opcjami `-lAFbhv` (wyjaśnij, co znaczą te opcje) i w celu stronicowania listingu korzysta z pagera `less` z opcjami `-XER` (wyjaśnij, co one znaczą).
* `gentmp` — wypisuje na standardowe wyjście napis `tmp-YYYYMMDDhhmmss` zawierający w miejscu odpowiednich liter bieżący czas.
* `genpwd` — wypisuje na standardowe wyjście napis długości $32$ znaków wygenerowany losowo za pomocą `/dev/urandom`. Znaki powinny pochodzić ze zbioru `[3-9A-HJ-NP-Z]` (pomijamy znaki, które można pomylić z innymi, np. `0O` itp.)
:::

Pierwsza funkcja na wypisanie zawartości i przekierowanie wyjścia do pagera `less`. Konieczne jest wykorzystanie funkcji, jako że alias nie może przyjmować argumentów, a w przypadku `ls` argumentami może być dowolna ilość katalogów lub ich brak. `echo` służy do wypisania wywoływanego polecenia, żeby łatwiej było sprawdzić, czy polecenie jest poprawne. O dziwo bez dopisku `function` pojawiają się błędy parsowania, ale jedynie wtedy, gdy nazwa funkcji to `ll`. Dla `lltest() {...}` wszystko już działało poprawnie.

```bash=
function ll() {
    echo "ls $@ -lAFbhv | less -XER"
    ls $@ -lAFbhv | less -XER
}
```

Opcje `ls` oznaczają: 
* `-l` - wypisanie wszystkich danych w kolumnach (*long listing format*),
* `-A` - wypisanie prawie wszystkich plików, oprócz `.` (bieżący katalog) oraz `..` (katalog niższy), 
* `-F` - dopisanie znaków takich jak `*`, `/`, `=`, `>`, `@`, `|` do odpowiednich wpisów (np. `/` do katalogów), 
* `-b` - wypisywanie stringów ze znakiem ucieczki *(ang. escape character)*, czyli z `\`. Polega to na tym, że pliki i katalogi wieloczłonowe są zapisywane w stylu `folder\ testowy`, a nie `'folder testowy'` (*backslash* przed spacją, brak apostrofów "opakowujących" string),
* `-h` - wypisywanie rozmiarów w łatwy do odczytania sposób, np. `254M`, `3G`,
* `-v` - sortowanie po wersjach

Opcje dla `less` oznaczają:

* `-X` - wyłącza wysyłanie termcapa przy inicializacji i deinicjalizacji stringów do terminala, dzięki czemu nieporządane akcje, takie jak czyszczenie ekranu, nie są wykonywane.
* `-E` - sprawia, że `less` automatycznie się kończy po pierwszym napotkaniu EOF,
* `-R` - sprawia, że znaki surowe *(ang. raw)* są wyświetlane, ale w przeciwieństwie do `-r`, wyświetla jeszcze kolory

Alias drugi na generowanie stringa z aktualną datą i godziną: 

```bash
alias gentmp="date +\"tmp-%Y%m%d%H%M%S\""
```

Alias do generowania haseł:

```bash
alias genpwd="cat /dev/urandom | tr -dc '3-9A-HJ-NP-Z' | head -c 32"
alias genpwd2="cat /dev/urandom | tr -dc '3-9A-HJ-NP-Z' | fold -w X | head -n Y"
             # X oznacza długość tworzonego hasła, Y to liczba haseł do wygenerowania
```

Wywołujemy `cat` na generatorze, szukamy w nim znaków z określonego zbioru (polecenie `tr`), a następnie dzięki `head -c` wyciągamy odpowiednią dla nas liczbę znaków. Dokładniej, użycie polecenia `tr -dc [zbiór]` sprawia, że usuwamy (`-d`) wszystkie znaki z dopełnienia (`-c`) zbioru. 

### Zadanie 2

:::info
Zapoznaj się z dokumentacją programu `grep(1)` i przygotuj krótkie omówienie wyrażeń regularnych oraz podstawowych opcji programu.
:::

`grep` to program służący do wyszukiwania ciągu znaków pasujących do podanego wyrażenia regularnego i wypisywania linii zawierających te ciągi. Wywołanie `grep`:

```bash
grep [opcje] [wzorzec] [pliki]
```

Najprzydatniejszymi i najczęściej używanymi opcjami (wg mnie) są:

* `-i` - ignorowanie wielkości znaków w tekście, a więc szukając wzorca `bar`, pokażą się nam również wyszukane fragmenty `bAr`, `BAR`, `bAR`, itd.
* `-w` - szukanie jedynie całych wyrazów, więc gdy szukamy wzorca `bar`, a w tekście znajdują się fragmenty `barfoo`, `foobar`, `fobaro`, `bar`, to wyszukamy tylko `bar`. Możemy skorzystać z `egrep` z tą samą funkcją, aby wyszukać alternatywy dwóch słów, np. `bar|foo`,
* `-n` - poprzedzenie każdego zwróconego wyniku numerem linii, w której jest szukany wzorzec,
* `-v` - odwrócenie szukanego wzorca, więc jeśli wpiszemy wzorzec `bar`, to pokaże nam wszystkie wyniki, w których tego wzorca nie znajdziemy,
* `-r` - wyszukanie wzorca rekursywnie, w bieżącym katalogu jak i wszystkich podkatalogach. Jeśli w katalogu znajdują się dowiązania symboliczne i również w nich chcemy przeszukiwać zawartość, to możemy skorzystać z opcji `-R`.

Wzorcem, a więc wyrażeniem regularnym, jest wzór opisujący pewien zbiór stringów. 

Zbiór wybranych znaków można zapisać w nawiasach kwadratowych, np. `[ABCDEF]` sprawi, że będziemy szukać jednoznakowego wzorca składającego się z jednej z tych liter. Można skorzystać również z zapisu `[A-F]`, jednak w zależności od ustawień regionalnych *(ang. locale)* może to sprawić, że za `[A-F]` zostanie postawione `[AbBcCdDeEfF]`. Aby temu zapobiec, można dopisać `LC_ALL` z wartością `C` do listy zmiennych środowiskowych. 

Najczęściej używane zbiory mają jednak predefiniowane aliasy, takie jak `[:alnum:]` (znaki alfanumeryczne), `[:digit:]` (cyfry), `[:lower:]`, `[:upper:]` i wiele więcej.

Aby nasze wyszukiwanie było znacznie bardziej rozbudowane, możemy skorzystać z operatorów powtórzeń, takich jak:

* `?` - zbiór jest opcjonalny, a jeśli wystąpi, to tylko raz,
* `*` - zbiór może wystąpić dowolną ilość razy (nawet zero),
* `+` - zbiór musi wystąpić co najmniej raz,
* `{n}` - zbiór wystąpi dokładnie `n` razy,
* `{n,}` - zbiór wystąpi co najmniej `n` razy,
* `{,m}` - zbiór wystąpi najwięcej `m` razy (rozszerzenie GNU),
* `{n,m}` - zbiór wystąpi co najmniej `n` razy i najwięcej `m` razy.

Wyszukiwanie można rozpocząć od początku linii, używając znaku karety `^`, lub zakończyć na końcu linii, korzystając ze znaku dolara `$`.

Oczywiście ten opis jest dość krótki, a całe wyrażenia regularne mogą składać się z wielu znaków. Wzorcem na wyszukanie godzin w formacie `HH:MM:SS` lub `HH:MM` jest:

```
^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])(:[0-5][0-9])?$ - koniec linii
| |                  |   |        |  |         | |
| --------------------   ----------  ----------- |
|zbiór godzin, zawiera   zbiór minut zbiór sekund|
|wszystkie możliwości                            |
|     od 00 do 23                            opcjonalny
|                                              wyraz
początek linii
```

Aby sprawdzić jakie stringi pokrywa nasz wzorzec, polecam stronę [regex101](https://regex101.com/), na której możemy stworzyć sobie nawet własne testy, a wszystkie fragmenty wzorców są rozpisane.

### Zadanie 3

:::info
Zapoznaj się z dokumentacją programu `find(1)` i przygotuj krótkie omówienie jego podstawowych opcji.
:::

Program `find` służy do wyszukiwania plików w katalogach. Podobnie jak `grep`, wykorzystuje możliwość szukania zgodnego z wzorcami. Wyszukuje on pliki od wybranego katalogu w dół, czyli jeśli będziemy chcieli wyszukać pliki w całym systemie, to skorzystamy ze ścieżki `/`. Gdy mamy np. taką strukturę plików:

```
main
 |
 +--foo
 |   +--foo1
 |   +--foo2
 |
 +--bar
     +--bar1
     +--bar2
```
To wyszukanie `find main` sprawi, że zostaną sprawdzone wszystkie podkatalogi, tzn. `foo`, `foo/foo1`, `foo/foo2`, `bar`, `bar/bar1`, `bar/bar2`. Dla `find main/foo` będą to `foo`, `foo/foo1`, `foo/foo2`, a dla `find main/foo/foo1` tylko pliki znajdujące się w `foo1`.

Samo polecenie wywołujemy w taki sposób:

```bash
find [katalog] [opcje (kryteria selekcji)] [co z tymi plikami zrobić?]
```

Wybór katalogów jest opisany powyżej, kryteriami selekcji są między innymi:

* `-name` - wyszukuje plik o określonej nazwie,
* `-type` - wyszukuje pliki o wybranym typie: `d` to katalog, `f` to plik normalny, `b` to plik blokowy, `c` to plik znakowy, a `l` to dowiązanie symboliczne,
* `-path 'pattern'` - wyszukuje pliki, których ścieżka dostępu pasuje do wzorca,
* `-links` - wyszukuje pliki z daną liczbą dowiązań do plików,
* `-size` - wyszukuje pliki o danej wielkości,
* `-user` - wyszukuje pliki należące do wybranego użytkownika,
* `-perm` - wyszukuje pliki, które mają określone prawa dostępu,
* `-atime` - wyszukuje pliki, które otwierane były w ostatnich dniach,
* `-mtime` - wyszukuje pliki, które były modyfikowane w ostatnich dniach.

Ze znalezionymi plikami można wykonać kilka akcji, np.

* `-print` - przekazuje na standardowe wyjście nazwę odnalezionego pliku oraz jego pełną ścieżkę dostępu (zamiast tej opcji możemy przekazać wynik w potoku do np. `more`),
* `-exec polecenie {}\;` - uruchamia polecenie dla odnalezionego pliku,
* `-ok. polecenie {}\;` - potwierdza uruchomienie polecenia dla odnalezionego pliku.

Korzystając z wyżej opisanych opcji, możemy wywołać np. takie polecenia:

```bash=
$ find . -type d # przeszukaj tylko katalogi w bieżącym katalogu
$ find . -name foo -print # wyszukaj wszystkie pliki o nazwie foo
$ find ~ -name 'foo*' -print # wyszukaj wszystkie pliki w katalogu
                             # domowym rozpoczynające się od foo
$ find . -type d -exec chmod 770 {}\; # uruchom na wszystkich katalogach
                                      # polecenie chmod 770
$ find . -type d -ok. chmod 770 {}\;  # jak wyżej, ale pytaj o zgodę
```

### Zadanie 4

:::info
Zapoznaj się z dokumentacją perlowego skryptu `rename` i przygotuj krótkie omówienie jego użycia.
:::

`rename` to polecenie do zmiany nazwy plików. Domyślnie, pomimo tego, że w `man rename` widziałem manual opcji `rename`, to nie był to perlowy skrypt, dlatego musiałem go zainstalować przez `apt install rename`. Wywołujemy je następująco:

```bash
rename [wyrażenie] [pliki]
```

Opcjami opisanymi w manualu są:

* `-v` - pokaż, w których plikach została zmieniona nazwa,
* `-n` - nie rób żadnych zmian, jedynie wypisz nazwy plików, których nazwa powinna zostać zmieniona, ale tej nazwy nie zmieniaj,
* `-0` - używaj `\0` jako separatora rekordów w trakcie czytania z `STDIN`,
* `-f` - pozwól na nadpisanie istniejących plików,
* `-d` - nie zmieniaj nazwy katalogu, zmień jedynie nazwę pliku będącą częścią ścieżki

Bardzo podstawowym wywołaniem może być polecenie służące do zamiany wszystkich dużych liter na małe we wszystkich plikach w katalogu.

```bash
rename 'y/A-Z/a-z/' *
```

Przykładowo, weźmy katalog z plikami `1.prog`, `2.prog`, `3.prog`, `4.prog`. Za pomocą `mv`, moglibyśmy zmienić kolejno nazwę każdego pliku osobno, albo napisać jakiś skrypt, który by za nas to zrobił. Chcemy zmienić rozszerzenie każdego pliku `*.prog` na `*.abc`. Wykorzystujemy do tego polecenie:

```bash
rename 's/.prog/.abc/' *.prog --+
        |    +--+ |             |
    substitute  | +-- za co?  pliki
     (podstaw)  | 
          co podstawić?
```

Możemy też się pozbyć jakiejś części pliku. Załóżmy, że każdy plik ma prefiks `abc_` oraz sufiks `_def`. Wykorzystajmy więc dwukrotnie `rename` i podstawmy pusty string za `abc_` i `_def`:

```bash=
rename 's/abc_//' *
rename 's/_def//' *
```

Podobnie jak w przypadku innych wyrażeń regularnych, możemy wykorzystać operatory `^` oraz `$` do zaznaczenia początku lub końca nazwy, aby nie zamienić jakiejś części pliku, której nie chcieliśmy zmieniać.


### Zadanie 5 TODO

:::info
Podczas instalacji większości pakietów w Debianie/Ubuntu jest tworzony podkatalog katalogu `/usr/share/doc/` o takiej samej nazwie, jak nazwa pakietu, zawierający podstawową dokumentację pakietu. W większości przypadków zawiera on plik `changelog.Debian.gz`, zawierający chronologiczną listę zmian dokonanych w tym pakiecie. Napisz odpowiednie polecenia, które wypiszą następujące informacje:

* Listę zainstalowanych pakietów, które nie posiadają własnego podkatalogu w `/usr/share/doc/`.
* Listę podkatalogów katalogu `/usr/share/doc/`, których nazwy nie są nazwami żadnego zainstalowanego pakietu. Przy każdym z takich podkatalogów wypisz nazwę pakietu, który jest jego właścicielem.
* Listę pakietów posiadających własny podkatalog w katalogu /usr/share/doc/, który jednak nie zawiera pliku `changelog.Debian.gz`.
* Listę pakietów posiadających własny plik `changelog.Debian.gz`, który zawiera tylko jeden wpis (zwykle *Initial release*).
* Liczbę wystąpień słowa bash (zapisanego małymi lub wielkimi literami) w pliku `/usr/share/doc/bash/INTRO.gz`
:::

Aby wypisać wszystkie zainstalowane pakiety, możemy skorzystać z `dpkg-query` z flagami `-W` (lub `--show`) pokazujące wszystkie pakiety oraz `-f` do poprawnego sformatowania wyjścia:

```bash
dpkg-query -W -f '${package}\n'
```

Lista zainstalowanych pakietów, które nie posiadają własnego podkatalogu w `/usr/share/doc/`:

```bash=
#!/bin/bash

ls /usr/share/doc > docs
apt list --installed | sed 's=/.\+==g' > installed
grep -v -f docs installed > installed_not_in_docs
```

To rozwiązanie jest dość brzydkie, do tego niepotrzebnie zapisujemy jakieś rzeczy do plików (co w systemach *read-only* byłoby niemożliwe). Skorzystajmy więc z polecenia:

```bash 
apt list --installed | sed 's=/.\+==g' 2> /dev/null | while read x; do if [ ! -d /usr/share/doc/$x ]; then echo $x; fi; done
```

Lista podkatalogów katalogu `/usr/share/doc/`, których nazwy nie są nazwami żadnego zainstalowanego pakietu. Przy każdym z takich podkatalogów wypisz nazwę pakietu, który jest jego właścicielem:

```bash=

```

Lista pakietów posiadających własny podkatalog w katalogu ``/usr/share/doc/``, który jednak nie zawiera pliku `changelog.Debian.gz`:

```bash=

```

Lista pakietów posiadających własny plik `changelog.Debian.gz`, który zawiera tylko jeden wpis (zwykle *Initial release*):

```bash=

```

Liczba wystąpień słowa bash (zapisanego małymi lub wielkimi literami) w pliku `/usr/share/doc/bash/INTRO.gz`:

```bash=

```

### Zadanie 6

:::info
Biblioteka współdzielona to plik, którego nazwa pasuje do wzorca `.*\.so\(\.[0-9]\+\)\?$`. Napisz odpowiednie polecenia, które wypiszą następujące informacje:

* Listę wszystkich bibliotek współdzielonych zainstalowanych w Twoim systemie.
* Listę dowiązań symbolicznych do bibliotek współdzielonych zainstalowanych w Twoim systemie.
* Liczbę, sumaryczny rozmiar w bajtach i średni rozmiar wszystkich bibliotek współdzielonych zainstalowanych w Twoim systemie.
* Listę (uporządkowaną i bez powtórzeń) wszystkich katalogów, w których występują biblioteki współdzielone.
* Listę (uporządkowaną i bez powtórzeń) wszystkich katalogów, w których występują dowiązania symboliczne do bibliotek współdzielonych zainstalowanych w Twoim systemie.
:::

Lista wszystkich bibliotek współdzielonych:

```bash
find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?$"
```

Lista dowiązań symbolicznych do bibliotek współdzielonych:

```bash
find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?$" -type l 
# -exec echo symlinks \;| uniq -c
```

Wyszukujemy wszystkie biblioteki współdzielone jak w pierwszym przykładzie, ale wyszukanie zawężamy do dowiązań symbolicznych (`-type l`). Jeśli chcemy zliczyć wystąpienia dowiązań symbolicznych, to wywołujemy `echo symlinks`, które potokiem przekierowujemy do `uniq -c`, zliczającego powtórzone linie. W moim przypadku jest to ~4600.

Sumaryczny rozmiar w megabajtach wszystkich bibliotek współdzielonych (to jeszcze nie jest do końca gotowe, bo `du` nie jest w stanie obsłużyć tak dużej listy rozmiarów, dlatego na wyjściu jest lista kilku rozmiarów, które trzeba dodać, ponadto chcemy rozmiar w bajtach):

```bash 
find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?$" -exec du -ch {} + | grep total$
```

Podobnie do polecenia wyżej, jednak korzystając z kalkulatora `bc`:

```bash
{ find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?$" -printf "%s+"; echo 0; } | bc
```

Najłatwiej i najszybciej, korzystając z `awk`:

```bash
find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?" -exec du -b {} \; 2> /dev/null | awk '{n+=$1} END {print n}'
```

Jeżeli chcemy uzyskać rozmiar w GB, przekazujemy wynik `bc` pipem do `numfmt --to=si`. Łatwiej jednak to wszystko załatwić skryptem, który jest znacznie bardziej czytelny, niż potencjalne jedno bardzo długie polecenie:

```bash=
#!/bin/bash

count () {
    size=$({ find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?" -printf "%s+"; echo 0; } | bc);
    items=$(find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?" -print | wc -l);
    avg=$(echo "$size / $items" | bc);

    echo "Size: $size bytes, items: $items, average size: $avg bytes"
}

count
```

Wartość `size` otrzymujemy następująco: wartość każdego wpisu znalezionego przez `find` traktujemy jako wartość liczbową, po czym dodajemy `+`, aby wszystkie wartości zsumować. To przekazujemy do kalkulatora `bc`, który zwraca nam wynik w bajtach. `items` uzyskujemy wypisując wszystkie znalezione biblioteki, a następnie licząc wszystkie linie poprzez `wc -l`.

Lista wszystkich katalogów, w których występują biblioteki współdzielone:

```bash=
#!/bin/bash

listdirs() {
    find / -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?$" | while read line; do
        echo $(dirname $line) >> input.txt
    done

    cat input.txt | sort | uniq > listdirs.txt
    rm input.txt
    echo "Listing directories finished!"
}

listdirs
```

Ponownie wykorzystujemy skrypt. Tym razem w pętli czytamy wszystkie wyniki z `find` i na każdym z nich wywołujemy `dirname`, aby uzyskać nazwę katalogu, w którym dana biblioteka się znajduje. Taką wartość zapisujemy do pliku tymczasowego. Gdy już wszystkie wyniki będziemy mieli odczytane, sortujemy ten plik za pomocą `sort`, wyodrębniamy tylko unikalne katalogi poprzez `uniq`, a następnie zapisujemy całość do pliku wyjściowego. Na koniec możemy usunąć pomocniczy plik i wypisać komunikat.

Aby uzyskać listę z dowiązaniami symbolicznymi, wystarczy w 4. linijce zamienić wyszukanie na:

```bash=4
find / -type l -regextype sed -regex ".*\.so\(\.[0-9]\+\)\?$" | while read line; do
```

A więc dodajemy jedynie wyszukiwanie po typie `-type l`, czyli dowiązaniu symbolicznym.

### Zadanie 7 TODO

:::info
Dane podsystemu obsługującego komunikaty w językach narodowych znajdują się
w katalogu `/usr/share/locale/`. Zawiera on podkatalogi o nazwach będących nazwami języków w kodowaniu `ISO 639-1`. Zawierają one m. in. podkatalog `LC_MESSAGES` zawierający pliki MO zawierające tłumaczenia komunikatów programów (dane dla biblioteki GNU Gettext wykorzystywanej przez te programy). Napisz odpowiednie polecenia, które wypiszą następujące informacje:

* Listę wszystkich nazw języków, dla których istnieje plik MO co najmniej jednego programu.
* Listę wszystkich nazw języków, dla których istnieją komunikaty programu `dpkg`.
* Listę wszystkich programów posiadających komunikaty w języku `pl`. 
 
Strony podręcznika systemowego `man(1)` w różnych językach znajdują się w katalogu `/usr/share/man`. Tradycyjnie znajdowały się tam tylko podkatalogi `man1–man8` odpowiadające ośmiu rozdziałom podręcznika. Obecnie znajdują się tam również katalogi o nazwach będących nazwami języków w kodowaniu `ISO 639-1` zawierające podkatalogi `man1–man8`. Podkatalogi te zawierają różnojęzyczne strony podręcznika zawarte w plikach o nazwach pasujących do `globa *.[1-8]`. Napisz odpowiednie polecenia, które
wypiszą następujące informacje:
* Dla każdego z ośmiu rozdziałów podręcznika listę wszystkich nazw języków, dla których istnieje co najmniej jedna strona dokumentacji w danym języku i w danym rozdziale. Pamiętaj że zamiast katalogu `en/man1` mamy katalog `man1/` itd.
* Dla każdego z ośmiu rozdziałów podręcznika listę wszystkich stron podręcznika w języku `pl`.
:::

Lista wszystkich nazw języków, dla których istnieje plik MO co najmniej jednego programu:

```bash
find /usr/share/doc -type d -name "??" -print | awk -F / '{print tolower($NF)}' | tr -dc "[:lower:]\n" | awk 'length>1' | sort -u 
```

Z komentarzami:

```bash=
find /usr/share/doc -type d -name "??" -print # wyszukaj katalogów
                                              # z dwuznakową nazwą
awk -F / '{print tolower($NF)}' # wyciągnij tylko ostatnią część
                                # ścieżki jako małe litery, wypisz to
tr -dc "[:lower:]\n"   # usuń wszystkie znaki niebędące małymi literami
                       # albo znakiem nowej linii 
awk 'length>1'         # wypisz tylko stringi dłuższe niż 1 znak
sort -u                # posortuj wszystkie wartości, usuwając duplikaty
```

### Zadanie 8 TODO

:::info
Aby szybko wyszukiwać wiele plików w drzewie katalogów możemy zamiast programu `find` użyć następującego sposobu: za pomocą polecenia `ls` z odpowiednimi opcjami zapisujemy ścieżki do wszystkich plików z pewnego drzewa katalogów w pliku tekstowym (zwykle o nazwie `ls-R`). Aby wyszukać plik przeszukujemy plik `ls-R` za pomocą programu `grep`. Zaprogramuj w Bashu proste narzędzie, które pozwoli wygenerować taki plik `ls-R`, a następnie w nim wyszukiwać. Zapoznaj się następnie z poleceniami `mlocate(1)` i `updatedb(8)` (czemu są lepsze niż stare dobre `locate`?) oraz z TEX-ową biblioteką KPathSea (w tym z poleceniem `kpsewhich(1)`) i przygotuj ich omówienie.
:::

### Zadanie 9

:::info
Zapoznaj się ze stroną podręcznika programu `sed(1)` i przygotuj jego krótkie omówienie.
:::

`sed` to narzędzie służące do edycji strumieni, filtrowania i transformowania tekstu. Wykorzystuje się go na strumieniu wejścia, a więc dla pliku lub przekierowania z potoku. Warto zwrócić uwagę na to, że wszystkie akcje są podejmowane dla każdego wiersza po kolei, a sam `sed` nie edytuje pliku - rezultaty są kierowane na `STDOUT`. Samo narzędzie udostępnia wiele opcji, a wywołać je można następującym poleceniem:

```bash 
sed [opcje] [wyrażenia/skrypt] [plik wejściowy]
```

Najbardziej przydatnymi opcjami są:

* `-n` - wypisywanie tylko tych linii, w których została wykonana komenda (wyrażenie) `p` lub `s`,
* `-e` - przetwarzanie kilku poleceń/skryptów,
* `-f` - wczytanie komend z pliku,
* `-i` - edycja pliku w miejscu, tzn. rezultaty są zapisywane do pliku wejściowego,
* `-b` - zapisanie kopii zapasowej oryginalnego pliku.

Za przykład posłuży zamiana wszystkich wystąpień `foo` na `bar` w pliku `foobars.txt`:

```bash
sed -i 's/foo/bar/' foobars.txt
```

Niestety, jeśli w jakiejś linijce mielibyśmy tekst `foofoofoo`, to zamienione zostałoby tylko pierwsze wystąpienie `foo` - otrzymalibyśmy ciąg `barfoofoo`. Aby temu zapobiec, możemy dodać do naszego podstawienia `g`, co oznacza, że zostaną wykonane zmiany globalnie (globalność oznacza całą bieżącą linijkę). Wtedy skrypt będzie taki: `s/foo/bar/g`, a otrzymana wartość to `foofoofoo`.

Możemy również skasować wszystkie linie (#1), wybraną (#2) lub jakiś zakres (#3):

```bash=
sed -e 'd' foo.txt
sed -e '1d' foo.txt
sed -e '1,3d' foo.txt
```

W razie, gdybyśmy chcieli odczytać zawartość pliku bez komentarzy (niech tutaj będą to linijki rozpoczynające się znakiem `#`), to moglibyśmy wywołać takie polecenie:

```bash
sed -e '/^#/d' foo.txt
```

Możemy zauważyć, że znak karety `^` oznacza początek linii, jako że w `sed` wykorzystujemy wyrażenia regularne, co sprawia, że `sed` jest potężnym narzędziem, co pokaże również kolejny przykład. Teraz chcemy zamienić jakiś tekst w HTML z tagami na taki, który tagów nie zawiera:

```bash
sed -e 's/<[^>]*>//g' foo.html
```

Takie podstawienie oznacza: weź wzór rozpoczynający się od `<`, w którym kolejne znaki (dowolna ilość) są inne niż `>`, co oznacza zapis `[^>]*`, a następnie weź znak `>` zamykający tag. Całość ("wnętrze" tagu, np. w `<a>` jest to `a`) zamień na ciąg pusty, a więc skasuj tag.

Podobnie jak w przypadku *regex*ów, możemy wykorzystywać klasy znakowe takie jak `[:alpha:]`. Ostatnią rzeczą, o której warto wspomnieć w tym "krótkim" omówieniu jest wykorzystanie znaku ampersanda `&`, który umożliwia wstawienie tekstu znalezionego przez wyrażenie regularne. Weźmy takie polecenie, który przed wyrażeniem regularnym `.*` oznaczającym dowolny ciąg znaków, wstawi `tu będzie podstawienie:`:

```bash
sed -e 's/.*/tu będzie podstawienie: &/' foobars.txt
```

### Zadanie 10

:::info
Oto jeden z popularnych przykładów rozważanych w Teorii Przepisywania: $n$ kameleonów siedzi w rzędzie. Każdy z nich jest w danej chwili czerwony $R$, zielony $G$ bądź niebieski $B$. Kameleony chciałyby być niewidoczne, dlatego dwa sąsiadujące kameleony różnych kolorów zmieniają kolor na trzeci. Zmiany może zawsze dokonać pierwsza od lewej para różnokolorowych kameleonów, zatem np. ciąg $RRRRGBGB$ zamieni się na $RRRBBBGB$, a ten na $RRGGBBGB$ itd. Napisz skrypt dla `sed`-a, który zobrazuje krok po kroku zmiany ciągu kameleonów. Uwaga: nie zawsze kameleony osiągną monochromatyczność. Czasem ciąg zmian, a zatem i nasz skrypt, może się zapętlić!
:::

```bash=
#!/bin/bash

# If there is provided any non-empty string, assign its value
# to the variable. Otherwise, allow user to input some data.
if [ "$1" != "" ]; then
    cameleons=$1
else
    read cameleons
fi

N=${#cameleons} # by ${#var} we get variable's length
RED='\033[0;31m'    # some colors to improve the output       
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NOCOLOR='\033[0m'   # default color of text, we need to clear it
echo "Starting position of $N cameleons: $cameleons."

# Run the loop until either of combinations is made: all cameleons
# are red, all are green or all are blue. Otherwise keep going and
# searching for the solution.
while ! [[ $cameleons =~ R{$N}|G{$N}|B{$N} ]]; do
    # Create temporary cameleons with all possible color changes
    sub1=$(echo $cameleons | sed 's:RG\|GR:BB:')
    sub2=$(echo $cameleons | sed 's:RB\|BR:GG:')
    sub3=$(echo $cameleons | sed 's:GB\|BG:RR:')

    # Iterate through all cameleons, if any change has occured (we
    # always look for the first change from the left), assign new
    # colors to these cameleons and break from the for loop.
    for (( i=0; i<$N; i++ )); do
        check=${cameleons:$i:1}
        if [[ "${sub1:$i:1}" != $check ]]
        then
            cameleons=$sub1
            break
        elif [[ "${sub2:$i:1}" != $check ]]
        then
            cameleons=$sub2
            break
        elif [[ "${sub3:$i:1}" != $check ]]
        then
            cameleons=$sub3
            break
        fi
    done

    # Color the output! Occurence of any letter has to be changed
    # so it handles coloring, we use printf for it, as sed itself
    # is not good enough (there were some problems that characters
    # were cut, so printf is a way to overcome them).
    temp=$(sed "s|R|`printf "${RED}R"`|g;\
                s|G|`printf "${GREEN}G"`|g;\
                s|B|`printf "${BLUE}B"`|g" <<< $cameleons)
    echo -e "$temp${NOCOLOR}"
done

```
