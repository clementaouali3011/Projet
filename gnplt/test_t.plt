set datafile separator ";" 
set terminal pngcairo enhanced size 800,900 
set output 'images/res_t.png' 

set style data histograms 
set style histogram clustered 
set boxwidth 1.5 
set style fill solid 
set ytics nomirror 

set title 'Option -t : NB routes=f(Towns)' 
set ylabel 'NB Routes' 
set xlabel 'Town Names' 
set xtics rotate by -45 
set size 1, 1 

plot 'temp/res_t.dat' using 2:xtic(1) title 'Total routes' lc rgb "#0072BD", \
'' using 3 title 'First town' lc rgb "#EDB120"
