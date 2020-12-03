reset
n=100
max=1000
min=1
width=(max-min)/n
hist(x,width)=width*floor(x/width)+width/2.0
set terminal png size 1000,1000
set output "all.png"
set xrange[min:max]
set yrange[0:]
#set offset graph 0.05,0.05,0.05,0.0
set xtics min,(max-min)/5,max
set boxwidth width*0.9
# set style function filledcurves y1=0
set style fill transparent solid 0.5 noborder
set tics out nomirror
set xlabel "values"
set ylabel "frequency"
plot "binomial.csv" u (hist($1,width)):(1.0) smooth frequency w boxes lc rgb"green" title "Binomial distribution", \
     "uniform.csv" u (hist($1,width)):(1.0) smooth frequency w boxes lc rgb"red" title "Uniform distribution", \
     "normal.csv" u (hist($1,width)):(1.0) smooth frequency w boxes lc rgb"blue" title "Normal distribution"

# reset
# n=100
# max=1000
# min=1
# width=(max-min)/n
# hist(x,width)=width*floor(x/width)+width/2.0
# set term png
# set output "uniform.png"
# set xrange[min:max]
# set yrange[0:]
# set offset graph 0.05,0.05,0.05,0.0
# set xtics min,(max-min)/5,max
# set boxwidth width*0.9
# set style fill solid 0.5
# set tics out nomirror
# set xlabel "values"
# set ylabel "frequency"
# plot "uniform.csv" u (hist($1,width)):(1.0) smooth freq w boxes lc rgb"green" notitle

# reset
# n=100
# max=1000
# min=1
# width=(max-min)/n
# hist(x,width)=width*floor(x/width)+width/2.0
# set term png
# set output "binomial.png"
# set xrange[min:max]
# set yrange[0:]
# set offset graph 0.05,0.05,0.05,0.0
# set xtics min,(max-min)/5,max
# set boxwidth width*0.9
# set style fill solid 0.5
# set tics out nomirror
# set xlabel "values"
# set ylabel "frequency"
# plot "binomial.csv" u (hist($1,width)):(1.0) smooth freq w boxes lc rgb"green" notitle

# reset
# n=100
# max=1000
# min=1
# width=(max-min)/n
# hist(x,width)=width*floor(x/width)+width/2.0
# set term png
# set output "normal.png"
# set xrange[min:max]
# set yrange[0:]
# set offset graph 0.05,0.05,0.05,0.0
# set xtics min,(max-min)/5,max
# set boxwidth width*0.9
# set style fill solid 0.5
# set tics out nomirror
# set xlabel "values"
# set ylabel "frequency"
# plot "normal.csv" u (hist($1,width)):(1.0) smooth freq w boxes lc rgb"green" notitle