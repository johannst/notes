# xray-instrument

[Xray][xray] offers a function trace instrumentation which has very little
overhead when trace capturing is disabled.

The xray system consists of compile time instrumentation, a runtime library to
control capturing as well as tools for post processing.

During compilation functions are instrumented with nop space which is patched on
the fly when tracing is enabled to hook function entry and exit
points. Additional information for enabling the traces is placed in some special
sections in the elf file.

The following provides an starting point example demonstrating
- `account` to gather function call statistics.
- `convert` to convert the binary trace into a trace format that can be opened
  by other tools (eg [perfetto.dev][perfetto] or [speedscope.app][speedscope].

```make
{{#include xray/Makefile}}
```

```c
{{#include xray/main.c}}
```

[xray]: https://llvm.org/docs/XRay.html
[perfetto]: https://ui.perfetto.dev/
[speedscope]: https://www.speedscope.app/
