# objdump(1)

```markdown
  objdump [opts] <elf>
    -M intel                   use intil syntax
    -d                         disassemble text section
    -D                         disassemble all sections
    --disassemble=<sym>        disassemble symbol <sym>
    -S                         mix disassembly with source code
    -C                         demangle
    -j <section>               display info for section
    --[no-]show-raw-insn       [dont] show object code next to disassembly
    --visualize-jumps[=color]  visualize jumps with ascii art, optionally color arrows
```

## Disassemble section
For example `.plt` section:
```markdown
  objdump -j .plt -d <elf>
```

## Example: disassemble raw binary
This can be helpful for example as a cheap analysis tool when toying with JIT
generating code. We could just write thee binary code buffer to a file and
disassemble with `objdump`.

To re-create that case, we just assemble and link some ELF file and then create
a raw binary of the text section with `objcopy`.

```x86asm
# file: test.s
.section .text, "ax"

.global _start
_start:
    xor %rax, %rax
    mov $0x8, %rax
1:
    cmp $0, %rax
    je 2f
    dec %rax
    jmp 1b
2:
    # x86-64 exit(2) syscall
    mov $0, %rdi
    mov $60, %rax
    syscall
```
```bash
# Assemble & link.
as -o test.o test.s
ld -o test test.o testc.o
# ELF -> binary (only take .text section).
objcopy -O binary --only-section .text test test-bin

# Disassemble raw binary.
objdump -D -b binary -m i386:x86-64 test-bin
```

## Example: disassemble specific symbol
```bash
# Disassemble main().
objdump --disassemble=main <bin>
# Disassemble 'foo::bar()' (mangled).
objdump --disassemble=_ZN3foo3barEvr <bin>

# Disassemble 'foo::bar()' (demangled), requires -C
objdump -C --disassemble=foo::bar <bin>
```
