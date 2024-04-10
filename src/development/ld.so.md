# ld.so(8)

## Environment Variables
```console
LD_PRELOAD=<l_so>       colon separated list of libso's to be pre loaded
LD_DEBUG=<opts>         comma separated list of debug options
        =help           list available options
        =libs           show library search path
        =files          processing of input files
        =symbols        show search path for symbol lookup
        =bindings       show against which definition a symbol is bound
```

### LD_LIBRARY_PATH and dlopen(3)
When dynamically loading a shared library during program runtime with
`dlopen(3)`, only the `LD_LIBRARY_PATH` as it was during program startup is
evaluated.
Therefore the following is a code smell:
```c
// at startup LD_LIBRARY_PATH=/moose

// Assume /foo/libbar.so
setenv("LD_LIBRARY_PATH", "/foo", true /* overwrite */);

// Will look in /moose and NOT in /foo.
dlopen("libbar.so", RTLD_LAZY);
```

## LD_PRELOAD: Initialization Order and Link Map
Libraries specified in `LD_PRELOAD` are loaded from `left-to-right` but
initialized from `right-to-left`.

```
> ldd ./main
  >> libc.so.6 => /usr/lib/libc.so.6

> LD_PRELOAD=liba.so:libb.so ./main
           -->
    preloaded in this order
           <--
    initialized in this order
```

The preload order determines:
- the order libraries are inserted into the `link map`
- the initialization order for libraries

For the example listed above the resulting `link map` will look like the
following:

```
  +------+    +------+    +------+    +------+
  | main | -> | liba | -> | libb | -> | libc |
  +------+    +------+    +------+    +------+
```

This can be seen when running with `LD_DEBUG=files`:

```
> LD_DEBUG=files LD_PRELOAD=liba.so:libb.so ./main
  # load order (-> determines link map)
  >> file=liba.so [0];  generating link map
  >> file=libb.so [0];  generating link map
  >> file=libc.so.6 [0];  generating link map

  # init order
  >> calling init: /usr/lib/libc.so.6
  >> calling init: <path>/libb.so
  >> calling init: <path>/liba.so
  >> initialize program: ./main
```

To verify the `link map` order we let `ld.so` resolve the `memcpy(3)` libc
symbol (used in _main_) dynamically, while enabling `LD_DEBUG=symbols,bindings`
to see the resolving in action.

```
> LD_DEBUG=symbols,bindings LD_PRELOAD=liba.so:libb.so ./main
  >> symbol=memcpy;  lookup in file=./main [0]
  >> symbol=memcpy;  lookup in file=<path>/liba.so [0]
  >> symbol=memcpy;  lookup in file=<path>/libb.so [0]
  >> symbol=memcpy;  lookup in file=/usr/lib/libc.so.6 [0]
  >> binding file ./main [0] to /usr/lib/libc.so.6 [0]: normal symbol `memcpy' [GLIBC_2.14]
```

## `RTLD_LOCAL` and `RTLD_DEEPBIND`
As shown in the `LD_PRELOAD` section above, when the dynamic linker resolves
symbol relocations, it walks the link map and until the first object provides
the requested symbol.

When libraries are loaded dynamically during runtime with `dlopen(3)`, one can
control the visibility of the symbols for the loaded library. The following two
flags control this visibility.
- `RTLD_LOCAL` the symbols of the library (and its dependencies) are not
  visible in the global symbol scope and therefore do not participate in global
  symbol resolution from other libraries (default).
- `RTLD_GLOBAL` the symbols of the library are visible in the global symbol
  scope.

Additionally to the visibility one can use the `RTLD_DEEPBIND` flag to define
the lookup order when resolving symbols of the loaded library. With deep
binding, the symbols of the loaded library (and its dependencies) are searched
first before the global scope is searched. Without deep binding, the order is
reversed and the global space is searched first, which is the default.

The sources in [ldso/deepbind][src-deepbind] give a minimal example, which can
be used to experiment with the different flags and investigate their behavior.
```
main
|-> explicitly link against liblink.so
|-> dlopen(libdeep.so, RTLD_LOCAL | RTLD_DEEPBIND)
`-> dlopen(libnodp.so, RTLD_LOCAL)
```

