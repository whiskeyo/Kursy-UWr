# Kurs administrowania systemem Linux (L9)

## Lista zadań nr 9

### Zadanie 4

Przygotowałem dwie maszyny wirtualne z Debianem 10.9 - nie instalowałem na nich nic oprócz GNOME, dlatego pomiary wykonane na każdej z nich będą miarodajne. Najpierw poleceniem `systemd-analyze` zmierzę czas uruchamiania się systemu, a później na jednej z maszyn zainstaluję `dracut` zamiast `mkinitramfs`. Niestety czasy nie są zbyt korzystne, jako że nie mam w ogóle miejsca na SSD i wszystko robię na HDD. `dracut` to program do generowania `initramfs`, którego celem jest tworzenie jak najmniejszych obrazów.

Na V1 otrzymałem takie czasy:

```
Startup finished in 5.528s (kernel) + 52.413s (userspace) = 57.941s
graphical.target reached after 52.375s in userspace
```

A na V2 takie:

```
Startup finished in 7.143s (kernel) + 50.801s (userspace) = 57.944s
graphical.target reached after 50.776s in userspace
```

Są one dość podobne, dlatego na jednej z maszyn (niech to będzie V1) skasuję `mkinitramfs` i wtedy sprawdzimy czy jest jakaś różnica w czasie uruchamiania obu maszyn. Domyślnym zachowaniem po skasowaniu paczki `initramfs-tools-core` (np. poleceniem `sudo apt remove initramfs-tools-core`) jest automatyczne zainstalowanie `dracut`. Po tym, jak usuwanie się zakończy, możemy uruchomić `dracut` jako `root`.

Pojawi się dość sporo informacji o tym, jakie moduły zostaną zainstalowane/wczytane, część z nich może oczywiście zostać pominięta, gdy nie ma ich w systemie. Przy pozostałych otrzymamy takie informacje:

```
dracut: *** Including module: bash ***
dracut: *** Including module: dash ***
dracut: *** Including module: systemd ***
...
```

Jednak nas najbardziej interesują linie pod koniec wyjścia, dokładniej te:

```
dracut: *** Creating image file '/boot/initramfs-4.19.0-16-amd64.img' ***
dracut: *** Creating initramfs image file '/boot/initramfs-4.19.0-16-amd64.img' done ***
```

Znajduje się tu bowiem ścieżka, którą będziemy musieli wykorzystać w GRUBie. Wpisujemy teraz `reboot` i w GRUBie klikamy `E`, aby edytować parametry wykorzystywane przy bootowaniu komputera. Zmieniamy linijkę, w której jest

```
        initrd        /boot/initrd.img-4.19.0.16-amd64
```

na taką, która wykorzystuje nowowygenerowaną ścieżkę. System nam się powinien wtedy poprawnie załadować, możemy jeszcze sprawdzić `journalctl -b | grep dracut`, czy rzeczywiście wykorzystujemy nową konfigurację.

Sprawdźmy czasy uruchamiania poprzez `systemd-analyze`:

```
Startup finished in 701ms (kernel) + 1.919s (initrd) +  23.864s (userspace) = 26.485s
graphical.target reached after 23.832s in userspace
```

Widzimy, że różnica w czasie ładowania systemu jest bardzo duża, na naszą korzyść. Porównajmy więc zawartość obu obrazów. Na V2 (korzystającej z `mkinitramfs`) służy do tego polecenie `lsinitramfs [ścieżka obrazu] | wc -l`, a na V1 (z `dracut`) jest to `lsinitrd [ścieżka obrazu] | wc -l`. Na V2 otrzymałem 1460, a na V1 tylko 695, co prawdopodobnie oznacza, że wykorzystując `dracut` ładowane jest znacznie mniej modułów, co przyspiesza uruchamianie PC.

To sprawia, że dalsze optymalizowanie (prawdopodobnie) nie ma zbyt dużego sensu. Różnica między ładowaniem na HDD rzeczywiście jest dość spora, jednak warto zwrócić uwagę na to, że większość nowych komputerów korzysta z dysków SSD, których prędkości odczytywania są 10-70 razy szybsze, niż w przypadku HDD. Z tego powodu odnoszę wrażenie, że korzystanie z `dracut` nie jest konieczne, a szczególnie przestawianie się na niego po jakimś czasie korzystania z systemu.

### Zadanie 6

Chcemy, żeby nasz system ładował się do RAMu, a później w nim działał. Pełna lista takich systemów znajduje się np. [na Wikipedii](https://en.wikipedia.org/wiki/List_of_Linux_distributions_that_run_from_RAM), a ja w tym celu wykorzystam [Alipne Linux (Extended)](https://alpinelinux.org/downloads/), który został zalecony w zadaniu. 

Pierwszym krokiem jest stworzenie VM bez żadnego dysku twardego, następnie można uruchomić VM bez żadnego systemu, co spowoduje, że pojawi się błąd mówiący o tym, że żadnego dysku nie ma, który prawdopodobnie będzie wyglądać tak:

```
FATAL: Could not read from the boot medium! System halted.
```

Wystarczy "wsadzić" płytkę z systemem i zresetować maszynę. Pojawią się takie informacje:

![](https://i.imgur.com/AW3oX3B.png)

Chciałem sprawdzić co jest w `/media/cdrom/boot/initramfs-lts`, ale skorzystałem z polecenia `more` - na przyszłość nie polecam, tak tego nie podejrzymy.

Wracając do zadania, po "wyjęciu" płytki z napędu system nadal działa, jako że wszystko jest przechowywane na ten moment w RAMie i korzystamy z Alpine w trybie Diskless Mode. Tryb ten charakteryzuje się tym, że jest bardzo szybki, jednak problemem jest to, że pliki znikną po reboocie. 

Wyłączmy system i przejdźmy więc do działania. Stworzyłem dysk twardy, przypisałem go do VM i uruchomiłem znów system "z płytki". Tym razem po uruchomieniu się systemu wpisałem `setup-alpine`, wszystko zostawiam na domyślne (jako że to nie wpływa za bardzo na to, co robimy w zadaniu)