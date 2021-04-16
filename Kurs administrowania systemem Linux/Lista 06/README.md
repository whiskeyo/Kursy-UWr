# Kurs administrowania systemem Linux (L6)

## Lista zadań nr 6

### Zadanie 1

Założenie konta `Jan Testowy <jantest>`:

```
sudo useradd jantest
```

Zapisanie użytkownika do grup CD-ROM, akceleracji grafiki, dźwięku, interfejsu bluetooth:

```
sudo usermod -a -gG cdrom,audio,video,bluetooth jantest
```

Flaga `-a` oznacza dodanie użytkownika do grup, należy jej używać tylko w obecności flagi `-G`, która oznacza listę grup przekazywanych do flagi `-a`.


Korzystając z `visudo` możemy dodać możliwość uruchamiania polecenia `ip` jako użytkownik `root`, więc uruchamiamy `sudo visudo`, a następnie dodajemy do pliku linijkę:

```
jantest ALL=(root) NOPASSWD: /bin/ip
```

Utworzenie grupy `projekt`, a następnie przypisanie do niej siebie i użytkownika `jantest`:

```
sudo groupadd projekt
sudo adduser whiskeyo projekt
sudo adduser jantest projekt
```

A następnie utworzenie pliku `opis.txt` (to robię w `/tmp`, bo wygodniej), przypisanie go do grupy `projekt` i sprawdzenie, czy rzeczywiście tylko oba konta przypisane do grupy `projekt` mają dostęp do tego pliku:

```
touch opis.txt
sudo chown :projekt opis.txt
chmod 770 opis.txt 
```

Rzeczywiście po zalogowaniu na konto `tester` nienależące do grupy `projekt` wyskakuje *Permission denied*, a więc wszystko się zgadza.

Aby sprawdzić do których grup należę ja oraz konto `jantest`, wystarczy wpisać polecenie

```
groups {whiskeyo,jantest}
```

które zwróci listę wszystkich grup, do których należą użytkownicy.

Ponownie chcemy dodać uprawnienia Janowi, dlatego uruchamiamy `sudo visudo`, a następnie dodajemy linijkę:

```
jantest ALL=(whiskeyo) /usr/bin/whoami
```

Po wywołaniu `whoami` na koncie `jantest` zwracane jest `jantest`, a przy próbie wywołania polecenia z `sudo` pokazuje się powiadomienie, że użytkownik `jantest` nie jest w stanie wywołać `/usr/bin/whoami` jako `root`.

Aby umożliwić zmianę imienia wszystkim użytkownikom, należy dodać `f` do `CHFN_RESTRICT` w pliku `/etc/login.defs`, a następnie poleceniem `chfn -f [imie i nazwisko] [user]` je zmienić. 

### Zadanie 2 (częściowo działa)

RCS to dość prosty system kontroli wersji, jego najbardziej podstawowymi poleceniami są `rcs`, `ci` oraz `co`. Aby rozpocząć pracę z RCSem należy w katalogu, który chcemy śledzić, utworzyć katalog `RCS` lub stworzyć dowiązanie symboliczne katalogu w którym pracujemy do katalogu `RCS` (np. poprzez `ln -s /home/whiskeyo/project/src/RCS RCS`). Żeby nie martwić się o to, że jakiś użytkownik nie ma dostępu do plików znajdujących się w dzielonym katalogu, utworzyłem katalog `/shared` z uprawnieniami `777`. Chciałem też zapobiec automatycznemu kasowaniu się plików, dlatego nie skorzystałem z `/tmp`.

Pierwszym krokiem jest dodanie pliku do śledzenia, służy do tego polecenie `ci -u [plik]` (`-u` to odblokowanie pliku), a następnie dodać jakiś opis do *"commita"*. Możemy podejrzeć teraz katalog `RCS`, powinien utworzyć się tam nowy plik - `[plik],v`, którego zawartością jest historia zmian, wraz z informacjami kto i kiedy je wykonał (do podejrzenia przez `cat`).

Drugim krokiem będzie *checkout*, a więc polecenie `co`. Dodając flagę `-l` blokujemy dostęp do pliku innym użytkownikom, a więc po wywołaniu `co -l opis.txt` będziemy w stanie edytować ten plik tylko na koncie, na którym polecenie zostało wywołane, a dla pozostałych użytkowników będzie zablokowany dostęp. Wykonajmy więc jakieś zmiany, np. `vim`em.

