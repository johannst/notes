# arm64
keywords: arm64, aarch64, abi

- 64bit synonyms: `arm64`, `aarch64`
- ISA type: `RISC`
- Endianness: `little`, `big`

## Registers
### General purpose registers
```markdown
bytes
[7:0]     [3:0]     desc
---------------------------------------------
x0-x28    w0-w28    general purpose registers
x29       w29       frame pointer (FP)
x30       w30       link register (LR)
sp        wsp       stack pointer (SP)
pc                  program counter (PC)
xzr       wzr       zero register
```
> Write to `wN` register clears upper 32bit.

### Special registers per EL
```markdown
bytes
[7:0]       desc
---------------------------------------------
sp_el0      stack pointer EL0

sp_el1      stack pointer EL1
elr_el1     exception link register EL1
spsr_el1    saved process status register EL1

sp_el2      stack pointer EL2
elr_el2     exception link register EL2
spsr_el2    saved process status register EL2

sp_el3      stack pointer EL3
elr_el3     exception link register EL3
spsr_el3    saved process status register EL3
```

## Instructions cheatsheet

### Accessing system registers
Reading from system registers:
```armasm
mrs x0, vbar_el1      // move vbar_el1 into x0
```

Writing to system registers:
```armasm
msr vbar_el1, x0      // move x0 into vbar_el1
```

### Control Flow
```armasm
b <offset>    // relative forward/back branch
br <Xn>       // absolute branch to address in register Xn

// branch & link, store return address in X30 (LR)
bl <offset>   // relative forward/back branch
blr <Xn>      // absolute branch to address in register Xn

ret {Xn}      // return to address in X30, or Xn if supplied
```

## Addressing
### Offset
```armasm
ldr x0, [x1]                // x0 = [x1]
ldr x0, [x1, 8]             // x0 = [x1 + 8]
ldr x0, [x1, x2, lsl #3]    // x0 = [x1 + (x2<<3)]
ldr x0, [x1, w2, stxw]      // x0 = [x1 + sign_ext(w2)]
ldr x0, [x1, w2, stxw #3]   // x0 = [x1 + (sign_ext(w2)<<3)]
```
> Shift amount can either be `0` or `log2(access_size_bytes)`. Eg for 8byte
> access it can either be `{0, 3}`.

### Index
```armasm
ldr x0, [x1, 8]!    // pre-inc : x1+=8; x0 = [x1]
ldr x0, [x1], 8     // post-inc: x0 = [x1]; x1+=8
```

### Pair access
```armasm
ldp x1, x2, [x0]    // x1 = [x0]; x2 = [x0 + 8]
stp x1, x2, [x0]    // [x0] = x1; [x0 + 8] = x2
```

## Procedure Call Standard ARM64 ([`aapcs64`][aapcs64])

### Passing arguments to functions
- Integer/Pointer arguments
  ```markdown
  reg     arg
  -----------
  x0        1
  ..       ..
  x7        8
  ```
- Additional arguments are passed on the stack. Arguments are pushed
  `right-to-left (RTL)`, meaning next arguments are closer to current `sp`.
  ```markdown
  void take(..., int a9, int a10);
                     |       |   | ... |       Hi
                     |       +-->| a10 |       |
                     +---------->| a9  | <-SP  |
                                 +-----+       v
                                 | ... |       Lo
  ```

### Return values from functions
- Integer/Pointer return values
  ```markdown
  reg          size
  -----------------
  x0         64 bit
  ```

### Callee saved registers
- `x19` - `x28`
- `SP`

### Stack
- grows downwards
- `sp` must be 16byte aligned when used to access memory for r/w
- `sp` must be 16byte aligned on public interface interfaces

### Frame chain
- linked list of stack-frames
- each frame links to the frame of its caller by a `frame record`
  - a frame record is described as a `(FP,LR)` pair
- `x29 (FP)` must point to the frame record of the current stack-frame
  ```markdown
        +------+                   Hi
        |   0  |     frame0        |
     +->|   0  |                   |
     |  |  ... |                   |
     |  +------+                   |
     |  |  LR  |     frame1        |
     +--|  FP  |<-+                |
        | ...  |  |                |
        +------+  |                |
        |  LR  |  |  current       |
  x29 ->|  FP  |--+  frame         v
        | ...  |                   Lo
  ```
- end of the frame chain is indicated by following frame record `(0,-)`
- location of the frame record in the stack frame is not specified

### Function prologue & epilogue
- prologue
  ```armasm
  sub sp, sp, 16
  stp x29, x30, [sp]      // [sp] = x29; [sp + 8] = x30
  mov x29, sp             // FP points to frame record
  ```
- epilogue
  ```armasm
  ldp x29, x30, [sp]      // x29 = [sp]; x30 = [sp + 8]
  add sp, sp, 16
  ret
  ```

## ASM skeleton
Small assembler skeleton, ready to use with following properties:
- use raw Linux syscalls (`man 2 syscall` for ABI)
- no `C runtime (crt)`
- gnu assembler [`gas`][gas_doc]
```armasm
// file: greet.S

#include <asm/unistd.h>      // syscall NRs

    .arch armv8-a

    .section .text, "ax", @progbits
    .balign 4                // align code on 4byte boundary
    .global _start
_start:
    mov x0, 2                // fd
    ldr x1, =greeting        // buf
    ldr x2, =greeting_len    // &len
    ldr x2, [x2]             // len
    mov w8, __NR_write       // write(2) syscall
    svc 0

    mov x0, 0                // exit code
    mov w8, __NR_exit        // exit(2) syscall
    svc 0

    .balign 8                // align data on 8byte boundary
    .section .rodata, "a", @progbits
greeting:
    .asciz "Hi ASM-World!\n"
greeting_len:
    .int .-greeting
```
> man gcc: `file.S` assembler code that must be preprocessed.

To cross-compile and run:
```bash
> aarch64-linux-gnu-g++ -o greet greet.S -nostartfiles -nostdlib          \
    -Wl,--dynamic-linker=/usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 \
  && qemu-aarch64 ./greet
Hi ASM-World!
```
> Cross-compiling on `Ubuntu 20.04 (x86_64)`, paths might differ on other
> distributions. Explicitly specifying the dynamic linker should not be
> required when compiling natively on arm64.

## References
- [Procedure Call Standard ARM64][aapcs64]
- [ARMv8-A Programmer's Guide][armv8a_prog_guide]
- [ARMv8-A Architecture Reference Manual][armv8a_arm]
- [AppNote: ARMv8 Bare-metal boot code][armv8_baremetal_boot]
- [GNU Assembler][gas_doc]
- [GNU Assembler Directives][gas_directives]


[aapcs64]: https://github.com/ARM-software/abi-aa/blob/master/aapcs64/aapcs64.rst
[armv8a_prog_guide]: https://developer.arm.com/documentation/den0024/a
[armv8a_arm]: https://developer.arm.com/documentation/ddi0487/latest
[armv8_baremetal_boot]: https://developer.arm.com/documentation/dai0527/a
[gas_doc]: https://sourceware.org/binutils/docs/as
[gas_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html#Pseudo-Ops
