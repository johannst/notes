# ELF Symbol Versioning

The [ELF symbol versioning][lsb-symver] mechanism allows to attach version
information to symbols.
This can be used to express symbol version requirements or to provide certain
symbols multiple times in the same ELF file with different versions (eg for
backwards compatibility).

The `libpthread.so` library is an example which provides the
`pthread_cond_wait` symbol multiple times but in different versions.
With readelf the version of the symbol can be seen after the `@`.
```bash
> readelf -W --dyn-syms /lib/libpthread.so

Symbol table '.dynsym' contains 342 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
   ...
   141: 0000f080   696 FUNC    GLOBAL DEFAULT   16 pthread_cond_wait@@GLIBC_2.3.2
   142: 00010000   111 FUNC    GLOBAL DEFAULT   16 pthread_cond_wait@GLIBC_2.2.5
```

The `@@` denotes the **default symbol version** which will be used during
static linking against the library.
The following dump shows that the `tmp` program linked against `lpthread` will
depend on the symbol version `GLIBC_2.3.2`, which is the default version.
```bash
> echo "#include <pthread.h>
        int main() {
          return pthread_cond_wait(0,0);
        }" | gcc -o tmp -xc - -lpthread;
  readelf -W --dyn-syms tmp | grep pthread_cond_wait;

Symbol table '.dynsym' contains 7 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
   ...
     2: 00000000     0 FUNC    GLOBAL DEFAULT  UND pthread_cond_wait@GLIBC_2.3.2 (2)
```
> Only **one** symbol can be annotated as the `@@` default version.

Using the `--version-info` flag with readelf, more details on the symbol
version info compiled into the `tmp` ELF file can be obtained.
- The `.gnu.version` section contains the version definition for each symbol in
  the `.dynsym` section. `pthread_cond_wait` is at index `2` in the `.dynsym`
  section, the corresponding symbol version is at index `2` in the
  `.gnu.version` section.
- The `.gnu.version_r` section contains symbol version requirements per shared
  library dependency (`DT_NEEDED` dynamic entry).
```bash
> readelf -W --version-info --dyn-syms tmp

Symbol table '.dynsym' contains 7 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTable
     2: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND pthread_cond_wait@GLIBC_2.3.2 (2)
     3: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@GLIBC_2.2.5 (3)
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     5: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
     6: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.2.5 (3)

Version symbols section '.gnu.version' contains 7 entries:
 Addr: 0x0000000000000534  Offset: 0x000534  Link: 6 (.dynsym)
  000:   0 (*local*)       0 (*local*)       2 (GLIBC_2.3.2)   3 (GLIBC_2.2.5)
  004:   0 (*local*)       0 (*local*)       3 (GLIBC_2.2.5)

Version needs section '.gnu.version_r' contains 2 entries:
 Addr: 0x0000000000000548  Offset: 0x000548  Link: 7 (.dynstr)
  000000: Version: 1  File: libc.so.6  Cnt: 1
  0x0010:   Name: GLIBC_2.2.5  Flags: none  Version: 3
  0x0020: Version: 1  File: libpthread.so.0  Cnt: 1
  0x0030:   Name: GLIBC_2.3.2  Flags: none  Version: 2
```

The gnu dynamic linker allows to inspect the version processing during runtime
by setting the `LD_DEBUG` environment variable accordingly.
```text
# version: Display version dependencies.
> LD_DEBUG=versions ./tmp
    717904: checking for version `GLIBC_2.2.5' in file /usr/lib/libc.so.6 [0] required by file ./tmp [0]
    717904: checking for version `GLIBC_2.3.2' in file /usr/lib/libpthread.so.0 [0] required by file ./tmp [0]
    ...

#  symbols : Display symbol table processing.
#  bindings: Display information about symbol binding.
> LD_DEBUG=symbols,bindings ./tmp
    ...
    718123: symbol=pthread_cond_wait;  lookup in file=./tmp [0]
    718123: symbol=pthread_cond_wait;  lookup in file=/usr/lib/libpthread.so.0 [0]
    718123: binding file ./tmp [0] to /usr/lib/libpthread.so.0 [0]: normal symbol `pthread_cond_wait' [GLIBC_2.3.2]
```

## Example: version script

The following shows an example C++ library `libfoo` which provides the same
symbol multiple times but in different versions.
```cpp
// file: libfoo.cc
#include<stdio.h>

