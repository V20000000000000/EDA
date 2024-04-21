reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 0,5 to 3,8
set label "M1" at 1,6 center
set object 2 rect from 0,3 to 2,5
set label "M2" at 1,4 center
set object 3 rect from 0,0 to 3,3
set label "M3" at 1,1 center
set object 4 rect from 3,3 to 4,4
set label "M4" at 3,3 center
set object 5 rect from 4,3 to 6,5
set label "M5" at 5,4 center
set object 6 rect from 3,0 to 5,2
set label "M6" at 4,1 center
set xtics 1
set ytics 1
plot [0:6][0:8]0
set terminal png size 1024, 768
set output "output.png"
replot