Aby dodać nową wersję pliku, używamy `ci -u [plik]` i ponownie wprowadzamy opis. W taki sposób możemy "przejmować" plik jako użytkownik, gdy chcemy go modyfikować, więc spróbujmy wykonać jakieś zmiany jako `jantest`. 

Niestety tutaj napotkałem problem - mimo, że użytkownicy `whiskeyo` oraz `jantest` należą do tej samej grupy `projekt` i plik ma uprawnienia `-rwxrwx---`, to coś złego się dzieje. Po każdym wywołaniu jakiegoś polecenia z RCS sprawdzam uprawnienia pliku i to, do którego użytkownika i grupy należy. 

Najpierw wywołuje `ci -u test.txt`, dodaję plik do śledzenia i widzę pierwszą zmianę - uprawnienia nagle przechodzą na `-r-xr-x---`, a więc do pliku nie można pisać. Jest to dość normalne, jako że `ci` zapisuje ostatnie zmiany. Później wywołuje `co -l test.txt`, aby umożliwić sobie edycje pliku. Uprawnienia przechodzą na `-rwxr-x---`, a więc tak, jak byśmy tego oczekiwali - plik jednocześnie może edytować tylko jedna osoba. Niestety zmienia się również grupa pliku na `whiskeyo`, a więc na grupę mojego użytkownika. Zmieniłem zawartość pliku `test.txt` i ponownie wywołuje `ci -u test.txt`, żeby zmiany zapisać. Plik teraz ma uprawnienia `-r-xr-x---`, jednak grupa nie powróciła do `projekt`, co sprawia że na użytkowniku `jantest` nie jestem w stanie wywołać żadnego polecenia z tych dwóch, które używałem wcześniej, zwyczajnie dostaje *Permission denied*.

Poleceniem `rcs log [plik]` możemy zobaczyć log zmian wybranego pliku.

### Zadanie 3

Sprawdzenie, czy działa serwer *ssh* jest dość proste, wystarczy wywołać `systemctl status ssh`, nie potrzeba nawet mieć uprawnień administratora. Gdyby okazało się, że *ssh* jest wyłączone, wystarczy użyć `sudo systemctl enable ssh` i będziemy mogli na tym działać.

Klucze RSA generuje w katalogu `~/.ssh` poleceniem `ssh-keygen -t rsa -b 4096 -n dojana`, zapisałem je jako `dojana`, `dojana2` i `dojana3`. Klucz kopiuje poleceniem `ssh-copy-id -i dojana jantest@localhost`. Wtedy, gdy chcę wejść przez *ssh* na konto `jantest`, wystarczy że podam hasło do odblokowania klucza prywantego lub skorzystam z polecenia `ssh -i dojana jantest@localhost` i wtedy, po podaniu nazwy pliku (bez `.pub`) zostanę również zalogowany, tym razem bez podawania hasła.

Postanowiłem, że kluczem wykorzystywanym do logowania będzie jedynie `dojana`, a więc to on zostanie wykorzystany do stworzenia pliku `ssh_config`, a dokładniej do `~/.ssh/config`, który znajduje się w katalogu domowym użytkownika `whiskeyo` (a więc użytkownika z poziomu którego chcę się zalogować przez *ssh* do `jantest`). Zawartość tego pliku jest następująca:

```
Host localhost
    HostName        localhost
    User            jantest
    IdentityFile    ~/.ssh/dojana
```

Poleceniem `ssh-add -t 3600 dojana` zapamiętuje klucz prywatny `dojana`, a następnie przenoszę ten klucz do katalogu `~/.ssh/disabled` (aby go nie usuwać). Dalej mogę wchodzić przez *ssh* do `jantest`.

Aby zablokować hasło na koncie `jantest`, korzystam z polecenia `usermod -L jantest` i rzeczywiście dopóki nie minie godzina od dodaniu klucza prywatnego `dojana`, mogę logować się nawet z zablokowanym hasłem. Aby używać poleceń na `jantest` za pomocą `sudo`, odpalamy `sudo visudo` i dopisujemy linijkę

```
jantest ALL=(root:root) NOPASSWD: ALL
```

Wtedy hasła nie musimy podawać wcale, w przypadku linijki bez `NOPASSWD:` należy wpisać hasło, jednak podając dobre hasło, wyskakuje *Authentication failure*.

