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
