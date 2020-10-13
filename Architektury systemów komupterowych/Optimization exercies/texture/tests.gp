set terminal png size 1000,1000
set output 'tests.png'
set title "Wykres czasow dzialania i IPC dla roznych implementacji"
set xlabel "Nr pomiaru"
set ylabel "Czas dzialania / IPC"
set key right center
set xrange [1:30]
plot "tests.dat" using 1:2 with linespoints title "czas index0",\
     "tests.dat" using 1:3 with linespoints title "czas index1slow",\
     "tests.dat" using 1:4 with linespoints title "czas index1fast",\
     "tests.dat" using 1:5 with linespoints title "IPC index0",\
     "tests.dat" using 1:6 with linespoints title "IPC index1slow",\
     "tests.dat" using 1:7 with linespoints title "IPC index1fast"