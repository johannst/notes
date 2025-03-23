# tracy(1)

[Tracy][gh-tracy] is a frame profiler, supporting manual code instrumentation
and providing a sampling profiler.

One can either record and visualize the profiling data live using
`tracy-profiler` or record the profiling data to a file using `tracy-capture`.

```
tracy-profiler [file] [-p port]

tracy-capture -o file [-f] [-p port]
    -f   overwrite <file> if it exists
```

## Example

The example showcases different cases:
1. Use tracy from a single binary. In that case the `TracyClient.cpp` can be
   directly linked / included in the instrumented binary.
2. Use tracy from different binaries (eg main executable + shared library). In
   this case the `TracyClient.cpp` should be compiled into its own shared
   library, such that there is a single tracy client.
3. Use tracy from different binaries on windows. In this case the
   `TracyClient.cpp` must be compiled again into a separate shared library,
   while defining `TRACY_EXPORTS`. The code being instrumented must be compiled
   with `TRACY_IMPORTS` defined.

An instrumented `c++` example:
```cpp
{{#include tracy/main.cpp}}
```

An instrumented `c` example:
```c
{{#include tracy/foo.c}}
```

Raw build commands to demonstrate compiling tracy w/o `cmake`, in case we need
to integrate it into a different build system.
```makefile
{{#include tracy/Makefile}}
```
> Find `get-tracy.sh` [here][get-tracy].

[gh-tracy]: https://github.com/wolfpld/tracy
[gh-doc]: https://github.com/wolfpld/tracy/releases/latest/download/tracy.pdf
[get-tracy]: https://github.com/johannst/notes/blob/master/src/trace_profile/tracy/get-tracy.sh
