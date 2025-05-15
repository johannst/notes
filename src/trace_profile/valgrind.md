# valgrind(1)

## Memcheck `--tool=memcheck`

Is the default tool when invoking `valgrind` without explicitly specifying
`--tool`.

Memory checker used to identify:
- memory leaks
- out of bound accesses
- uninitialized reads

```sh
valgrind [OPTIONS] PROGRAM [ARGS]
    --log-file=FILE             Write valgrind output to FILE.
    --leak-check=full           Enable full leak check.
    --show-error-list=yes       List errors at exit again (use 'all' to include suppressed).
    --track-origins=yes         Show origins of undefined values.
    --keep-debuginfo=no|yes     Keep symbols etc for unloaded code.
    --num-callers=N             Max depth for reported backtrace.

    --gen-suppressions=yes      Generate suppressions file from the run.
    --suppressions=FILE         Load suppressions file.

    --vgdb=yes                  Enable builtin gdbserver.
    --vgdb-error=N              Stop after N errors. Starting from then on,
                                each error triggers a breakpoint.
```

## Example: use gdbserver to catch valgrind errors

The following shows an example how to connect a gdb to a program running under
valgrind and stop at any valgrind error to inspect the program state.

The example program wich may access an uninitialized variable.
```c
// build: gcc -g -o uninit uninit.c

#include <stdio.h>
#include <stdlib.h>

int get(int* v, int cond) {
  if (cond)
    *v = 2;
  return *v;
}

void use(int* v) {
  printf("v=%d\n", *v);
}

int main(int argc, char* argv[]) {
  int v;
  get(&v, argc == 2);
  use(&v);
}
```

Run the program in valgrind and instruct the gdbserver to stop from the very
first valgrind error encountered.
```sh
> valgrind --vgdb=yes --vgdb-error=0 ./uninit
```

Connect gdb to valgrinds gdbserver. If one has multiple valgrind gdbserver
running, pass `--pid=` to `vgdb` to disambiguate.
```
> gdb -q -ex 'target remote | vgdb' uninit
(gdb) c
# ...
Program received signal SIGTRAP, Trace/breakpoint trap.
0x00000000048ea522 in _itoa_word (value=<optimized out>, value@entry=0, buflim=0x1fff000177 "", 
    buflim@entry=0x1fff000178 "", base=<optimized out>, upper_case=upper_case@entry=0) at _itoa.c:183
183	      SPECIAL (10);
(gdb)
```
