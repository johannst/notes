# callgrind

Callgrind is a tracing profiler to record the function call history of a target
program. It is part of the [valgrind][callgrind] tool suite.

Profiling data is collected by instrumentation rather than sampling of the
target program.

Callgrind does not capture the actual time spent in a function but computes the
cost of a function based on the instructions fetched (`Ir = Instruction read`).
Therefore effects like slow IO are not reflected, which should be kept in mind
when analyzing callgrind results.

By default the profiler data is dumped when the target process is terminating,
but [callgrind_control] allows for interactive control of callgrind.
```bash
# Run a program under callgrind.
valgrind --tool=callgrind -- <prog>

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
```

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