Hasło do konta odblokowujemy poleceniem `sudo usermod -U jantest`, a następnie blokujemy konto użytkownika poprzez `sudo usermod -e 1970-01-01 jantest`. Próba wejścia na konto poprzez `su jantest` nie działa, podobnie łączenie się przez `ssh`. Jedynym obejściem jakie znalazłem jest `sudo -u jantest /bin/bash`, przez co dostajemy się do powłoki jako `jantest`, mimo że konto jest zablokowane. Odblokowuje konto poleceniem `sudo usermod -e -1 jantest` (mimo, że w manualu jest jasno napisane *an empty `EXPIRE_DATE`* argument will disable the expiration of the account*, to bez `-1` nie jestem w stanie odblokować konta).

Domyślną powłokę zmieniam poleceniem `sudo usermod -s /bin/false jantest` i wtedy nie jestem w stanie poprzez `su jantest` i *ssh* zalogować się do konta, ale polecenia `sudo -u jantest whoami` czy `sudo -u jantest /bin/bash` już działają poprawnie.

Polecenie `nologin` (u mnie w `/usr/sbin/nologin`) wyświetla informacje o tym, że konto jest tymczasowo niedostępne, dodatkową zaletą jest możliwość zmiany wyświetlanej wiadomości (choć nie udało mi się tego doszukać).

Wyłączenie możliwości logowania się na konto `root` przez *ssh* odbywa się poprzez edycję zawartości pliku `/etc/ssh/sshd_config`, a dokładniej dodanie linijki `PermitRootLogin no`. 

### Zadanie 4 (o tym więcej na SK)

Korzystam z maszyn wirtualnych, jako że niestety nigdzie nie mam kabla Ethernet, ale to akurat nie zmienia wiele w rozwiązaniu zadania. W obu maszynach ustawiam po jednym interfejsie sieci wewnętrznej nazwane `local`, jako że z innych nie będziemy korzystać. Na początek warto wypisać wszystkie dostępne interfejsy, służy do tego polecenie `ip link`, nas szczególnie interesować będzie `enp0s3`, jako że `lo` *(loopback)* to interfejs wirtualny, przez który nie możemy połączyć dwóch maszyn. 

Jeśli chcemy połączyć dwie maszyny, należy przypisać każdej z nich jakiś adres IP oraz maskę, wybrałem w tym celu adresy `10.0.1.1/16` oraz `10.0.1.2/16`, a następnie uruchomić interfejsy obu maszyn. Wpisuję kolejno polecenia:

```=
VM1# ip addr add 10.0.1.1/16 dev enp0s3
VM1# ip link set enp0s3 up

VM2# ip addr add 10.0.1.2/16 dev enp0s3
VM2# ip link set enp0s3 up
```

Poleceniem `ip addr add/del [address/mask] dev [device]` możemy dodać/usunąć przypisany adres IP do/z wybranego interfejsu, wtedy pingując dowolny z istniejących adresów IP przypisanych do tego interfejsu uzyskamy odpowiedź. Poleceniem `ip addr flush dev [device]` możemy usunąć całą konfigurację wybranego interfejsu.

Oczywiście `ip addr show dev [device]` pokazuje właściwości wybranego interfejsu.

### Zadanie 5

Do pliku `/etc/network/interfaces` na obu maszynach dopisuje:

```
iface enp0s3 inet static
    address 10.0.1.1/16 LUB 10.0.1.2/16
```

To wystarcza, aby za pomocą polecenia `ifup enp0s3` uruchomić interfejs sieciowy, który umożliwi nam wzajemne pingowanie urządzeń, a `ifdown enp0s3` wyłączy ten interfejs. 

Ponadto możemy w w/w pliku dodać `auto` do pierwszej linijki, wtedy mając więcej intefejsów będziemy w stanie za pomocą `ifup -a` włączać wszystkie interfejsy używające tagu `auto`. Poleceniem `ifdown -a` możemy wyłączyć wszystkie aktywne interfejsy sieciowe. 

Polecenia `if{up,down}` są tak naprawdę tym samym programem, który jest wywoływany poprzez różne nazwy. Interfejsy nie są konfigurowane bezpośrednio, na podstawie pliku `/etc/network/interfaces` są wywoływane są różne programy (np. `ip`) z odpowiednimi opcjami, które wykonują całą robotę.# Kurs administrowania systemem Linux (L6)

