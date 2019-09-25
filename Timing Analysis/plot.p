set terminal png
set output 'size1000.png'

set style fill solid 0.5 border -1
set style boxplot outliers pointtype 7
set style data boxplot
set boxwidth  0.5
set pointsize 0.5

unset key
set border 2
set xtics ("mkl" 1, "openblas" 2, "iteration" 3, "pthread" 4) scale 0.0
set xtics nomirror
set ytics nomirror
#set yrange [0:50]

plot for [i=1:4] 'mkl_size1000.txt' using (i):i notitle




