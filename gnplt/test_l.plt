set terminal png size 800,800
set output 'images/res_l.png'
set style data histogram
set xlabel "ROUTE ID"
set ylabel "DISTANCE (Km)"
set title "Option -l : Distance = f(Route)"
set style fill solid
set boxwidth 2.30 absolute
set datafile separator";"

set autoscale xfix
set offset 0.5,0.5,0,0
set yrange [0:*]

plot 'temp/res_l.dat' using 2:xticlabels(1) with histogram notitle linecolor 'aquamarine'
unset key
replot
