# perf(1)

```markdown
perf list      show supported hw/sw events

perf stat
  -p <pid> .. show stats for running process
  -I <ms> ... show stats periodically over interval <ms>
  -e <ev> ... filter for events

perf top
  -p <pid> .. show stats for running process
  -F <hz> ... sampling frequency
  -K ........ hide kernel threads

perf record
  -p <pid> ............... record stats for running process
  -F <hz> ................ sampling frequency
  --call-graph <method> .. [fp, dwarf, lbr] method how to caputre backtrace
                           fp   : use frame-pointer, need to compile with
                                  -fno-omit-frame-pointer
                           dwarf: use .cfi debug information
                           lbr  : use hardware last branch record facility
  -g ..................... short-hand for --call-graph fp
  -e <ev> ................ filter for events

perf report
  -n .................... annotate symbols with nr of samples
  --stdio ............... report to stdio, if not presen tui mode
  -g graph,0.5,caller ... show caller based call chains with value >0.5
```

```markdown
Useful <ev>:
  page-faults
  minor-faults
  major-faults
  cpu-cycles`
  task-clock
```

## [`Flamegraph`](https://github.com/brendangregg/FlameGraph)

### Flamegraph with single event trace
```markdown
perf record -g -e cpu-cycles -p <pid>
perf script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > cycles-flamegraph.svg
```

### Flamegraph with multiple event traces
```markdown
perf record -g -e cpu-cycles,page-faults -p <pid>
perf script --per-event-dump
# fold & generate as above
```

