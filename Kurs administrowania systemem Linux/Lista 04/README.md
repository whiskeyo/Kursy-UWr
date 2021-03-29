# Kurs administrowania systemem Linux (L4)

## Lista zadań nr 4

### Zadanie 1

Program `ls` służy do wylistowania zawartości katalogów. Jego opcje oznaczają: 
* `-l` - wypisanie wszystkich danych w kolumnach (*long listing format*),
* `-a` - wypisanie plików rozpoczynających się od `.` (np. `.gitignore`, `.bashrc`)
* `-A` - wypisanie prawie wszystkich plików, oprócz `.` (bieżący katalog) oraz `..` (katalog niższy), 
* `-F` - dopisanie znaków takich jak `*`, `/`, `=`, `>`, `@`, `|` do odpowiednich wpisów (np. `/` do katalogów), 
* `-b` - wypisywanie stringów ze znakiem ucieczki *(ang. escape character)*, czyli z `\`. Polega to na tym, że pliki i katalogi wieloczłonowe są zapisywane w stylu `folder\ testowy`, a nie `'folder testowy'` (*backslash* przed spacją, brak apostrofów "opakowujących" string),
* `-h` - wypisywanie rozmiarów w łatwy do odczytania sposób, np. `254M`, `3G`,
* `-v` - sortowanie po wersjach,
* `--block-size=[SIZE]` - skalowanie rozmiaru plików poprzez `SIZE`,
* `-B` - ignorowanie backupów, czyli wpisów rozpoczynających się od `~`,
* `-d` - wypisanie jedynie katalogów, bez ich zawartości,
* `-h` - wypisanie rozmiarów sposobem *human readable*, a więc w kilobajtach, megabajtach, itp.,
* `-R` - wypisanie zawartości podkatalogów rekurencyjnie,
* `-s` - wypisanie rozmiaru plików przy ich nazwie,
* `-c` (użyte z `-lt`) - sortowanie po czasie ostatniej modyfikacji,
* `-S` - sortowanie po rozmiarze pliku malejąco,
* `-t` - sortowanie po czasie modyfikacji od najnowszego,
* `-u` (użyte z `-lt`) - sortowanie po czasie dostępu od najnowszego,
* `-U` - brak sortowania, wypisanie w kolejności w jakiej katalogi są zapisane w systemie plików,
* `-X` - sortowanie po rozszerzeniu pliku.

Program `stat` służy do wyświetlenia informacji o pliku lub systemie plików. Domyślnie wyświetlanymi danymi są nazwa, rozmiar, typ, numer *inode*'a, liczba dowiązań, uprawnienia, jak i czasy dostępu, modyfikacji (zawartości pliku) oraz zmiany (np. nazwy pliku). Opcje przekazywane do `stat` służą głównie do zmiany formatowania wyświetlanych informacji. Przykładowy wydruk:

```
  File: README.md
  Size: 1250            Blocks: 8          IO Block: 4096   regular file
Device: 818h/2072d      Inode: 1968962     Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1000/whiskeyo)   Gid: ( 1000/whiskeyo)
Access: 2021-03-22 16:21:34.811758186 +0100
Modify: 2021-03-22 15:39:45.595411478 +0100
Change: 2021-03-22 15:39:45.595411478 +0100
 Birth: -
