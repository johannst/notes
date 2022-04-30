# riscv
keywords: rv32, rv64

- ISA type: `RISC`
- Endianness: `little`, `big`

## Registers
- riscv32 => `XLEN=32`
- riscv64 => `XLEN=64`

### General purpose registers
```markdown
[XLEN-1:0]     abi name     desc
---------------------------------------------
x0             zero         zero register
x1             ra           return addr
x2             sp           stack ptr
x3             gp           global ptr
x4             tp           thread ptr
x5-x7          t0-t2        temp regs
x8-x9          s0-s1        saved regs
x10-x17        a0-a7        arg regs
x18-x27        s2-s11       saved regs
x28-x31        t3-t6        temp regs
```

## ASM skeleton
Small assembler skeleton, ready to use with following properties:
- use raw Linux syscalls (`man 2 syscall` for ABI)
- no `C runtime (crt)`
- gnu assembler [`gas`][gas_doc]
```mipsasm
// file: greet.S

#include <asm/unistd.h>     // syscall NRs

    .section .text, "ax", @progbits
    .balign 4               // align code on 4byte boundary
    .global _start
_start:
    li a0, 2                // fd
    la a1, greeting         // buf
    ld a2, (greeting_len)   // &len
    li a7, __NR_write       // write(2) syscall
    ecall

    li a0, 42               // exit code
    li a7, __NR_exit        // exit(2) syscall
    ecall

    .balign 8               // align data on 8byte boundary
    .section .rodata, "a", @progbits
greeting:
    .asciz "Hi ASM-World!\n"
greeting_len:
    .int .-greeting
```
> man gcc: `file.S` assembler code that must be preprocessed.

To cross-compile and run:
```bash
> riscv64-linux-gnu-gcc -o greet greet.S -nostartfiles -nostdlib                \
    -Wl,--dynamic-linker=/usr/riscv64-linux-gnu/lib/ld-linux-riscv64-lp64d.so.1 \
  && qemu-riscv64 ./greet
Hi ASM-World!
```
> Cross-compiling on `Ubuntu 20.04 (x86_64)`, paths might differ on other
> distributions. Explicitly specifying the dynamic linker should not be
> required when compiling natively on riscv.
>
> Select dynamic linker according to abi used during compile & link.

## References
- [GNU Assembler][gas_doc]
- [GNU Assembler Directives][gas_directives]

[gas_doc]: https://sourceware.org/binutils/docs/as
[gas_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html#Pseudo-Ops
