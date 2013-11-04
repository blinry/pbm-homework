set terminal png size 1024,1024
set output 'ex01.png'

set style data lines

set xrange [-6e12:6e12]
set yrange [-6e12:6e12]

set size square

plot 'ex01.dat' using 2:3 title 'Sun', '' using 4:5 title 'Mercury', '' using 6:7 title 'Venus', '' using 8:9 title 'Earth', '' using 10:11 title 'Mars', '' using 12:13 title 'Jupiter', '' using 14:15 title 'Saturn', '' using 16:17 title 'Uranus', '' using 18:19 title 'Neptune'

