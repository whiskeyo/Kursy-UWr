
set terminal png size 3000,1800
set output "critical_line.png"
set grid
plot "results.csv" using 1:2 w line lw 3 title "Real part", \
     "results.csv" using 1:3 w line lw 3 title "Imaginary part"
