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

```markdown
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

```makrdown
  +------+    +------+    +------+    +------+
  | main | -> | liba | -> | libb | -> | libc |
  +------+    +------+    +------+    +------+
```

This can be seen when running with `LD_DEBUG=files`:

```makrdown
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

```makrdown
> LD_DEBUG=symbols,bindings LD_PRELOAD=liba.so:libb.so ./main
  >> symbol=memcpy;  lookup in file=./main [0]
  >> symbol=memcpy;  lookup in file=<path>/liba.so [0]
  >> symbol=memcpy;  lookup in file=<path>/libb.so [0]
  >> symbol=memcpy;  lookup in file=/usr/lib/libc.so.6 [0]
  >> binding file ./main [0] to /usr/lib/libc.so.6 [0]: normal symbol `memcpy' [GLIBC_2.14]
```

## Dynamic Linking (x86_64)
Dynamic linking basically works via one indirect jump. It uses a combination of
function trampolines (`.plt` section) and a function pointer table (`.got.plt`
section).
On the first call the trampoline sets up some metadata and then jumps to the
`ld.so` runtime resolve function, which in turn patches the table with the
correct function pointer.
```makrdown
.plt ....... procedure linkage table, contains function trampolines, usually
             located in code segment (rx permission)
.got.plt ... global offset table for .plt, holds the function pointer table
```

Using `radare2` we can analyze this in more detail:

```makrdown
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
  ```makrdown
  > readelf -r <main>
    >> Relocation section '.rela.plt' at offset 0x4b8 contains 1 entry:
    >>   Offset          Info           Type           Sym. Value    Sym. Name + Addend
    >> 000000404018  000200000007 R_X86_64_JUMP_SLO 0000000000000000 puts@GLIBC_2.2.5 + 0
  ```
  As we can see the offset from relocation at index `0` points to `GOT[3]`.

