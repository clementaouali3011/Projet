set terminal png size 800,800
set output 'images/res_d1.png'
set style data histogram
set ylabel "DRIVER NAMES"
set xlabel "DISTANCE (Km)"
set title "Option -d2 : Distance = f(Driver)"
set style fill solid
set datafile separator";"

set autoscale yfix
set offset 0,1,0.5,0.5
set xrange [0:*]
set yrange reverse

plot 'temp/res_d1.dat' using ($2*0.5):0:($2*0.5):(0.4):yticlabel(1) with boxxyerrorbars notitle linecolor 'aquamarine'
unset key
replot