```

Program `realpath` służy do wyświetlania rzeczywistej (absolutnej) ścieżki pliku, a więc dla pliku umieszczonego w katalogu `~/Desktop/abc.def` uzyskamy ścieżkę `/home/whiskeyo/Desktop/abc.def`, a więc za `~` zostanie podstawiona ścieżka ze zmiennej środowiskowej `$HOME`. Możemy również wykorzystać opcje dostarczane przez program, takie jak:

* `-m` - ignorowanie tego, że ścieżka nie istnieje (bo np. nie ma katalogów),
* `-e` - wymóg istnienia wszystkich składowych ścieżki (czym się różni to od wywołania `realpath` bez tej opcji, skoro błąd o braku ścieżki jest i tak zwracany?),
* `-P` - rozpatrywanie dowiązań symbolicznych w razie, gdyby takie wystąpiły,
* `-q` - ignorowanie większości błędów,
* `--relative-to=DIR` - wypisanie ścieżki w stosunku do `DIR`,
* `-s` - nie rozszerzaj dowiązań symbolicznych.

Program `readlink` niewiele różni się od `realpath` (a szczególnie użycie `readlink -f`, które działa tak samo jak `realpath`). Użyta opcja `-f` oznacza, że w ścieżce muszą istnieć wszystkie komponenty poza ostatnim. Opcje `-e`, `-m`, `-q` (lub `-s`) działają tak, jak w przypadku `realpath`. Po stworzeniu pliku `plik` oraz dowiązania symbolicznego poleceniem `ln -s plik symplik`, wywołanie `readlink symplik` zwróci `plik`.

### Zadanie 2

Korzystam z Debiana 10.8, dlatego w drugiej tabelce są wpisy dotyczące Debiana.

|   wpis `man`    | plik              | znaczenie                                                                                                                                                                            |
|:---------------:| ----------------- |:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
|  `hostname(5)`  | `/etc/hostname`   | Nazwa urządzenia, w obrębie sieci pozwala na łączenie się z komputerem przez *ssh*, np. `ssh whiskeyo@dm`.                                                                           |
| `machine-id(5)` | `/etc/machine-id` | Unikalne ID systemu generowane w trakcie instalacji systemu lub podczas pierwszego boot'a.                                                                                           |
| `os-release(5)` | `/etc/os-release` | Informacje o aktualnie zainstalowanej wersji systemu operacyjnego. Zawiera nazwę, wersję (ID oraz tekstową) oraz kilka linków (strona główna Debiana, support i reportowanie bugów). |

| polecenie         | znaczenie                                                                                                                                                                                    |
| ----------------- |:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `hostname(1)`     | Wyświetlenie nazwy hosta, możliwość jej zmiany. Można również pokazać adresy IP (opcje `-i` oraz `-I`).                                                                                      |
| `hostnamectl(1)`  | Podobne do `hostname(1)`, jednak bardziej rozbudowane. Domyślnie wyświetla informacje takie jak nazwa hosta, ID maszyny oraz ID boot'a, wersję systemu operacyjnego i jądra, i architekturę. |
| `uuid(1)`         | *w moim manualu nie ma wpisu o `uuid(1)`*                                                                                                                                                                                             |
| `uuidgen(1)`      |  *w moim manualu nie ma wpisu o `uuidgen(1)`*                                                                                                                                                                                             |
| `dbus-uuidgen(1)` | Program do tworzenia/generowania UUID.                                                                                                                                                       |
| `uname(1)`        | Wypisanie informacji o systemie, takich jak nazwa jądra, jego wydanie i wersja, typ procesora (architektura), nazwa systemu operacyjnego.                                                    |
| `lsb_release(1)`  | Wyświetlanie informacji podobnych do `os-release(5)`.                                                                                                                                        |

Aby zidentyfikować dystrybucję Linuxa jak i wersję konkretnej instalacji, na której pracujemy, możemy skorzystać z jednego z poleceń: `uname -a`, `cat /proc/version`, `hostnamectl`, `lsb_release -a`. Poniżej wydruk kolejno wywoływanych poleceń u mnie:

```
whiskeyo@dm Desktop → uname -a
    Linux dm 4.19.0-14-amd64 #1 SMP Debian 4.19.171-2 (2021-01-30) x86_64 GNU/Linux
whiskeyo@dm Desktop → cat /proc/version 
    Linux version 4.19.0-14-amd64 (debian-kernel@lists.debian.org) (gcc version 8.3.0 (Debian 8.3.0-6)) #1 SMP Debian 4.19.171-2 (2021-01-30)
whiskeyo@dm Desktop → hostnamectl
       Static hostname: dm
             Icon name: computer-laptop
               Chassis: laptop
            Machine ID: 570e4055e22a4a65bb0d39fa9109f418
               Boot ID: c92744e68cfd408096e2c7fb2b3e596d
      Operating System: Debian GNU/Linux 10 (buster)
                Kernel: Linux 4.19.0-14-amd64
          Architecture: x86-64
