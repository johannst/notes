# [vtune(1)][vtune]

Vtune offers different analysis. Run `vtune -collect help` to list the
availale analysis.

## Profiling

The following shows some common flows with the `hotspot` analsysis
as an example.
```
# Launch and profile process.
vtune -collect hotspots [opts] -- target [args]

# Attach and profile running process.
vtune -collect hotspots [opts] -target-pid <pid>
```
Some common options are the following.
```
-r <dir>           output directory for the profile
-no-follow-child   dont attach to to child processes (default is to follow)
-start-paused      start with paused profiling
```

## Analyze
```
vtune-gui <dir>
```

## Programmatically control sampling
Vtune offers an API to *resume* and *pause* the profile collection from within
the profilee itself.  This can be helpful if either only a certain phase should
be profiled or some phase should be skipped.

The following gives an example where only one phase in the program is profiled.
The program makes calls to the vtune API to resume and pause the collection,
while vtune is invoked with `-start-paused` to pause profiling initially.
```c
{{#include vtune/main.c::16}}
```
The makefile gives an example how to build and profile the application.
```makefile
{{#include vtune/Makefile::7}}
```

[vtune]: https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide
