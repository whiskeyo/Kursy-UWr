# Kurs administrowania systemem Linux (L5)

## Lista zadań nr 5

### Zadanie 3

Bity `rwx` dla katalogów pełnią następującą rolę:
* `r` *(read)* pozwala na pozyskanie listy wszystkich plików w katalogu,
* `w` *(write)* pozwala na tworzenie, zmienianie nazw oraz kasowanie plików w katalogu, jak i modyfikowanie atrybutów katalogu,
* `x` *(execute)* pozwala na przechodzenie po katalogu, gdy jest on w ścieżce, do której chcemy się dostać, tzn. można dzięki temu dostać się do wszystkich plików i katalogów w środku. Czasami ten bit nazywany jest *search bit* ze względu na swoje działanie.

Oczywiście występują one w uprawnieniach trzykrotnie: pierwsze trzy bity należą do uprawnień użytkownika, drugie do uprawnień grupy, a trzecie dla "innych". Te 9 bitów zajmują bity od drugiego do dziesiątego. Pierwszy jest przeznaczony na typ pliku: 
* `-` - plik zwykły
* `d` - katalog
* `c` - urządzenie znakowe
* `b` - urządzenie blokowe
* `s` - gniazdo
* `p` - nazwany *pipe*
* `l` - dowiązanie symboliczne

#### Trochę więcej o bitach `rwx`

Jeśli stworzymy katalog `read` a w nim kilka plików, np. `a`, `b`, `c`, a następnie zmienimy uprawnienia katalogu na `000`, a potem na `+r`, to będziemy w stanie wylistować zawartość katalogu, jednak nie będziemy mogli dostać się do tych plików (np. poleceniem `less read/a`). Wylistowanie plików w katalogu sprawi, że przy każdym pliku otrzymamy następujące błędy:

```
$ ls read                                                                 
ls: cannot access 'read/a': Permission denied 
ls: cannot access 'read/c': Permission denied 
ls: cannot access 'read/b': Permission denied 
a  b  c
```

Oczywiście jest to wina braku dostępu do przechodzenia po katalogu, a więc bitu *execute*. Dodajmy więc bit `x` do uprawnień i sprawdźmy jak będzie działać `ls` po zmianach dla `read`. Dostajemy na wyjściu już tylko pliki w katalogu, bez żadnych błędów, a poleceniem `less` możemy odczytać ich zawartość. Oczywiście nadal nie możemy tworzyć plików, jako że brakuje nam bitu *write*, po zmianie uprawnień będziemy w stanie tworzyć już pliki w katalogu `read`.

#### Bit `sticky`

Bit `sticky` w katalogu pozwala na usuwanie/zmienianie uprawnień/pliku tylko właścicielowi owego katalogu. Bit ten jest stosowany często w `/tmp`, do którego mogą mieć dostęp wszyscy użytkownicy systemu, przez co użytkownicy nie mogą usuwać plików nienależących do nich (w uprawnieniach oznaczony jest przez `t` na ostatnim miejscu):

```
drwxrwxrwt  36 root root 20480 Mar 31 01:05 tmp
```

Bit ten ustawiamy poleceniem:

```
chmod +t [dir]
```

Stwórzmy jako użytkownik A katalog w `/tmp`, a w nim kilka plików. Zmieńmy użytkownika na B, wejdźmy do utworzonego katalogu i spróbujmy edytować któryś z plików z tego katalogu. W `vim`ie otrzymamy informacje, że plik jest tylko do odczytu.

#### Bit `setgid`

Dzięki bitowi `setgid` w katalogu, wszystkie nowo tworzone pliki i katalogi stają się własnością grupy będącej właścicielem katalogu, zwykle atrybut ten jest dziedziczony przez nowo tworzone podkatalogi. Oznacza to, że jeśli katalog `foo` ma ustawiony bit `setgid`, to utworzone pliki w tym katalogu będą miały tą samą grupę co katalog, a nie osoba która utworzyła te pliki. Aby zlokalizować bit `setgid`, należy szukać literki `s` zamiast `x` w uprawnieniach grupy. Za ustawienie bitu `setgid` odpowiada polecenie

```
chmod g+s [dir]
```

a za usunięcie

```
chmod g-s [dir]
```

Jak to zrobić? W `tmp` stworzyłem katalogu jako użytkownik `tester`, a w nim kilka plików. Poleceniem `chown whiskeyo /tmp/[dir]` zmieniłem właściciela katalogu na siebie, po czym dodałem tam nowy plik. Listując wszystkie pliki poleceniem `ls -l` otrzymałem taki output:

```
-rw-r--r-- 1 tester   tester 23 Mar 31 01:52 a
-rw-r--r-- 1 tester   tester  0 Mar 31 01:51 b
-rw-r--r-- 1 tester   tester  0 Mar 31 01:51 c
-rw-r--r-- 1 tester   tester  0 Mar 31 02:03 d
-rw-r--r-- 1 tester   tester  0 Mar 31 02:03 e
-rw-r--r-- 1 tester   tester  0 Mar 31 02:03 f
-rw-r--r-- 1 whiskeyo tester  0 Mar 31 02:05 whiskeyoA
                      ^----^
              ta kolumna oznacza grupę będącą
                właścicielem tego pliku
```

Bez bitu `setgid` po wykonaniu takich kroków mielibyśmy taki output:

```
-rw-r--r-- 1 tester   tester   0 Mar 31 02:09 a
-rw-r--r-- 1 tester   tester   0 Mar 31 02:09 b
-rw-r--r-- 1 whiskeyo whiskeyo 0 Mar 31 02:10 c
-rw-r--r-- 1 whiskeyo whiskeyo 0 Mar 31 02:10 d
```

Oznacza to, że grupa będąca właścicielem już się nie przenosi tak jak wcześniej.