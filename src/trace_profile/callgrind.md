# callgrind

Callgrind is a tracing profiler which records the function call history of a
target program and collects the number of executed instructions. It is part of
the [valgrind][callgrind] tool suite.

Profiling data is collected by instrumentation rather than sampling of the
target program.

Callgrind does not capture the actual time spent in a function but computes the
*inclusive* & *exclusive* cost of a function based on the instructions fetched
(`Ir = Instruction read`). This provides reproducibility and high-precision and
is a major difference to sampling profilers like `perf` or `vtune`.
Therefore effects like slow IO are not reflected, which should be kept in mind
when analyzing callgrind results.

By default the profiler data is dumped when the target process is terminating,
but [callgrind_control] allows for interactive control of callgrind.
```bash
# Run a program under callgrind.
valgrind --tool=callgrind -- <prog> [<args>]

# Interactive control of callgrind.
callgrind_control [opts]
  opts:
    -b ............. show current backtrace
    -e ............. show current event counters
    -s ............. show current stats
    --dump[=file] .. dump current collection
    -i=on|off ...... turn instrumentation on|off
```

Results can be analyzed by using one of the following tools
- [callgrind_annotate] (cli)
  ```sh
  # Show only specific trace events (default is all).
  callgrind_annotate --show=Ir,Dr,Dw [callgrind_out_file]
  ```
- [kcachegrind] (ui)

The following is a collection of frequently used callgrind options.
```bash
valgrind --tool=callgrind [opts] -- <prog>
  opts:
    --callgrind-out-file=<file> .... output file, rather than callgrind.out.<pid>
    --dump-instr=<yes|no> .......... annotation on instrucion level,
                                     allows for asm annotations

    --instr-atstart=<yes|no> ....... control if instrumentation is enabled from 
                                     beginning of the program

    --separate-threads=<yes|no> .... create separate output files per thread,
                                     appends -<thread_id> to the output file

    --cache-sim=<yes|no> ........... control if cache simulation is enabled
```

## Trace events

By default callgrind collects following events:
- `Ir`: Instruction read

Callgrind also provides a functional cache simulation with their own model,
which is enabled by passing `--cache-sim=yes`.
This simulates a 2-level cache hierarchy with separate L1 *instruction* and
*data* caches (`L1i`/ `L1d`) and a *unified* last level (`LL`) cache.
When enabled, this collects the following additional events:
- `I1mr`: L1 cache miss on instruction read
- `ILmr`: LL cache miss on instruction read
- `Dr`: Data reads access
- `D1mr`: L1 cache miss on data read
- `DLmr`: LL cache miss on data read
- `Dw`: Data write access
- `D1mw`: L1 cache miss on data write
- `DLmw`: LL cache miss on data write

## Profile specific part of the target
Programmatically enable/disable instrumentation using the macros defined in
the callgrind header.
```c
#include <valgrind/callgrind.h>

int main() {
    // init ..

    CALLGRIND_START_INSTRUMENTATION;
    compute();
    CALLGRIND_STOP_INSTRUMENTATION;

    // shutdown ..
}
```
> In this case, callgrind should be launched with `--instr-atstart=no`.

Alternatively instrumentation can be controlled with `callgrind_control -i
on/off`.

The files [cg_example.cc](callgrind/cg_example.cc) and
[Makefile](callgrind/Makefile) provide a full example.

[callgrind]: https://valgrind.org/docs/manual/cl-manual.html
[callgrind_annotate]: https://valgrind.org/docs/manual/cl-manual.html#cl-manual.callgrind_annotate-options
[callgrind_control]: https://valgrind.org/docs/manual/cl-manual.html#cl-manual.callgrind_control-options
[kcachegrind]: https://kcachegrind.github.io/html/Home.html
