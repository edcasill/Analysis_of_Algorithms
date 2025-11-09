set terminal pngcairo size 1200,800 enhanced font 'Arial,12'
set output 'complexity_comparison.png'

set title 'Complejidad Asintótica - Counting Sort vs Radix Sort vs Bucket Sort'
set xlabel 'Tamaño del Array (n)'
set ylabel 'Tiempo (segundos)'
set logscale xy
set grid
set key left top

plot 'counting_sort_data.txt' using 1:2 with linespoints title 'Counting Sort O(n+k)', \
     'radix_sort_data.txt' using 1:2 with linespoints title 'Radix Sort O(dn)', \
     'bucket_sort_data.txt' using 1:2 with linespoints title 'Bucket Sort O(n)', \
     'theoretical_data.txt' using 1:2 with lines title 'O(n) teórico' linewidth 2, \
     'theoretical_data.txt' using 1:3 with lines title 'O(nk) teórico' linewidth 2, \
     'theoretical_data.txt' using 1:4 with lines title 'O(nd) teórico' linewidth 2, \
     'theoretical_data.txt' using 1:5 with lines title 'O(n^2) teórico' linewidth 2
