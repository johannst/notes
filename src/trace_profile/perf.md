# perf(1)

```
perf list     show supported hw/sw events & metrics
  -v ........ print longer event descriptions
  --details . print information on the perf event names
              and expressions used internally by events

perf stat
  -p <pid> ..... show stats for running process
  -I <ms> ...... show stats periodically over interval <ms>
  -e <ev> ...... select event(s)
  -M <met> ..... print metric(s), this adds the metric events
  --all-user ... configure all selected events for user space
  --all-kernel . configure all selected events for kernel space

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
  -e <ev> ................ select event(s)
  --all-user ... configure all selected events for user space
  --all-kernel . configure all selected events for kernel space

perf report
  -n .................... annotate symbols with nr of samples
  --stdio ............... report to stdio, if not presen tui mode
  -g graph,0.5,callee ... show callee based call chains with value >0.5
```

```
Useful <ev>:
  page-faults
  minor-faults
  major-faults
  cpu-cycles`
  task-clock
```

## Select specific events

Events to sample are specified with the `-e` option, either pass a comma
separated list or pass `-e` multiple times.

Events are specified in the following form `name[:modifier]`. The list and
description of the `modifier` can be found in the
[`perf-list(1)`][man-perf-list] manpage under `EVENT MODIFIERS`.
```sh
# L1 i$ misses in user space
# L2 i$ stats in user/kernel space mixed
# Sample specified events.
perf stat -e L1-icache-load-misses:u \
          -e l2_rqsts.all_code_rd:uk,l2_rqsts.code_rd_hit:k,l2_rqsts.code_rd_miss:k \
          -- stress -c 2
```

The `--all-user` and `--all-kernel` options append a `:u` and `:k` modifier to
all specified events. Therefore the following two command lines are equivalent.
```sh
# 1)
perf stat -e cycles:u,instructions:u -- ls

# 2)
perf stat --all-user -e cycles,instructions -- ls
```

### Raw events

In case perf does not provide a _symbolic_ name for an event, the event can be
specified in a _raw_ form as `r + UMask + EventCode`.

The following is an example for the [L2_RQSTS.CODE_RD_HIT][l2i-req-ev] event
with `EventCode=0x24` and `UMask=0x10` on my laptop with a `sandybridge` uarch.
```sh
perf stat -e l2_rqsts.code_rd_hit -e r1024 -- ls
# Performance counter stats for 'ls':
#
#       33.942      l2_rqsts.code_rd_hit
#       33.942      r1024
```

### Find raw performance counter events (intel)

The [`intel/perfmon`][perfmon] repository provides a performance event
databases for the different intel uarchs.

The table in [`mapfile.csv`][perfmon-map] can be used to lookup the
corresponding uarch, just grab the family model from the procfs.
```sh
 cat /proc/cpuinfo | awk '/^vendor_id/  { V=$3 }
                          /^cpu family/ { F=$4 }
                          /^model\s*:/  { printf "%s-%d-%x\n",V,F,$3 }'
```
> The table in [performance monitoring events][perfmon-kinds] describes how
> events are sorted into the different files.

### Raw events for perfs own symbolic names

Perf also defines some own _symbolic_ names for events. An example is the
`cache-references` event. The [`perf_event_open(2)`][man-perf-ev-open] manpage
gives the following description.
```man
perf_event_open(2)

PERF_COUNT_HW_CACHE_REFERENCES
    Cache accesses.  Usually this indicates Last Level Cache accesses but this
    may vary depending on your CPU.  This may include prefetches and coherency
    messages; again this depends on the design of your CPU.
```

The `sysfs` can be consulted to get the concrete performance counter on the
given system.
```sh
cat /sys/devices/cpu/events/cache-misses
# event=0x2e,umask=0x41
```

## [`Flamegraph`](https://github.com/brendangregg/FlameGraph)

### Flamegraph with single event trace
```
perf record -g -e cpu-cycles -p <pid>
perf script | FlameGraph/stackcollapse-perf.pl | FlameGraph/flamegraph.pl > cycles-flamegraph.svg
```

### Flamegraph with multiple event traces
```sh
perf record -g -e cpu-cycles,page-faults -p <pid>
perf script --per-event-dump
# fold & generate as above
```

## Examples
### Determine theoretical max instructions per cycle

```c
{{#include src/noploop.c }}
```

```sh
perf stat -e cycles,instructions ./noploop
# Performance counter stats for './noploop':
#
#     1.031.075.940      cycles
#     4.103.534.341      instructions       #    3,98  insn per cycle
```

### Caller vs callee callstacks

The following gives an example for a scenario where we have the following calls
- `main -> do_foo() -> do_work()`
- `main -> do_bar() -> do_work()`

```sh
perf report --stdio -g graph,callee

# Children      Self  Command  Shared Object         Symbols
# ........  ........  .......  ....................  .................
#
#  49.71%    49.66%   bench    bench                 [.] do_work
#          |
#           --49.66%--_start                <- callstack bottom
#                     __libc_start_main
#                     0x7ff366c62ccf
#                     main
#                     |
#                     |--25.13%--do_bar
#                     |          do_work    <- callstack top
#                     |
#                      --24.53%--do_foo
#                                do_work

perf report --stdio -g graph,callee

# Children      Self  Command  Shared Object         Symbols
# ........  ........  .......  ....................  .................
#
#  49.71%    49.66%   bench    bench                 [.] do_work
#          |
#          ---do_work                       <- callstack top
#             |
#             |--25.15%--do_bar
#             |          main
#             |          0x7ff366c62ccf
#             |          __libc_start_main
#             |          _start             <- callstack bottom
#             |
#              --24.55%--do_foo
#                        main
#                        0x7ff366c62ccf
#                        __libc_start_main
#                        _start             <- callstack bottom
```

## References
- [intel/perfmon][perfmon] - intel PMU event database per uarch
- [intel/perfmon-html][perfmon-html] - a html rendered version of the PMU events with search
- [intel/perfmon/mapfile.csv][perfmon-map] - processor family to uarch mapping
- [linux/perf/events][perf-pmu-ev] - x86 PMU events known to perf tools
- [linux/arch/events][x86-core-ev] - x86 PMU events linux kernel
- [wikichip] - computer architecture wiki
- [perf-list(1)][man-perf-list] - manpage
- [perf_event_open(2)][man-perf-ev-open] - manpage


[perfmon-html]: https://perfmon-events.intel.com/
[perfmon]: https://github.com/intel/perfmon
[perfmon-map]: https://github.com/intel/perfmon/blob/main/mapfile.csv
[perfmon-kinds]: https://github.com/intel/perfmon/tree/main#performance-monitoring-events

[perf-pmu-ev]: https://github.com/torvalds/linux/tree/master/tools/perf/pmu-events/arch/x86
[x86-core-ev]: https://github.com/torvalds/linux/blob/master/arch/x86/events/intel/core.c
[l2i-req-ev]: https://github.com/intel/perfmon/blob/09c155f72e1b8f14b09aea346a35467a03a7d62b/SNB/events/sandybridge_core.json#L808

[man-perf-ev-open]: https://man7.org/linux/man-pages/man2/perf_event_open.2.html
[man-perf-list]: https://man7.org/linux/man-pages/man1/perf-list.1.html

[wikichip]: https://en.wikichip.org/wiki/WikiChip