whiskeyo@dm Desktop → lsb_release -a
    No LSB modules are available.
    Distributor ID: Debian
    Description:    Debian GNU/Linux 10 (buster)
    Release:        10
    Codename:       buster
```

Sam często korzystam z narzędzia `neofetch`, które wyświelta więcej informacji o systemie, włącznie ze specyfikacją komputera (CPU, GPU, pamięć RAM), używaną powłoką, zainstalowanymi paczkami itp.

```
       _,met$$$$$gg.          whiskeyo@dm 
    ,g$$$$$$$$$$$$$$$P.       ----------- 
  ,g$$P"     """Y$$.".        OS: Debian GNU/Linux 10 (buster) x86_64 
 ,$$P'              `$$$.     Host: N85_N87,HJ,HJ1,HK1 
',$$P       ,ggs.     `$$b:   Kernel: 4.19.0-14-amd64 
`d$$'     ,$P"'   .    $$$    Uptime: 13 days, 1 hour, 35 mins 
 $$P      d$'     ,    $$P    Packages: 2827 (dpkg), 12 (snap) 
 $$:      $$.   -    ,d$$'    Shell: bash 5.0.3 
 $$;      Y$b._   _,d$P'      Resolution: 1920x1080 
 Y$$.    `.`"Y$$$$P"'         DE: KDE 
 `$$b      "-.__              WM: KWin 
  `Y$$                        Theme: Breeze Dark [KDE], Breeze [GTK3] 
   `Y$$.                      Icons: breeze-dark [KDE], breeze [GTK3] 
     `$$b.                    Terminal: konsole 
       `Y$$b.                 Terminal Font: Hack 12 
          `"Y$b._             CPU: Intel i5-7300HQ (4) @ 3.500GHz 
              `"""            GPU: Intel HD Graphics 630 
                              GPU: NVIDIA GeForce GTX 1050 Ti Mobile 
                              Memory: 4951MiB / 15931MiB 
```

### Zadanie 3

Program `lsof(8)` służy do wypisania listy otwartych plików (również przez procesy!). Otwartym plikiem może być normalny plik, katalog, plik urządzenia blokowego, plik urządzenia znakowego, biblioteka, strumień lub plik sieciowy (np. gniazdo). Program ten może zostać wykorzystany np. w trakcie odmontowywania różnych urządzeń poleceniem `umount`, szczególnie gdy ta operacja nie może zostać poprawnie przeprowadzona. Wtedy dzięki `lsof` można łatwo znaleźć otwarte na tym urządzeniu pliki jak i zidentyfikować procesy, które te pliki wykorzystują. 

Poleceniem `fuser(1)` możemy zidentyfikować procesy używające plików bądź gniazd. Działa on bardzo podobnie do `lsof`, jednak umożliwia jedną rzecz, której nie posiada `lsof` - chodzi o zabijanie procesów flagą `-k`. Aby to samo wykonać za pomocą `lsof`, należałoby przekazać listę *PID*ów do polecenia `kill`, chociażby w taki sposób:

```
kill -TERM $(lsof -t [path])
```

Wracając do `lsof`, możemy w nim przeglądać również sockety za pomocą flagi `-U`, za pomocą wyżej użytego `-t` otrzymamy tylko listę ID procesów. Z flagą `-u` możemy ograniczyć wyszukiwanie do wybranego użytkownika (np. `lsof -u whiskeyo`). Flaga `+d` służy do przejrzenia plików otwartych w wybranym katalogu, a `+D` wykona tę operację rekurencyjnie na swoich podkatalogach.

```
# Sprawdzenie plików otwartych przez podany proces:
lsof -p [pid]

# Sprawdzenie który proces otworzył wybrany plik:
lsof [plik/katalog]  <--- lista wszystkich procesów wraz z dodatkowymi informacjami
lsof -t [plik/katalog] <--- lista PIDów

# Wypisanie plików otworzonych przez wybranego użytkownika:
lsof -u [user]
```

### Zadanie 4

Program `strace` pozwala śledzić wywołania systemowe oraz sygnały wysyłane przez programy/aplikacje (a więc zbadać interakcję z jądrem systemu operacyjnego). Korzystając z różnych opcji można sprecyzować to, co chcemy uzyskać wywołując `strace`. Co ważne, opcje należy przekazać jako argument przed wywołaniem wybranego programu, a więc

```
strace [OPTIONS] [PROG]
```

Gdybyśmy tą kolejność zmienili i zamiast wywołać `strace -t /bin/true` wywołalibyśmy `strace /bin/true -t`, zobaczymy że `-t` zostanie przekazane w liście argumentów `execve` do `/bin/true`:

```
execve("/bin/false", ["/bin/false", "-t"], 0x7ffc1b5294e8 /* 62 vars */) = 0
```

Opcją `-e trace=` można śledzić wybrane *syscall*e poprzez ich nazwę, np. `mmap`, `clone`, `execve` i inne. Jeśli chcemy śledzić kilka syscalli, możemy wypisać je po przecinku (bez spacji). Za pomocą opcji `-c`, `-C`, `-T` możemy zliczyć czas wykonania poszczególnych syscalli, jak i ich liczbę. Opcje `-t` oraz `-r` służą do wypisania czasu, pierwsza z nich służy do wypisania czasu wykonania (godzina, np. `19:45:32`), a druga do wypisania czasu absolutnego (a więc zaczynającego się od `0`, gdy wywoływany jest `strace`). Opcja `-t` ma kilka wariantów: `-t` wypisze czas w formacie `HH:MM:SS`, `-tt` dopisze do tego czas w mikrosekundach, a `-ttt` wypisze czas od początku czasu uniksowego. Opcja `-k` wypisze również operacje ze stosu.

### Zadanie 5

Skrypt główny w pliku `mylogs`:

```bash=
#!/bin/bash
pipe="/tmp/mylog.fifo"
mkfifo $pipe

while :; do
    while read -r log; do
        if [ "$log" ]; then
            date=$(date '+%Y-%m-%d %H:%M:%S')
            echo "[$date]: $log"
        fi
    done < "$pipe"
done
```

Uruchamiamy `mylogs`, a skrypty `freemem` i `spam` odpalamy w innych sesjach terminala. Wtedy wszystkie dane są przekierowywane do `mylogs`, który wyświetla datę i godzinę uzyskania logów, oraz samą treść logów. 

### Zadanie 6

`netstat(8)` służy do wypisania informacji o połączeniach sieciowych, tablicach routingu, dostępnych interfejsach, itp. Użycie programu bez parametrów powoduje wyświetlenie aktywnych połączeń protokołu TCP. 

Użycie programu z opcją `-r` spowoduje wypisanie tablicy routingu:

```
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
default         192.168.0.1     0.0.0.0         UG        0 0          0 wlp4s0
link-local      0.0.0.0         255.255.0.0     U         0 0          0 wlp4s0
192.168.0.0     0.0.0.0         255.255.255.0   U         0 0          0 wlp4s0
```

Opcja `-i` sprawi, że wypisane zostaną wszystkie dostępne interfejsy sieciowe:

```
Kernel Interface table
Iface      MTU    RX-OK RX-ERR RX-DRP RX-OVR    TX-OK TX-ERR TX-DRP TX-OVR Flg
enp3s0f1  1500        0      0      0 0             0      0      0      0 BMU
lo       65536    63071      0      0 0         63071      0      0      0 LRU
wlp4s0    1500  4092886      0      0 0       1381253      0      0      0 BMRU
```

Aby dowiedzieć się o tym, który proces utworzył gniazdo, można najpierw skorzystać z polecenia `lsof -Ua` (polecam przekierować wyjście do `less`, dzięki czemu będziemy przeglądać całą listę od początku, a nie od końca). Dzięki temu uzyskamy taką listę (oczywiście to tylko fragment, rzeczywiście jest dużo dłuższa):

```
COMMAND     PID     USER   FD   TYPE             DEVICE SIZE/OFF    NODE NAME
systemd    1424 whiskeyo    1u  unix 0x0000000086842cbe      0t0   34255 type=STREAM
systemd    1424 whiskeyo    2u  unix 0x0000000086842cbe      0t0   34255 type=STREAM
systemd    1424 whiskeyo    3u  unix 0x0000000092284ad9      0t0   34264 type=DGRAM
systemd    1424 whiskeyo   12u  unix 0x000000000afc4bf2      0t0   34288 /run/user/1000/pulse
systemd    1424 whiskeyo   16u  unix 0x000000007e49caf2      0t0   34278 /run/user/1000/syste
systemd    1424 whiskeyo   17u  unix 0x00000000d7025797      0t0   34280 type=DGRAM
systemd    1424 whiskeyo   18u  unix 0x00000000d10e4c25      0t0   34281 type=DGRAM
systemd    1424 whiskeyo   19u  unix 0x000000009dc1bba2      0t0   34282 /run/user/1000/syste
systemd    1424 whiskeyo   20u  unix 0x00000000404301a3      0t0   34284 type=STREAM
```

Po wybraniu jakiegoś *node*'a możemy skorzystać z polecenia

```
netstat -p | grep [NODE]
```

I na przykład dla pierwszego wpisu z `lsof -Ua`, czyli *node*'a `34255` uzyskałem takie wyjście:

```
unix  3      [ ]         STREAM     CONNECTED     34255    1424/systemd  
```

Czyli `systemd` utworzyło to gniazdo.

Aby dowiedzieć się, które gniazda należą do wybranego procesu, musimy najpierw znaleźć PID procesu, np. poleceniem `ps -ef | grep teams`, a następnie uzyskany PID podstawić w poleceniu:

```
netstat --all --program | grep [PID]
```

Uzyskałem takie wyjście:

```
unix  3      [ ]         STREAM     CONNECTED     2585221  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2585222  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2593956  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2587157  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2587159  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2593455  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2587155  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2587158  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2588109  12370/teams --type=  
unix  3      [ ]         STREAM     CONNECTED     2588117  12370/teams --type=  
```

`ss(8)` jest kolejnym programem do podglądania socketów, wyświetla podobne informacje do `netstat`. Poleceniem `ss -s` możemy wyświetlić statystyki połączeń. Za pomocą opcji możemy wyświetlić bardziej sprecyzowane wyniki: `-4` lub `-6` wyświetli gniazda odpowiednio dla IPv4 lub IPv6, opcje `-t`, `-u`, itp. wyświetlą tylko gniazda TCP, UDP. Myślę, że bardziej czytelne jednak są długie opcje, jako że nie musimy się zastanawiać co się dokładnie wyświetli (np. `--tcp`, `--ipv6`, itp.). W przypadku bezpołączeniowego protokołu UDP należy raczej skorzystać z polecenia `ss -au`, jako że w przeciwnym wypadku (samo `ss -u`) prawdopodobnie nic nie wylistuje. Jeśli chcemy, aby nazwy numeryczne nie były rozwijane, to należy dodać opcję `-n`. Po dodaniu opcji `-p` ukażą nam się nazwa procesu, jego PID oraz wybrany deskryptor. Można również szukać tylko gniazd o wybranym stanie, dodając do polecenia `state [stan]`, gdzie stanem mogą być: `established`, `syn-sent`, `syn-recv` i inne, wypisane w manualu w sekcji `STATE-FILTER`. Przydatną opcją może być również wyszukiwanie połączeń o porcie docelowym: `ss -nt '( dst :443 or dst :80 )'`.

Polecenie `lsof -U` wylistuje wszystkie pliki gniazd uniksowych.

### Zadanie 7

Prosty skrypt z przekierowaniem sieciowym:

```bash=
#!/bin/bash

echo "Hello" > /dev/tcp/127.0.0.1/1234
#                    ^      ^       ^
#                    |      |       |
#                protokol  IP     port   
```

Ten plik oczywiście nie istnieje, podobnie jak katalog `/dev/tcp`.

A słuchanie na tym porcie robimy *netcatem*:
```
nc -l -p 1234
```

### Zadanie 9

DBus to system komunikacji międzyprocesowej *(ang. inter-process communication, IPC)* dla aplikacji. Zwykle uruchamia się jako `dbus-daemon`. Podstawowym narzędziem są busy (magistrale?) do których mogą podłączać się programy w celu odczytywania wiadomości poruszających się po niej. Poleceniem `dbus-monitor` możemy obserwować co się dzieje na magistrali, np. po odłączeniu zasilacza uzyskałem następujący komunikat:

```
signal time=1616460080.544943 sender=:1.35 -> destination=(null destination) serial=26935 path=/org/freedesktop/UPower; interface=org.freedesktop.DBus.Properties; member=PropertiesChanged
   string "org.freedesktop.UPower"
   array [
      dict entry(
         string "OnBattery"
         variant             boolean true
      )
   ]
   array [
   ]
```

Wyjście można jednak bardziej "sprecyzować", w tym celu polecenie edytujemy na takie: `dbus-monitor "type='signal'"`. Dzięki temu obserwować będziemy tylko sygnały, bez takich tablic jak widać powyżej. Odłączenie zasilacza wygląda tak:

```
signal time=1616460377.477436 sender=:1.1130 -> destination=(null destination) serial=876 path=/org/kde/Solid/PowerManagement; interface=org.kde.Solid.PowerManagement; member=profileChanged
   string "Battery"
signal time=1616460377.477453 sender=:1.1130 -> destination=(null destination) serial=877 path=/org/freedesktop/PowerManagement/Inhibit; interface=org.freedesktop.PowerManagement; member=PowerSaveStatusChanged
   boolean true
signal time=1616460377.477458 sender=:1.1130 -> destination=(null destination) serial=878 path=/org/freedesktop/PowerManagement; interface=org.freedesktop.PowerManagement; member=PowerSaveStatusChanged
   boolean true
```

A jego podłączenie tak:

```
signal time=1616460469.572350 sender=:1.1130 -> destination=(null destination) serial=887 path=/org/kde/Solid/PowerManagement; interface=org.kde.Solid.PowerManagement; member=batteryRemainingTimeChanged
   uint64 518000
signal time=1616460469.576118 sender=:1.1130 -> destination=(null destination) serial=888 path=/org/kde/Solid/PowerManagement; interface=org.kde.Solid.PowerManagement; member=profileChanged
   string "AC"
signal time=1616460469.576135 sender=:1.1130 -> destination=(null destination) serial=889 path=/org/freedesktop/PowerManagement/Inhibit; interface=org.freedesktop.PowerManagement; member=PowerSaveStatusChanged
   boolean false
signal time=1616460469.576141 sender=:1.1130 -> destination=(null destination) serial=890 path=/org/freedesktop/PowerManagement; interface=org.freedesktop.PowerManagement; member=PowerSaveStatusChanged
   boolean false
```

Widzimy takie różnice jak zmiana trybu zasilania z baterii na AC, czy wyłączenie oszczędzania energii po podłączeniu zasilacza.

Jako że siedzę obok routera, ale nie mam podłączonego laptopa do sieci kablem, to wyświetla się również komunikat o sile zasięgu:

```
signal time=1616460810.067899 sender=:1.6 -> destination=(null destination) serial=2578 path=/org/freedesktop/NetworkManager/AccessPoint/43; interface=org.freedesktop.NetworkManager.AccessPoint; member=PropertiesChanged
   array [
      dict entry(
         string "Strength"
         variant             byte 100
      )
   ]
```

Skorzystałem z [wiki Ubuntu](https://wiki.ubuntu.com/DebuggingDBus) aby dowiedzieć się jak sprawić, żeby wyjście było znacznie bardziej czytelne (zwykle pojawia się tablica na kilkanaście długości terminala z danymi zapisanymi w hex). 

Pod koniec, gdy już miałem kończyć działanie `dbus-monitor`, ujrzałem taki komunikat:

```
method call time=1616460990.164555 sender=:1.57 -> destination=org.freedesktop.UPower serial=107 path=/org/freedesktop/UPower/devices/DisplayDevice; interface=org.freedesktop.DBus.Properties; member=Get
   string "org.freedesktop.UPower.Device"
   string "IsPresent"
method return time=1616460990.164576 sender=:1.35 -> destination=:1.116 serial=89 reply_serial=567
   array [
      dict entry(
         string "NativePath"
         variant             string "BAT0"
      )
      dict entry(
         string "Vendor"
         variant             string "Notebook"
      )
      dict entry(
         string "Model"
         variant             string "BAT"
      )
      dict entry(
         string "Serial"
         variant             string "0001"
      )
      dict entry(
         string "UpdateTime"
         variant             uint64 1616460990
      )
      dict entry(
         string "Type"
         variant             uint32 2
      )
      dict entry(
         string "PowerSupply"
         variant             boolean true
      )
      dict entry(
         string "HasHistory"
         variant             boolean true
      )
      dict entry(
         string "HasStatistics"
         variant             boolean true
      )
      dict entry(
         string "Online"
         variant             boolean false
      )
      dict entry(
         string "Energy"
         variant             double 51.6816
      )
      dict entry(
         string "EnergyEmpty"
         variant             double 0
      )
      dict entry(
         string "EnergyFull"
         variant             double 51.6816
      )
      dict entry(
         string "EnergyFullDesign"
         variant             double 59.94
      )
      dict entry(
         string "EnergyRate"
         variant             double 0
      )
      dict entry(
         string "Voltage"
         variant             double 12.63
      )
      dict entry(
         string "Luminosity"
         variant             double 0
      )
      dict entry(
         string "TimeToEmpty"
         variant             int64 0
      )
      dict entry(
         string "TimeToFull"
         variant             int64 0
      )
      dict entry(
         string "Percentage"
         variant             double 100
      )
      dict entry(
         string "Temperature"
         variant             double 0
      )
      dict entry(
         string "IsPresent"
         variant             boolean true
      )
      dict entry(
         string "State"
         variant             uint32 4
      )
      dict entry(
         string "IsRechargeable"
         variant             boolean true
      )
      dict entry(
         string "Capacity"
         variant             double 86.2222
      )
      dict entry(
         string "Technology"
         variant             uint32 1
      )
      dict entry(
         string "WarningLevel"
         variant             uint32 1
      )
      dict entry(
         string "BatteryLevel"
         variant             uint32 1
      )
      dict entry(
         string "IconName"
         variant             string "battery-full-charged-symbolic"
      )
   ]
```

Są to prawdopodobnie wszystkie informacje przekazywane przez magistralę o baterii.

Można zauważyć przemykające komunikaty o zmianie jasności:
```
method call time=1616462047.426962 sender=:1.38 -> destination=:1.68 serial=32719 path=/org/gnome/SettingsDaemon/Power; interface=org.freedesktop.DBus.Properties; member=Set
   string "org.gnome.SettingsDaemon.Power.Screen"
   string "Brightness"
   variant       int32 31
method return time=1616462047.428035 sender=:1.68 -> destination=:1.38 serial=458 reply_serial=32719
signal time=1616462047.445903 sender=:1.68 -> destination=(null destination) serial=459 path=/org/gnome/SettingsDaemon/Power; interface=org.freedesktop.DBus.Properties; member=PropertiesChanged
   string "org.gnome.SettingsDaemon.Power.Screen"
   array [
      dict entry(
         string "Brightness"
         variant             int32 31
      )
   ]
   array [
   ]

```

Czy sygnały powstałe po zamknięciu powiadomień:

```
signal time=1616462198.674236 sender=:1.55 -> destination=(null destination) serial=1827 path=/org/freedesktop/Notifications; interface=org.freedesktop.Notifications; member=NotificationClosed
   uint32 330
   uint32 2
```