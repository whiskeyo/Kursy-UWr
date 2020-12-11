set terminal png size 3000,1800
set output "critical_line_opt.png"
set grid
plot "results_opt.csv" using 1:2 w line lw 3 title "Real part", \
     "results_opt.csv" using 1:3 w line lw 3 title "Imaginary part"