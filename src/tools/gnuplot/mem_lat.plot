# file: mem_lat.plot

set title  "memory latency (different strides)"
set xlabel "array in KB"
set ylabel "cycles / access"

set logscale x 2

plot "stride_32.txt"  title "32"  with linespoints, \
     "stride_64.txt"  title "64"  with linespoints, \
     "stride_128.txt" title "128" with linespoints, \
     "stride_256.txt" title "256" with linespoints, \
     "stride_512.txt" title "512" with linespoints