The following snippets are taken from `LD_DEBUG` to demonstrate the
`RLTD_LOCAL` and `RTLD_DEEPBIND` flags.
```ini
# dlopen("libdeep.so", RTLD_LOCAL | RTLD_DEEPBIND)
# scopes visible to libdeep.so, where scope [0] is the local one.
object=./libdeep.so [0]
 scope 0: ./libdeep.so /usr/lib/libc.so.6 /lib64/ld-linux-x86-64.so.2
 scope 1: ./main ./libprel.so ./liblink.so /usr/lib/libc.so.6 /lib64/ld-linux-x86-64.so.2

# main: dlsym(handle:libdeep.so, "test")
symbol=test;  lookup in file=./libdeep.so [0]
binding file ./libdeep.so [0] to ./libdeep.so [0]: normal symbol `test'

# libdeep.so: dlsym(RTLD_NEXT, "next_libdeep")
symbol=next_libdeep;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=next_libdeep;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./libdeep.so: error: symbol lookup error: undefined symbol: next_libdeep (fatal)

# libdeep.so: dlsym(RTLD_DEFAULT, "default_libdeep")
# first search local scope (DEEPBIND)
symbol=default_libdeep;  lookup in file=./libdeep.so [0]
symbol=default_libdeep;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=default_libdeep;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
symbol=default_libdeep;  lookup in file=./main [0]
symbol=default_libdeep;  lookup in file=./libprel.so [0]
symbol=default_libdeep;  lookup in file=./liblink.so [0]
symbol=default_libdeep;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=default_libdeep;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./libdeep.so: error: symbol lookup error: undefined symbol: default_libdeep (fatal)

# main: dlsym(handle:libdeep.so, "libdeep_main")
symbol=libdeep_main;  lookup in file=./libdeep.so [0]
symbol=libdeep_main;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=libdeep_main;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./libdeep.so: error: symbol lookup error: undefined symbol: libdeep_main (fatal)
```

The following snippets are taken from `LD_DEBUG` to demonstrate the
`RLTD_LOCAL` flag _without_ the `RTLD_DEEPBIND` flag.
```ini
# dlopen("libdeep.so", RTLD_LOCAL)
# scopes visible to libnodp.so, where scope [0] is the global one.
object=./libnodp.so [0]
 scope 0: ./main ./libprel.so ./liblink.so /usr/lib/libc.so.6 /lib64/ld-linux-x86-64.so.2
 scope 1: ./libnodp.so /usr/lib/libc.so.6 /lib64/ld-linux-x86-64.so.2

# main: dlsym(handle:libnodp.so, "test")
symbol=test;  lookup in file=./libnodp.so [0]
binding file ./libnodp.so [0] to ./libnodp.so [0]: normal symbol `test'

# libnodp.so: dlsym(RTLD_NEXT, "next_libnodp")
symbol=next_libnodp;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=next_libnodp;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./libnodp.so: error: symbol lookup error: undefined symbol: next_libnodp (fatal)

# libnodp.so: dlsym(RTLD_DEFAULT, "default_libnodp")
# first search global scope (no DEEPBIND)
symbol=default_libnodp;  lookup in file=./main [0]
symbol=default_libnodp;  lookup in file=./libprel.so [0]
symbol=default_libnodp;  lookup in file=./liblink.so [0]
symbol=default_libnodp;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=default_libnodp;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
symbol=default_libnodp;  lookup in file=./libnodp.so [0]
symbol=default_libnodp;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=default_libnodp;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./libnodp.so: error: symbol lookup error: undefined symbol: default_libnodp (fatal)

# main: dlsym(handle:libnodp.so, "libnodp_main")
symbol=libnodp_main;  lookup in file=./libnodp.so [0]
symbol=libnodp_main;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=libnodp_main;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./libnodp.so: error: symbol lookup error: undefined symbol: libnodp_main (fatal)
```

