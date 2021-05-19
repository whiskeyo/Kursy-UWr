# Kurs administrowania systemem Linux (L11)

## Lista zadań nr 11

### Zadanie 1

```shell=
$ touch disk.img
$ truncate disk.img -s $((2147483648 * 512))
# sgdisk --mbrtogpt disk.img
# gdisk disk.img
    Partition table scan:
    MBR: protective
    BSD: not present
    APM: not present
    GPT: present            <- sgdisk zadziałał poprawnie!

    Found valid GPT with protective MBR; using GPT.

    x l 1   <- wyłączenie alignmentu, żeby móc tworzyć partycje w dowolnych miejscach

      # poczatek    koniec      kod     GUID                                    nazwa
    n 1 34          2047        ef02    21686148-6449-6E6F-744E-656564454649    GNU Grub 2
    n 2 2048        262143      ef00    C12A7328-F81F-11D2-BA4B-00A0C93EC93B    ESP
    n 3 262144      2097151     8300    0FC63DAF-8483-4772-8E79-3D69D8477DE4    Linux /boot
    n 4 2097152     1065353215  8309    CA7D7CCB-63ED-4C53-861C-1742536059CC    Linux rootfs
    n 5 1065353216  1073741823  8200    0657FD6D-A4AB-43C4-84E5-0933C84B4F4F    Linux & OpenBSD swap
    n 6 1073741824  2147483614  a600    824CC7A0-36A8-11E3-890A-952519AD3F61    OpenBSD rootfs

    UWAGA: dla Linux LUKS nie wykrywa kodu ani GUID, mimo tego, że w specyfikacji jest jasno
           określone, że LUKS ma kod 8309 (https://wiki.archlinux.org/title/GPT_fdisk), też
           z tego powodu automatycznie jest przypisywany kod 8300, dlatego po wypisaniu listy
           wszystkich partycji właśnie 8300 będzie widniało obok partycji Linux rootfs

    c 1 GNU Grub 2
    c 2 ESP
    c 3 Linux /boot
    c 4 Linux rootfs
    c 5 Linux & OpenBSD swap
    c 6 OpenBSD rootfs

    x g 7C54FB1F-0CEE-497F-9557-BCE60937DEF6 <- zmiana GUID dysku
    x o <- MBR zawiera jeden wpis z kodem ee, dlatego musimy stworzyć tylko a6 i ee
    r h 6 Y a6 N Y ee

    w Y <- zapisujemy

# fdisk disk.img
    x M           <- wejście w edycję hybrydowego MBR
    i 0x7C54FB1F  <- zmiana identyfikatora dysku
    p             <- wyświetlenie informacji o partycjach, tu możemy zobaczyć, że utworzone
                     partycje w MBR mają rzeczywiście odpowiednie zakresy sektorów oraz CHS
    w
```

### Zadanie 3

Pierwszym krokiem jest stworzenie katalogu `/boot/grub/themes`, a w nim kolejnego, zawierającego nazwę naszego motywu. W katalogu `/boot/grub/themes/test/` tworzymy plik `theme.txt` i tam możemy modyfikować wszystko, jak nam odpowiada. 

Ja zdecydowałem się pobrać jakiś motyw [stąd](https://www.gnome-look.org/browse/cat/109/order/latest/), wypadło akurat na *Tela*. Pliki wypakowałem, katalog `Tela` skopiowałem do `/boot/grub/themes` i musiałem zmienić plik `/etc/default/grub` (warto zrobić sobie jego backup). Zmiana polega na dopisaniu takiej linii:

```
GRUB_THEME=/boot/grub/themes/Tela/theme.txt
```

I na koniec updateujemy grub poleceniem `update-grub` i resetujemy lapka. Jako że mieliśmy coś zmienić, to dodałem tytuł do ekranu GRUBa, żeby urozmaicić cały ekran, mówi on o tym, żeby wybrać OS który chcemy uruchomić, lub nacisnąć `E`, aby przejść do edycji parametrów bootowania. Wstępnie po restarcie ten tekst był czarny, dlatego chciałem dopasować go do pozostałych kolorów motywów, dlatego zmieniłem go na `#cccccc` korzystając z globalnego atrybutu `title-color`.