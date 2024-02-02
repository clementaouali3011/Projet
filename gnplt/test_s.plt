set datafile separator ";"
set terminal pngcairo enhanced size 1400,700
set output 'images/res_s.png'

set title 'Option -s'
set ylabel 'Distance (Km)'
set xlabel 'Route ID'
set key top right

set xtics rotate by -55
set ytics nomirror
set xtics nomirror

plot 'temp/res_s.dat' using 1:3:xtic(2) with lines notitle lc rgb "#0072BD", \
'' using 1:5 with lines notitle lc rgb "#EDB120", \
'' using 1:4 with lines title "Distances Moyennes" lc rgb "#53166D", \
'' using 1:3:5 with filledcurves below fill transparent solid title "Dist min -> Dist max" lc rgb "#0072BD"
