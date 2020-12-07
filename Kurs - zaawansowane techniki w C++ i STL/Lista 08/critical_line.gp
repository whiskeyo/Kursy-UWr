
set terminal png size 1000,600
set output "critical_line.png"
#set xrange[min:max]
#set yrange[0:]
set grid
# show grid
plot "results.csv" using 1:2 w line lw 3 title "Real part", \
     "results.csv" using 1:3 w line lw 3 title "Imaginary part"