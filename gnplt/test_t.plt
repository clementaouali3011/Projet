set datafile separator ";" 
set terminal pngcairo enhanced size 800,900 
set output 'images/res_t.png' 

set style data histograms 
set style histogram clustered 
set boxwidth 1 
set style fill solid 
set ytics nomirror 

set title 'Option -t: NB routes=f(Towns)' 
set ylabel 'Nombre de Routes' 
set xlabel 'Nom de la Ville' 
set xtics rotate by -90
set size 1, 1 

plot 'temp/res_t.dat' using 2:xtic(1) title 'Nombre de Trajets' lc rgb  "#FF0000", \
'' using 3 title 'Nombre de Départs' lc rgb "#008000"
