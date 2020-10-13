set terminal png size 1000,1000
set output 'misses.png'
set title "Wykres pomiarow chybien L1"
set xlabel "Nr pomiaru"
set yrange [0:100]
set ylabel "Procent chybien"
plot "tests.dat" using 1:3 with linespoints title "binsearch0",\
     "tests.dat" using 1:8 with linespoints title "binsearch1"

set terminal png size 1000,1000
set output 'times.png'
set title "Wykres czasow dzialania programu"
set xlabel "Nr pomiaru"
set ylabel "Czas dzialania w sekundach"
set yrange [0:10]
plot "tests.dat" using 1:2 with linespoints title "binsearch0",\
     "tests.dat" using 1:7 with linespoints title "binsearch1"

set terminal png size 1000,1000
set output 'tests.png'
set title "Wykres czasow dzialania programu z rozna kolejnoscia wykonywania instrukcji"
set xlabel "Nr pomiaru"
set ylabel "Czas dzialania w sekundach"
set yrange [0:8]
plot "testsIPC1.dat" using 1:2 with linespoints title "version 1",\
     "testsIPC2.dat" using 1:2 with linespoints title "version 2",\
     "testsIPC3.dat" using 1:2 with linespoints title "version 3",\

set terminal png size 1000,1000
set output 'testsipc.png'
set title "Wykres IPC z rozna kolejnoscia wykonywania instrukcji"
set xlabel "Nr pomiaru"
set ylabel "IPC"
set yrange [0.2:0.31]
plot "testsIPC1.dat" using 1:3 with linespoints title "version 1",\
     "testsIPC2.dat" using 1:3 with linespoints title "version 2",\
     "testsIPC3.dat" using 1:3 with linespoints title "version 3",\