## Lista zadań nr 6

### Zadanie 1

Założenie konta `Jan Testowy <jantest>`:

```
sudo useradd jantest
```

Zapisanie użytkownika do grup CD-ROM, akceleracji grafiki, dźwięku, interfejsu bluetooth:

```
sudo usermod -a -gG cdrom,audio,video,bluetooth jantest
```

Flaga `-a` oznacza dodanie użytkownika do grup, należy jej używać tylko w obecności flagi `-G`, która oznacza listę grup przekazywanych do flagi `-a`.


Korzystając z `visudo` możemy dodać możliwość uruchamiania polecenia `ip` jako użytkownik `root`, więc uruchamiamy `sudo visudo`, a następnie dodajemy do pliku linijkę:

```
jantest ALL=(root) NOPASSWD: /bin/ip
```

Utworzenie grupy `projekt`, a następnie przypisanie do niej siebie i użytkownika `jantest`:

```
sudo groupadd projekt
sudo adduser whiskeyo projekt
sudo adduser jantest projekt
```

A następnie utworzenie pliku `opis.txt` (to robię w `/tmp`, bo wygodniej), przypisanie go do grupy `projekt` i sprawdzenie, czy rzeczywiście tylko oba konta przypisane do grupy `projekt` mają dostęp do tego pliku:

```
touch opis.txt
sudo chown :projekt opis.txt
chmod 770 opis.txt 
```

Rzeczywiście po zalogowaniu na konto `tester` nienależące do grupy `projekt` wyskakuje *Permission denied*, a więc wszystko się zgadza.

Aby sprawdzić do których grup należę ja oraz konto `jantest`, wystarczy wpisać polecenie

```
groups {whiskeyo,jantest}
```

które zwróci listę wszystkich grup, do których należą użytkownicy.

Ponownie chcemy dodać uprawnienia Janowi, dlatego uruchamiamy `sudo visudo`, a następnie dodajemy linijkę:

```
jantest ALL=(whiskeyo) /usr/bin/whoami
```

Po wywołaniu `whoami` na koncie `jantest` zwracane jest `jantest`, a przy próbie wywołania polecenia z `sudo` pokazuje się powiadomienie, że użytkownik `jantest` nie jest w stanie wywołać `/usr/bin/whoami` jako `root`.

Aby umożliwić zmianę imienia wszystkim użytkownikom, należy dodać `f` do `CHFN_RESTRICT` w pliku `/etc/login.defs`, a następnie poleceniem `chfn -f [imie i nazwisko] [user]` je zmienić. 

### Zadanie 2 (częściowo działa)

RCS to dość prosty system kontroli wersji, jego najbardziej podstawowymi poleceniami są `rcs`, `ci` oraz `co`. Aby rozpocząć pracę z RCSem należy w katalogu, który chcemy śledzić, utworzyć katalog `RCS` lub stworzyć dowiązanie symboliczne katalogu w którym pracujemy do katalogu `RCS` (np. poprzez `ln -s /home/whiskeyo/project/src/RCS RCS`). Żeby nie martwić się o to, że jakiś użytkownik nie ma dostępu do plików znajdujących się w dzielonym katalogu, utworzyłem katalog `/shared` z uprawnieniami `777`. Chciałem też zapobiec automatycznemu kasowaniu się plików, dlatego nie skorzystałem z `/tmp`.

Pierwszym krokiem jest dodanie pliku do śledzenia, służy do tego polecenie `ci -u [plik]` (`-u` to odblokowanie pliku), a następnie dodać jakiś opis do *"commita"*. Możemy podejrzeć teraz katalog `RCS`, powinien utworzyć się tam nowy plik - `[plik],v`, którego zawartością jest historia zmian, wraz z informacjami kto i kiedy je wykonał (do podejrzenia przez `cat`).

Drugim krokiem będzie *checkout*, a więc polecenie `co`. Dodając flagę `-l` blokujemy dostęp do pliku innym użytkownikom, a więc po wywołaniu `co -l opis.txt` będziemy w stanie edytować ten plik tylko na koncie, na którym polecenie zostało wywołane, a dla pozostałych użytkowników będzie zablokowany dostęp. Wykonajmy więc jakieś zmiany, np. `vim`em.

