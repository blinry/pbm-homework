set terminal svg size 1024,1024
set output 'ex02_2.svg'

set object 1 rect from screen 0, 0, 0 to screen 1, 1, 0 behind
set object 1 rect fc  rgb "white"  fillstyle solid 1.0

set style data lines

set xrange [-6e12:6e12]
set yrange [-6e12:6e12]

set size square

set style line 1 lt 1 pt 7 lc 1 ps 0.1 lw 0.075
set style line 2 lt 1 pt 7 lc 2 ps 0.1 lw 0.075
set style line 3 lt 1 pt 7 lc 3 ps 0.1 lw 0.075

plot 'rk2.dat' using 2:3 title 'RK' ls 1 w lp, 'rk2.dat' using 4:5 ls 1 w lp notitle, \
     'euler2.dat' using 2:3 title 'Euler' ls 2 w lp, 'euler2.dat' using 4:5 ls 2 w lp notitle, \
     'adaptive2.dat' using 2:3 title 'Adaptive' ls 3 w lp, 'adaptive2.dat' using 4:5 ls 3 w lp notitle

