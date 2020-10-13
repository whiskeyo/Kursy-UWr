set title "A very important figure"

# set terminal postscript eps enhanced
set terminal pngcairo size 640,480 enhanced font 'Arial,10'

set xlabel "X-AXIS"
set ylabel "Y-AXIS"

set style line 1 linecolor rgb '#0060ad' linetype 1
set style line 2 linecolor rgb '#dd181f' linetype 1

plot "figure.dat" using 1:2:3 with yerrorbars notitle linestyle 1, \
     "figure.dat" using 1:2 with lines title "f(x)" linestyle 2

# vim: ft=gnuplot