Aby dodać nową wersję pliku, używamy `ci -u [plik]` i ponownie wprowadzamy opis. W taki sposób możemy "przejmować" plik jako użytkownik, gdy chcemy go modyfikować, więc spróbujmy wykonać jakieś zmiany jako `jantest`. 

Niestety tutaj napotkałem problem - mimo, że użytkownicy `whiskeyo` oraz `jantest` należą do tej samej grupy `projekt` i plik ma uprawnienia `-rwxrwx---`, to coś złego się dzieje. Po każdym wywołaniu jakiegoś polecenia z RCS sprawdzam uprawnienia pliku i to, do którego użytkownika i grupy należy. 

Najpierw wywołuje `ci -u test.txt`, dodaję plik do śledzenia i widzę pierwszą zmianę - uprawnienia nagle przechodzą na `-r-xr-x---`, a więc do pliku nie można pisać. Jest to dość normalne, jako że `ci` zapisuje ostatnie zmiany. Później wywołuje `co -l test.txt`, aby umożliwić sobie edycje pliku. Uprawnienia przechodzą na `-rwxr-x---`, a więc tak, jak byśmy tego oczekiwali - plik jednocześnie może edytować tylko jedna osoba. Niestety zmienia się również grupa pliku na `whiskeyo`, a więc na grupę mojego użytkownika. Zmieniłem zawartość pliku `test.txt` i ponownie wywołuje `ci -u test.txt`, żeby zmiany zapisać. Plik teraz ma uprawnienia `-r-xr-x---`, jednak grupa nie powróciła do `projekt`, co sprawia że na użytkowniku `jantest` nie jestem w stanie wywołać żadnego polecenia z tych dwóch, które używałem wcześniej, zwyczajnie dostaje *Permission denied*.

Poleceniem `rcs log [plik]` możemy zobaczyć log zmian wybranego pliku.

### Zadanie 3

Sprawdzenie, czy działa serwer *ssh* jest dość proste, wystarczy wywołać `systemctl status ssh`, nie potrzeba nawet mieć uprawnień administratora. Gdyby okazało się, że *ssh* jest wyłączone, wystarczy użyć `sudo systemctl enable ssh` i będziemy mogli na tym działać.

Klucze RSA generuje w katalogu `~/.ssh` poleceniem `ssh-keygen -t rsa -b 4096 -n dojana`, zapisałem je jako `dojana`, `dojana2` i `dojana3`. Klucz kopiuje poleceniem `ssh-copy-id -i dojana jantest@localhost`. Wtedy, gdy chcę wejść przez *ssh* na konto `jantest`, wystarczy że podam hasło do odblokowania klucza prywantego lub skorzystam z polecenia `ssh -i dojana jantest@localhost` i wtedy, po podaniu nazwy pliku (bez `.pub`) zostanę również zalogowany, tym razem bez podawania hasła.

Postanowiłem, że kluczem wykorzystywanym do logowania będzie jedynie `dojana`, a więc to on zostanie wykorzystany do stworzenia pliku `ssh_config`, a dokładniej do `~/.ssh/config`, który znajduje się w katalogu domowym użytkownika `whiskeyo` (a więc użytkownika z poziomu którego chcę się zalogować przez *ssh* do `jantest`). Zawartość tego pliku jest następująca:

```
Host localhost
    HostName        localhost
    User            jantest
    IdentityFile    ~/.ssh/dojana
```

Poleceniem `ssh-add -t 3600 dojana` zapamiętuje klucz prywatny `dojana`, a następnie przenoszę ten klucz do katalogu `~/.ssh/disabled` (aby go nie usuwać). Dalej mogę wchodzić przez *ssh* do `jantest`.

Aby zablokować hasło na koncie `jantest`, korzystam z polecenia `usermod -L jantest` i rzeczywiście dopóki nie minie godzina od dodaniu klucza prywatnego `dojana`, mogę logować się nawet z zablokowanym hasłem. Aby używać poleceń na `jantest` za pomocą `sudo`, odpalamy `sudo visudo` i dopisujemy linijkę

```
jantest ALL=(root:root) NOPASSWD: ALL
```

Wtedy hasła nie musimy podawać wcale, w przypadku linijki bez `NOPASSWD:` należy wpisać hasło, jednak podając dobre hasło, wyskakuje *Authentication failure*.