The following is a global lookup from the main application, since
`lib{deep,nodp}.so` were loaded with `RTLD_LOCAL`, they are not visible in the
global symbol scope.
```ini
# main: dlsym(RTLD_DEFAULT, "default_main")
symbol=default_main;  lookup in file=./main [0]
symbol=default_main;  lookup in file=./libprel.so [0]
symbol=default_main;  lookup in file=./liblink.so [0]
symbol=default_main;  lookup in file=/usr/lib/libc.so.6 [0]
symbol=default_main;  lookup in file=/lib64/ld-linux-x86-64.so.2 [0]
./main: error: symbol lookup error: undefined symbol: default_main (fatal)
```

## Load lib with same name from different locations
The sources in [ldso/samename][src-samename] show some experiments, loading the
libs with the same name but potentially from different locations (paths).

## Dynamic Linking (x86_64)
Dynamic linking basically works via one indirect jump. It uses a combination of
function trampolines (`.plt` section) and a function pointer table (`.got.plt`
section).
On the first call the trampoline sets up some metadata and then jumps to the
`ld.so` runtime resolve function, which in turn patches the table with the
correct function pointer.
```
.plt ....... procedure linkage table, contains function trampolines, usually
             located in code segment (rx permission)
.got.plt ... global offset table for .plt, holds the function pointer table
```

Using `radare2` we can analyze this in more detail:

```
[0x00401040]> pd 4 @ section..got.plt
            ;-- section..got.plt:
            ;-- .got.plt:    ; [22] -rw- section size 32 named .got.plt
            ;-- _GLOBAL_OFFSET_TABLE_:
       [0]  0x00404000      .qword 0x0000000000403e10 ; section..dynamic
       [1]  0x00404008      .qword 0x0000000000000000
            ; CODE XREF from section..plt @ +0x6
       [2]  0x00404010      .qword 0x0000000000000000
            ;-- reloc.puts:
            ; CODE XREF from sym.imp.puts @ 0x401030
       [3]  0x00404018      .qword 0x0000000000401036 ; RELOC 64 puts

[0x00401040]> pd 6 @ section..plt
            ;-- section..plt:
            ;-- .plt:       ; [12] -r-x section size 32 named .plt
        ┌─> 0x00401020      ff35e22f0000   push qword [0x00404008]
        ╎   0x00401026      ff25e42f0000   jmp qword [0x00404010]
        ╎   0x0040102c      0f1f4000       nop dword [rax]
┌ 6: int sym.imp.puts (const char *s);
└       ╎   0x00401030      ff25e22f0000   jmp qword [reloc.puts]
        ╎   0x00401036      6800000000     push 0
        └─< 0x0040103b      e9e0ffffff     jmp sym..plt
```

- At address `0x00401030` in the `.plt` section we see the indirect jump for
  `puts` using the function pointer in `_GLOBAL_OFFSET_TABLE_[3] (GOT)`.
- `GOT[3]` initially points to instruction after the `puts` trampoline
  `0x00401036`.
- This pushes the relocation index `0` and then jumps to the first trampoline
  `0x00401020`.
- The first trampoline jumps to `GOT[2]` which will be filled at program
  startup by the `ld.so` with its resolve function.
- The `ld.so` resolve function fixes the relocation referenced by the
  relocation index pushed by the `puts` trampoline.
- The relocation entry at index `0` tells the resolve function which symbol to
  search for and where to put the function pointer:
  ```
  > readelf -r <main>
    >> Relocation section '.rela.plt' at offset 0x4b8 contains 1 entry:
    >>   Offset          Info           Type           Sym. Value    Sym. Name + Addend
    >> 000000404018  000200000007 R_X86_64_JUMP_SLO 0000000000000000 puts@GLIBC_2.2.5 + 0
  ```
  As we can see the offset from relocation at index `0` points to `GOT[3]`.

[src-deepbind]: https://github.com/johannst/notes/tree/master/src/development/ldso/deepbind
[src-samename]: https://github.com/johannst/notes/tree/master/src/development/ldso/samename
