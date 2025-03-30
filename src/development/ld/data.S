.section .text, "ax", @progbits
.global _entry
_entry:
    mov $_stack_top, %rsp
    mov $asm_array, %rax
    mov (asm_len), %eax

    hlt
    jmp _entry

.section .data.asm, "aw", @progbits
asm_array:
    .4byte 0xa
    .4byte 0xb
    .4byte 0xc
    .4byte 0xd
.rept 4
    .4byte 0xff
.endr

.section .rodata.asm, "a", @progbits
asm_len:
    .4byte 8
