# gcc(1)

## CLI

- `-v` verbose, outputs exact compiler/linker invocations made by the gcc driver
- `-###` dry-run, outputting exact compiler/linker invocations
- `-print-multi-lib` print available multilib configurations
- `--help=<class>` print description of cmdline options for given class, eg
  `warnings`, `optimizers`, `target`, `c`, `c++`
- `-Wl,<opt>` additional option passed to the linker invocation (can
  be specified multiple times)
- `-Wl,--trace` trace each file the linker touches

### Environment variables
```
# Colon separated list of additional include paths used during preprocessing.
# This has the same effect as adding -isystem on the cmdline, however cmdline
# options take precedence over the environment variables.
C_INCLUDE_PATH
CPLUS_INCLUDE_PATH

# Colon separated list of additional library search paths, used when linking
# with gcc. This has the same effect as adding -L to the cmdline, however cmdline
# options take precedence over the environment variables.
LIBRARY_PATH
```

### Preprocessing
While debugging can be helpful to just pre-process files.

```bash
gcc -E [-dM] ...
```
- `-E` run only preprocessor
- `-dM` list only `#define` statements

### Target options
```bash
# List all target options with their description.
gcc --help=target

# Configure for current cpu arch and query (-Q) value of options.
gcc -march=native -Q --help=target
```

### Warnings / optimizations
```bash
# List available warnings with short description.
gcc --help=warnings
# List available optimizations with short description.
gcc --help=optimizers

# Prepend --help with `-Q` to print wheter options are enabled or disabled
# instead showing their description.
```

### Sanitizer
```bash
# Enable address sanitizer, a memory error checker (out of bounds, use after free, ..).
gcc -fsanitize=address ...

# Enable leak sanitizer, a memory leak detector.
gcc -fsanitize=leak

# Enable undefined behavior sanitizer, detects various UBs (integer overflow, ..).
gcc -fsanitize=undefined ...

# Enable thread sanitizer, a data race detector.
gcc -fsanitize=thread
```

## [Builtins][builtins]

### `__builtin_expect(expr, cond)`
Give the compiler a hint which branch is hot, so it can lay out the code
accordingly to reduce number of jump instructions.
See on [compiler explorer](https://godbolt.org/z/MbTHAP).

The semantics of this hint are as follows, the compiler prioritises `expr ==
cond`. So `__builtin_expect(expr, 0)` means that we expect the `expr` to be `0`
most of the time.

```bash
echo "
extern void foo();
extern void bar();
void run0(int x) {
  if (__builtin_expect(x,0)) { foo(); }
  else { bar(); }
}
void run1(int x) {
  if (__builtin_expect(x,1)) { foo(); }
  else { bar(); }
}
" | gcc -O2 -S -masm=intel -o /dev/stdout -xc -
```

Will generate something similar to the following.
- `run0`: `bar` is on the path without branch
- `run1`: `foo` is on the path without branch
```x86asm
run0:
        test    edi, edi
        jne     .L4
        xor     eax, eax
        jmp     bar
.L4:
        xor     eax, eax
        jmp     foo
run1:
        test    edi, edi
        je      .L6
        xor     eax, eax
        jmp     foo
.L6:
        xor     eax, eax
        jmp     bar
```

## ABI (Linux)

- C ABI (x86_64) - [SystemV ABI][sysvabi]
- C++ ABI - [C++ Itanium ABI][cppabi]

[builtins]: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
[sysvabi]: https://gitlab.com/x86-psABIs/x86-64-ABI
[cppabi]: https://itanium-cxx-abi.github.io/cxx-abi
