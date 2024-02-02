set datafile separator ";"
set terminal pngcairo enhanced size 1400,700
set output 'images/res_s.png'
 
set title 'Option -s: distance max-min'
set ylabel 'Distance (Km)'
set xlabel 'Route ID'
set key top right

set xtics rotate by -90
set ytics nomirror
set xtics nomirror

plot 'temp/res_s.dat' using 1:3:xtic(2) with lines notitle lc rgb "#FFD700", \
'' using 1:5 with lines notitle lc rgb "#FFD700", \
'' using 1:4 with lines title "Distances Moyennes" lc rgb "#008000", \
'' using 1:3:5 with filledcurves below fill transparent solid title "Dist min -> Dist max" lc rgb "#FFD700"
