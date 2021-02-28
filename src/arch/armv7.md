# armv7a
keywords: arm, armv7, abi

- ISA type: `RISC`
- Endianness: `little`, `big`

## Registers
### General purpose registers
```markdown
bytes
[3:0]     alt     desc
---------------------------------------------
r0-r12            general purpose registers
r11       fp
r13       sp      stack pointer
r14       lr      link register
r15       pc      program counter
```

### Special registers
```markdown
bytes
[3:0]             desc
---------------------------------------------
cpsr              current program status register
```

### CPSR register
```markdown
cpsr
bits  desc
-----------------------------
 [31]  N negative flag
 [30]  Z zero flag
 [29]  C carry flag
 [28]  V overflow flag
 [27]  Q cummulative saturation (sticky)
  [9]  E load/store endianness
  [8]  A disable asynchronous aborts
  [7]  I disable IRQ
  [6]  F disable FIQ
  [5]  T indicate Thumb state
[4:0]  M process mode (USR, FIQ, IRQ, SVC, ABT, UND, SYS)
```

## Instructions cheatsheet
### Accessing system registers
Reading from system registers:
```armasm
mrs r0, cpsr      // move cpsr into r0
```

Writing to system registers:
```armasm
msr cpsr, r0      // move r0 into cpsr
```

### Control Flow
```armasm
b <lable>     // relative forward/back branch
bl <lable>    // relative forward/back branch & link return addr in r14 (LR)

// branch & exchange (can change between ARM & Thumb instruction set)
//   bit Rm[0] == 0 -> ARM
//   bit Rm[0] == 1 -> Thumb
bx <Rm>       // absolute branch to address in register Rm
blx <Rm>      // absolute branch to address in register Rm &
              // link return addr in r14 (LR)
```

### Load/Store
Different addressing modes.
```armasm
str r1, [r0]         // [r0]=r1
str r1, [r0, #4]     // [r0+4]=r1
str r1, [r0, #4]!    // r0+=4; [r0]=r1
str r1, [r0], 4      // [r0]=r1; r0+=4
```

Load/store multiple registers full-descending.
```armasm
stmfd r0!, {r1-r2, r5}    // r0-=4; [r0]=r5
                          // r0-=4; [r0]=r2
                          // r0-=4; [r0]=r1
ldmfd r0!, {r1-r2, r5}    // r1=[r0]; r0+=4
                          // r2=[r0]; r0+=4
                          // r5=[r0]; r0+=4
```
> `!` is optional but has the effect to update the base pointer register `r0` here.

Push/Pop
```armasm
push {r0-r2}    // effectively stmfd sp!, {r0-r2}
pop {r0-r2}     // effectively ldmfd sp!, {r0-r2}
```

## Procedure Call Standard ARM ([`aapcs32`][aapcs32])
### Passing arguments to functions
- integer/pointer arguments
  ```markdown
  reg     arg
  -----------
  r0        1
  ..       ..
  r3        4
  ```
- a double word (64bit) is passed in two consecutive registers (eg `r1+r2`)
- additional arguments are passed on the stack. Arguments are pushed
  `right-to-left (RTL)`, meaning next arguments are closer to current `sp`.
  ```markdown
  void take(..., int a5, int a6);
                     |       |   | ... |       Hi
                     |       +-->| a6  |       |
                     +---------->| a5  | <-SP  |
                                 +-----+       v
                                 | ... |       Lo
  ```

### Return values from functions
- integer/pointer return values
  ```markdown
  reg          size
  -----------------
  r0         32 bit
  r0+r1      64 bit
  ```

### Callee saved registers
- `r4` - `r11`
- `sp`

### Stack
- full descending
  - full: `sp` points to the last used location (valid item)
  - descending: stack grows downwards
- `sp` must be 4byte aligned (word boundary) at all time
- `sp` must be 8byte aligned on public interface interfaces

### Frame chain
- not strictly required by each platform
- linked list of stack-frames
- each frame links to the frame of its caller by a `frame record`
  - a frame record is described as a `(FP,LR)` pair (2x32bit)
- `r11 (FP)` must point to the frame record of the current stack-frame
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
  r11 ->|  FP  |--+  frame         v
        | ...  |                   Lo
  ```
- end of the frame chain is indicated by following frame record `(0,-)`
- location of the frame record in the stack frame is not specified
- `r11` is not updated before the new frame record is fully constructed

### Function prologue & epilogue
- prologue
  ```armasm
  push {fp, lr}
  mov fp, sp              // FP points to frame record
  ```
- epilogue
  ```armasm
  pop {fp, pc}            // pop LR directly into PC
  ```

## ASM skeleton
Small assembler skeleton, ready to use with following properties:
- use raw Linux syscalls (`man 2 syscall` for ABI)
- no `C runtime (crt)`
- gnu assembler [`gas`][gas_doc]
```armasm
// file: greet.S

#include <asm/unistd.h>      // syscall NRs

    .arch armv7-a

    .section .text, "ax"
    .balign 4

    // Emit `arm` instructions, same as `.arm` directive.
    .code 32
    .global _start
_start:
    // Branch with link and exchange instruction set.
    blx _do_greet

    mov r0, #0               // exit code
    mov r7, #__NR_exit       // exit(2) syscall
    swi 0x0

    // Emit `thumb` instructions, same as `.thumb` directive.
    .code 16
    .thumb_func
_do_greet:
    mov r0, #2               // fd
    ldr r1, =greeting        // buf
    ldr r2, =greeting_len    // &len
    ldr r2, [r2]             // len
    mov r7, #__NR_write      // write(2) syscall
    swi 0x0

    // Branch and exchange instruction set.
    bx lr

    .balign 8                // align data on 8byte boundary
    .section .rodata, "a"
greeting:
    .asciz "Hi ASM-World!\n"
greeting_len:
    .int .-greeting
```
> man gcc: `file.S` assembler code that must be preprocessed.

To cross-compile and run:
```bash
> arm-linux-gnueabi-gcc -o greet greet.S -nostartfiles -nostdlib  \
    -Wl,--dynamic-linker=/usr/arm-linux-gnueabi/lib/ld-linux.so.3 \
  && qemu-arm ./greet
Hi ASM-World!
```
> Cross-compiling on `Ubuntu 20.04 (x86_64)`, paths might differ on other
> distributions. Explicitly specifying the dynamic linker should not be
> required when compiling natively on arm.

## References
- [Procedure Call Standard ARM][aapcs32]
- [ARMv7-A Programmer's Guide][armv7a_prog_guide]
- [ARMv7-A Architecture Reference Manual][armv7a_arm]
- [GNU Assembler][gas_doc]
- [GNU Assembler Directives][gas_directives]
- [GNU Assembler `ARM` dependent features][gas_arm]


[aapcs32]: https://github.com/ARM-software/abi-aa/blob/master/aapcs32/aapcs32.rst
[armv7a_prog_guide]: https://developer.arm.com/documentation/den0013/latest
[armv7a_arm]: https://developer.arm.com/documentation/ddi0406/latest
[gas_doc]: https://sourceware.org/binutils/docs/as
[gas_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html#Pseudo-Ops
[gas_arm]: https://sourceware.org/binutils/docs/as/ARM_002dDependent.html
