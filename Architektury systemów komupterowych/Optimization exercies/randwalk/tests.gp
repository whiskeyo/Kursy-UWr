set terminal png size 1000,1000
set output 'tests.png'
set title "Wykres usrednionych pomiarow czasu dla wywołania funkcji z powyższymi parametrami"
set xlabel "Nr pomiaru"
set ylabel "Czas obliczenia w sekundach"
plot "tests.dat" using 1:3 with linespoints title "randwalk0",\
"tests.dat" using 1:5 with linespoints title "randwalk1"

set terminal png size 1000,1000
set output 'times.png'
set title "Wykres pomiarow czasu dla roznych rozmiarow tablicy"
set xlabel "Nr pomiaru"
set ylabel "Czas obliczenia w sekundach"
plot "times.dat" using 1:2 with linespoints title "randwalk0, n = 4",\
"times.dat" using 1:3 with linespoints title "randwalk0, n = 5",\
"times.dat" using 1:4 with linespoints title "randwalk0, n = 6",\
"times.dat" using 1:5 with linespoints title "randwalk0, n = 7",\
"times.dat" using 1:6 with linespoints title "randwalk0, n = 8",\
"times.dat" using 1:7 with linespoints title "randwalk1, n = 4",\
"times.dat" using 1:8 with linespoints title "randwalk1, n = 5",\
"times.dat" using 1:9 with linespoints title "randwalk1, n = 6",\
"times.dat" using 1:10 with linespoints title "randwalk1, n = 7",\
"times.dat" using 1:11 with linespoints title "randwalk1, n = 8"

