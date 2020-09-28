# arm64
keywords: arm64, aarch64, abi

- 64bit synonyms: `arm64`, `aarch64`
- ISA type: `RISC`
- Endianness: `little`, `big`


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
    .align 2
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

    .align 3
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
> Cross-compiling on `Ubuntu 20.04`, paths might differ on other distributions.
> Compiling natively on arm64, specifying the dynamic linker should not be required.

## References
- [Procedure Call Standard ARM64][aapcs64]
- [ARMv8 Programmer's Guide][armv8a_prog_guide]
- [GNU Assembler][gas_doc]
- [GNU Assembler Directives][gas_directives]


[aapcs64]: https://github.com/ARM-software/abi-aa/blob/master/aapcs64/aapcs64.rst
[armv8a_prog_guide]: https://developer.arm.com/documentation/den0024/a
[gas_doc]: https://sourceware.org/binutils/docs/as
[gas_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html#Pseudo-Ops
