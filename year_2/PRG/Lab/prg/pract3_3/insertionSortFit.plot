set xrange [0:11000]
set yrange [-10:]
set xtics 1000
set ytics 3000
set xlabel "Size"
set ylabel "Microseconds"
set key left
set grid

#set term pdf colour enhanced solid
#set output "insertionSort.pdf"

plot "insertionSort.out" using 1:2 title "Best case" with points, \
     "insertionSort.out" using 1:3 title "Worst case" with points, \
     "insertionSort.out" using 1:4 title "Average case" with points

pause -1 "Press ENTER to continue ... "

f(x) = a*x + b
fit f(x) "insertionSort.out" using 1:2 via a,b
replot f(x)
pause -1 "Press ENTER to continue ... "

g(x) = c*x*x + d*x + e
fit g(x) "insertionSort.out" using 1:3 via c,d,e
replot g(x)
pause -1 "Press ENTER to continue ... "

h(x) = i*x*x + j*x + k
fit h(x) "insertionSort.out" using 1:4 via i,j,k
replot h(x)
pause -1 "Press ENTER to continue ... "

print "f(", 800000, ") = ", f(800000)
print "g(", 800000, ") = ", g(800000)
print "h(", 800000, ") = ", h(800000)
