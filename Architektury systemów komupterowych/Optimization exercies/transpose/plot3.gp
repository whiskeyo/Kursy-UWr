set terminal png size 1000,1000
set output 'plot3.png'
set title "Wykres usrednionych pomiarow czasu dla roznych rozmiarow macierzy"
set xlabel "Rozmiar n macierzy"
set ylabel "Czas obliczenia w sekundach"
plot "plot3.dat" using 1:2 with linespoints