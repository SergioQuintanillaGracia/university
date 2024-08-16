set title "SelectionSort time analysis"

set xrange [0:10000]
set yrange [-10:]
set xtics 1000
set ytics 3000
set xlabel "Size"
set ylabel "Microseconds"
set key left
set grid

#set term pdf colour enhanced solid
#set output "linearSearch.pdf"

plot "selectionSort.out" using 1:2 title "Average" with points

pause -1 "Press ENTER to continue ... "

f(x) = a*x*x + b*x + c
fit f(x) "selectionSort.out" using 1:2 via a,b,c
replot f(x)
pause -1 "Press ENTER to continue ... "

print "f(", 10**9, ") = ", f(10**9)
print "g(", 10**9, ") = ", g(10**9)
