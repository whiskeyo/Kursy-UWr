set terminal png size 1000,1000
set output 'plot1.png'
set title "Wykres usrednionych pomiarow czasu dla roznych wielkosci kafelka"
set xlabel "Rozmiar kafelka"
set ylabel "Czas obliczenia w sekundach"
plot "plot1.dat" using 1:2 with linespoints