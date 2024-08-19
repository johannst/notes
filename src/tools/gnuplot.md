# gnuplot (1)

```
# Launch interactive shell.
gnuplot

# Launch interactive shell.
gnuplot [opt]
  opt:
    -p ................ persist plot window
    -c <file> ......... run script file
    -e "<cmd1>; .." ... run cmd(s)
```

## Frequently used configuration
```sh
# Plot title.
set title "the plot"

# Labels.
set xlabel "abc"
set ylabel "def"

# Output format, 'help set term' for all output formats.
set term svg
# Output file.
set output "out.svg"

# Make axis logarithmic to given base.
set logscale x 2

# Change separator, default is whitespace.
set datafile separator ","
```

## Plot
```sh
# With specific style (eg lines, linespoint, boxes, steps, impulses, ..).
plot "<data_file>" with <plot_style>

> cat data.txt
1 1 3
2 2 2
3 3 1
4 2 2

# Plot specific column.
plot "data.txt" using 1:2, "data.txt" using 1:3
# Equivalent using the special file "", which re-uses the previous input file.
plot "data.txt" using 1:2, "" using 1:3

# Plot piped data.
plot "< head -n2 data.txt"

# Plot with alternate title.
plot "data.txt" title "moose"
```

## Example: Specify range directly during plot
```sh
# Plot two functions in the range 0-10.
plot [0:10] 10*x, 20*x
```

## Example: multiple data sets in plot
```sh
{{#include gnuplot/mem_lat.plot}}
```

On Linux x86_64, [`mem_lat.c`](gnuplot/mem_lat.c) provides an example which can
be run as follows.
```sh
{{#include gnuplot/plot.sh:3:9}}
```
