# glibc

## malloc tracer [`mtrace(3)`][mtrace]
Trace memory allocation and de-allocation to detect memory leaks.
Need to call `mtrace(3)` to install the tracing hooks.

If we can't modify the binary to call `mtrace` we can create a small shared
library and pre-load it.
```c
// libmtrace.c
#include <mcheck.h>
__attribute__((constructor))  static void init_mtrace() { mtrace(); }
```

Compile as:
```bash
gcc -shared -fPIC -o libmtrace.so libmtrace.c
```

To generate the trace file run:
```bash
export MALLOC_TRACE=<file>
LD_PRELOAD=./libmtrace.so <binary>
```
**Note**: If `MALLOC_TRACE` is not set `mtrace` won't install tracing hooks.

To get the results of the trace file:
```bash
mtrace <binary> $MALLOC_TRACE
```

## malloc check [`mallopt(3)`][mallopt]
Configure action when glibc detects memory error.

```bash
export MALLOC_CHECK_=<N>
```

Useful values:
```markdown
1   print detailed error & continue
3   print detailed error + stack trace + memory mappings & abort
7   print simple error message + stack trace + memory mappings & abort
```

[mtrace]: http://man7.org/linux/man-pages/man3/mtrace.3.html
[mallopt]: http://man7.org/linux/man-pages/man3/mallopt.3.html
