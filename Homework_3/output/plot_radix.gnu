set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'radix_sort_complexity.png'

set title 'Radix Sort - Complejidad O(d × n)'
set xlabel 'Tamaño del Array (n)'
set ylabel 'Tiempo (segundos)'
set grid
set key left top

plot 'radix_sort_data.txt' using 1:2 with linespoints title 'Tiempo experimental', \
     'theoretical_data.txt' using 1:4 with lines title 'O(d × n) teórico' linewidth 2
