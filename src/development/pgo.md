# Profile guided optimization (pgo)

`pgo` is an optimization technique to optimize a program for its usual
workload.

It is applied in two phases:
1. Collect profiling data (best with representative benchmarks).
1. Optimize program based on collected profiling data.

The following simple program is used as demonstrator.
```c
#include <stdio.h>

#define NOINLINE __attribute__((noinline))

NOINLINE void foo() { puts("foo()"); }
NOINLINE void bar() { puts("bar()"); }

int main(int argc, char *argv[]) {
  if (argc == 2) {
    foo();
  } else {
    bar();
  }
}
```

## clang

On the actual machine with `clang 15.0.7`, the following code is generated for
the `main()` function.
```x86asm
# clang -o test test.c -O3

0000000000001160 <main>:
    1160:  50                   push   rax
    ; Jump if argc != 2.
    1161:  83 ff 02             cmp    edi,0x2
    1164:  75 09                jne    116f <main+0xf>
    ; foor() is on the hot path (fall-through).
    1166:  e8 d5 ff ff ff       call   1140 <_Z3foov>
    116b:  31 c0                xor    eax,eax
    116d:  59                   pop    rcx
    116e:  c3                   ret
    ; bar() is on the cold path (branch).
    116f:  e8 dc ff ff ff       call   1150 <_Z3barv>
    1174:  31 c0                xor    eax,eax
    1176:  59                   pop    rcx
    1177:  c3                   ret
```

The following shows how to compile with profiling instrumentation and how to
optimize the final program with the collected profiling data ([llvm
pgo][llvm-pgo]).

The arguments to `./test` are chosen such that `9/10` runs call `bar()`, which
is currently on the `cold path`.

```bash
# Compile test program with profiling instrumentation.
clang -o test test.cc -O3 -fprofile-instr-generate

# Collect profiling data from multiple runs.
for i in {0..10}; do
    LLVM_PROFILE_FILE="prof.clang/%p.profraw" ./test $(seq 0 $i)
done

# Merge raw profiling data into single profile data.
llvm-profdata merge -o pgo.profdata prof.clang/*.profraw

# Optimize test program with profiling data.
clang -o test test.cc -O3 -fprofile-use=pgo.profdata
```
> NOTE: If `LLVM_PROFILE_FILE` is not given the profile data is written to
> `default.profraw` which is re-written on each run. If the `LLVM_PROFILE_FILE`
> contains a `%m` in the filename, a unique integer will be generated and
> consecutive runs will update the same generated profraw file,
> `LLVM_PROFILE_FILE` can specify a new file every time, however that requires
> more storage in general.

After optimizing the program with the profiling data, the `main()` function
looks as follows.
```x86asm
0000000000001060 <main>:
    1060:  50                    push   rax
    ; Jump if argc == 2.
    1061:  83 ff 02              cmp    edi,0x2
    1064:  74 09                 je     106f <main+0xf>
    ; bar() is on the hot path (fall-through).
    1066:  e8 e5 ff ff ff        call   1050 <_Z3barv>
    106b:  31 c0                 xor    eax,eax
    106d:  59                    pop    rcx
    106e:  c3                    ret
    ; foo() is on the cold path (branch).
    106f:  e8 cc ff ff ff        call   1040 <_Z3foov>
    1074:  31 c0                 xor    eax,eax
    1076:  59                    pop    rcx
    1077:  c3                    ret
```

## gcc

With `gcc 13.2.1` on the current machine, the optimizer puts `bar()` on the
`hot path` by default.
```x86asm
0000000000001040 <main>:
    1040:  48 83 ec 08          sub    rsp,0x8
    ; Jump if argc == 2.
    1044:  83 ff 02             cmp    edi,0x2
    1047:  74 0c                je     1055 <main+0x15>
    ; bar () is on the hot path (fall-through).
    1049:  e8 22 01 00 00       call   1170 <_Z3barv>
    104e:  31 c0                xor    eax,eax
    1050:  48 83 c4 08          add    rsp,0x8
    1054:  c3                   ret
    ; foo() is on the cold path (branch).
    1055:  e8 06 01 00 00       call   1160 <_Z3foov>
    105a:  eb f2                jmp    104e <main+0xe>
    105c:  0f 1f 40 00          nop    DWORD PTR [rax+0x0]

```

The following shows how to compile with profiling instrumentation and how to
optimize the final program with the collected profiling data.

The arguments to `./test` are chosen such that `2/3` runs call `foo()`, which
is currently on the `cold path`.

```bash
gcc -o test test.cc -O3 -fprofile-generate
./test 1
./test 1
./test 2 2
gcc -o test test.cc -O3 -fprofile-use
```
> NOTE: Consecutive runs update the generated `test.gcda` profile data file
> rather than re-write it.

After optimizing the program with the profiling data, the `main()` function
```x86asm
0000000000001040 <main.cold>:
    ; bar() is on the cold path (branch).
    1040:  e8 05 00 00 00       call   104a <_Z3barv>
    1045:  e9 25 00 00 00       jmp    106f <main+0xf>

0000000000001060 <main>:
    1060:  51                   push   rcx
    ; Jump if argc != 2.
    1061:  83 ff 02             cmp    edi,0x2
    1064:  0f 85 d6 ff ff ff    jne    1040 <main.cold>
    ; for() is on the hot path (fall-through).
    106a:  e8 11 01 00 00       call   1180 <_Z3foov>
    106f:  31 c0                xor    eax,eax
    1071:  5a                   pop    rdx
    1072:  c3                   ret
```

[llvm-pgo]: https://clang.llvm.org/docs/UsersManual.html#profile-guided-optimization
