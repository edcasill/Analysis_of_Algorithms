set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'counting_sort_complexity.png'

set title 'Counting Sort - Complejidad O(n + k)'
set xlabel 'Tamaño del Array (n)'
set ylabel 'Tiempo (segundos)'
set grid
set key left top

plot 'counting_sort_data.txt' using 1:2 with linespoints title 'Tiempo experimental', \
     'theoretical_data.txt' using 1:3 with lines title 'O(n + k) teórico' linewidth 2
