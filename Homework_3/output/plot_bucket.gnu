set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'bucket_sort_complexity.png'

set title 'Bucket Sort - Complejidad O(n)'
set xlabel 'Tamaño del Array (n)'
set ylabel 'Tiempo (segundos)'
set grid
set key left top

plot 'bucket_sort_data.txt' using 1:2 with linespoints title 'Tiempo experimental', \
     'theoretical_data.txt' using 1:2 with lines title 'O(n) teórico' linewidth 2