// Bind function symbols to version nodes.
//
// ..@       -> Is the unversioned symbol.
// ..@@..    -> Is the default symbol.

__asm__(".symver func_v0,func@");
__asm__(".symver func_v1,func@LIB_V1");
__asm__(".symver func_v2,func@@LIB_V2");

extern "C" {
    void func_v0() { puts("func_v0"); }
    void func_v1() { puts("func_v1"); }
    void func_v2() { puts("func_v2"); }
}

__asm__(".symver _Z11func_cpp_v1i,_Z8func_cppi@LIB_V1");
__asm__(".symver _Z11func_cpp_v2i,_Z8func_cppi@@LIB_V2");

void func_cpp_v1(int) { puts("func_cpp_v1"); }
void func_cpp_v2(int) { puts("func_cpp_v2"); }

void func_cpp(int) { puts("func_cpp_v2"); }
```

Version script for `libfoo` which defines which symbols for which versions are
exported from the ELF file.
```ld
# file: libfoo.ver
LIB_V1 {
    global:
        func;
        extern "C++" {
            "func_cpp(int)";
        };
    local:
        *;
};

LIB_V2 {
    global:
        func;
        extern "C++" {
            "func_cpp(int)";
        };
} LIB_V1;
```
> The **local:** section in `LIB_V1` is a catch all, that matches any symbol
> not explicitly specified, and defines that the symbol is local and therefore
> not exported from the ELF file.

The library `libfoo` can be linked with the version definitions in `libfoo.ver`
by passing the version script to the linker with the `--version-script` flag.
```bash
> g++ -shared -fPIC -o libfoo.so libfoo.cc -Wl,--version-script=libfoo.ver
> readelf -W --dyn-syms libfoo.so | c++filt

Symbol table '.dynsym' contains 14 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
   ...
     6: 0000000000000000     0 OBJECT  GLOBAL DEFAULT  ABS LIB_V1
     7: 000000000000114b    29 FUNC    GLOBAL DEFAULT   13 func_cpp(int)@LIB_V1
     8: 0000000000001168    29 FUNC    GLOBAL DEFAULT   13 func_cpp(int)@@LIB_V2
     9: 0000000000001185    29 FUNC    GLOBAL DEFAULT   13 func_cpp(int)@@LIB_V1
    10: 0000000000000000     0 OBJECT  GLOBAL DEFAULT  ABS LIB_V2
    11: 0000000000001109    22 FUNC    GLOBAL DEFAULT   13 func
    12: 000000000000111f    22 FUNC    GLOBAL DEFAULT   13 func@LIB_V1
    13: 0000000000001135    22 FUNC    GLOBAL DEFAULT   13 func@@LIB_V2
```

The following program demonstrates how to make use of the different versions:
```cpp
// file: main.cc
#include <dlfcn.h>
#include <assert.h>

// Links against default symbol in the lib.so.
extern "C" void func();

int main() {
    // Call the default version.
    func();

#ifdef _GNU_SOURCE
    typedef void (*fnptr)();

    // Unversioned lookup.
    fnptr fn_v0 = (fnptr)dlsym(RTLD_DEFAULT, "func");
    // Version lookup.
    fnptr fn_v1 = (fnptr)dlvsym(RTLD_DEFAULT, "func", "LIB_V1");
    fnptr fn_v2 = (fnptr)dlvsym(RTLD_DEFAULT, "func", "LIB_V2");

    assert(fn_v0 != 0);
    assert(fn_v1 != 0);
    assert(fn_v2 != 0);

    fn_v0();
    fn_v1();
    fn_v2();
#endif

    return 0;
}
```

Compiling and running results in:
```bash
> g++ -o main main.cc -ldl ./libfoo.so && ./main
func_v2
func_v0
func_v1
func_v2
```

## References
- [ELF Symbol Versioning][elfsymver]
- [Binutils ld: Symbol Versioning][ld-version]
- [LSB: Symbol Versioning][lsb-symver]
- [How To Write Shared Libraries][dsohowto]

[dsohowto]: https://akkadia.org/drepper/dsohowto.pdf
[elfsymver]: https://akkadia.org/drepper/symbol-versioning

[ld-version]: https://sourceware.org/binutils/docs/ld/VERSION.html
[lsb-symver]: https://refspecs.linuxbase.org/LSB_5.0.0/LSB-Core-generic/LSB-Core-generic/symversion.html