Hasło do konta odblokowujemy poleceniem `sudo usermod -U jantest`, a następnie blokujemy konto użytkownika poprzez `sudo usermod -e 1970-01-01 jantest`. Próba wejścia na konto poprzez `su jantest` nie działa, podobnie łączenie się przez `ssh`. Jedynym obejściem jakie znalazłem jest `sudo -u jantest /bin/bash`, przez co dostajemy się do powłoki jako `jantest`, mimo że konto jest zablokowane. Odblokowuje konto poleceniem `sudo usermod -e -1 jantest` (mimo, że w manualu jest jasno napisane *an empty `EXPIRE_DATE`* argument will disable the expiration of the account*, to bez `-1` nie jestem w stanie odblokować konta).

Domyślną powłokę zmieniam poleceniem `sudo usermod -s /bin/false jantest` i wtedy nie jestem w stanie poprzez `su jantest` i *ssh* zalogować się do konta, ale polecenia `sudo -u jantest whoami` czy `sudo -u jantest /bin/bash` już działają poprawnie.

Polecenie `nologin` (u mnie w `/usr/sbin/nologin`) wyświetla informacje o tym, że konto jest tymczasowo niedostępne, dodatkową zaletą jest możliwość zmiany wyświetlanej wiadomości (choć nie udało mi się tego doszukać).

Wyłączenie możliwości logowania się na konto `root` przez *ssh* odbywa się poprzez edycję zawartości pliku `/etc/ssh/sshd_config`, a dokładniej dodanie linijki `PermitRootLogin no`. 

### Zadanie 4 (o tym więcej na SK)

Korzystam z maszyn wirtualnych, jako że niestety nigdzie nie mam kabla Ethernet, ale to akurat nie zmienia wiele w rozwiązaniu zadania. W obu maszynach ustawiam po jednym interfejsie sieci wewnętrznej nazwane `local`, jako że z innych nie będziemy korzystać. Na początek warto wypisać wszystkie dostępne interfejsy, służy do tego polecenie `ip link`, nas szczególnie interesować będzie `enp0s3`, jako że `lo` *(loopback)* to interfejs wirtualny, przez który nie możemy połączyć dwóch maszyn. 

Jeśli chcemy połączyć dwie maszyny, należy przypisać każdej z nich jakiś adres IP oraz maskę, wybrałem w tym celu adresy `10.0.1.1/16` oraz `10.0.1.2/16`, a następnie uruchomić interfejsy obu maszyn. Wpisuję kolejno polecenia:

```=
VM1# ip addr add 10.0.1.1/16 dev enp0s3
VM1# ip link set enp0s3 up

VM2# ip addr add 10.0.1.2/16 dev enp0s3
VM2# ip link set enp0s3 up
```

Poleceniem `ip addr add/del [address/mask] dev [device]` możemy dodać/usunąć przypisany adres IP do/z wybranego interfejsu, wtedy pingując dowolny z istniejących adresów IP przypisanych do tego interfejsu uzyskamy odpowiedź. Poleceniem `ip addr flush dev [device]` możemy usunąć całą konfigurację wybranego interfejsu.

Oczywiście `ip addr show dev [device]` pokazuje właściwości wybranego interfejsu.

### Zadanie 5

Do pliku `/etc/network/interfaces` na obu maszynach dopisuje:

```
iface enp0s3 inet static
    address 10.0.1.1/16 LUB 10.0.1.2/16
```

To wystarcza, aby za pomocą polecenia `ifup enp0s3` uruchomić interfejs sieciowy, który umożliwi nam wzajemne pingowanie urządzeń, a `ifdown enp0s3` wyłączy ten interfejs. 

Ponadto możemy w w/w pliku dodać `auto` do pierwszej linijki, wtedy mając więcej intefejsów będziemy w stanie za pomocą `ifup -a` włączać wszystkie interfejsy używające tagu `auto`. Poleceniem `ifdown -a` możemy wyłączyć wszystkie aktywne interfejsy sieciowe. 

Polecenia `if{up,down}` są tak naprawdę tym samym programem, który jest wywoływany poprzez różne nazwy. Interfejsy nie są konfigurowane bezpośrednio, na podstawie pliku `/etc/network/interfaces` są wywoływane są różne programy (np. `ip`) z odpowiednimi opcjami, które wykonują całą robotę.