set title "SelectionSort Runtime"

set xrange [0:11000]
set yrange [-10:]
set xtics 1000
set ytics 4000
set xlabel "Size"
set ylabel "Microseconds"
set key left
set grid

#set term pdf colour enhanced solid
#set output "linearSearch.pdf"

plot "selectionSort.out" using 1:2 title "Average case" with points

pause -1 "Press ENTER to continue ... "

f(x) = a*x*x + b*x + c
fit f(x) "selectionSort.out" using 1:2 via a,b,c
replot f(x)

print "f(", 800000, ") = ", f(800000)
