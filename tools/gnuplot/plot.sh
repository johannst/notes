#!/bin/bash

gcc -o mem_lat mem_lat.c -g -O3 -Wall -Werror

for stride in 32 64 128 256 512; do \
    taskset -c 1 ./mem_lat 128 $stride | tee stride_$stride.txt ; \
done

gnuplot -p -c mem_lat.plot

rm -f stride_*.txt mem_lat
