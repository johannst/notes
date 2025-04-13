# gas

## Frequently used directives
- `.section` to define a section (elf files)
  ```x86asm
  .section .text.foo, "ax", @progbits
  ; defines section named .text.foo with alloc+exec perms

  .section .data.foo, "aw", @progbits
  ; defines section named .data.foo with alloc+write perms

  .section .rodata.foo, "a", @progbits
  ; defines section named .rodata.foo with alloc perms
  ```
- `.byte`, `.2byte`, `.4byte`, `.8byte` to define a N byte value
  ```x86asm
  .byte 0xaa
  .2byte 0xaabb                 # .word
  .2byte 0xaa, 0xbb
  .4byte 0xaabbccdd             # .long
  .8byte 0xaabbccdd11223344     # .quad
  ```
- `.ascii` to define an ascii string
  ```x86asm
  .ascii "foo"   ; allocates 3 bytes
  ```
- `.asciz` to define an ascii string with `'\0'` terminator
  ```x86asm
  .asciz "foo"   ; allocates 4 bytes (str + \0)
  ```
- `.macro` to define assembler macros. Arguments are accessed with the
  `\arg` syntax.
  ```x86asm
  .macro defstr name str
  \name:
      .ascii "\str"
  \name\()_len:
      .8byte . - \name
  .endm

  ; use as
  defstr foo, "foobar"
  ```
  > Use `\()` to concatenate macro argument and literal.
- `.rept` to repeat a sequence of lines between `.rept` and `.endr`.
  ```x86asm
  .rept 4
  .4byte 123
  .endr
  ```
- `.fill cnt, elem_size, val` write `cnt` times `val` with element size `elem_size`. For example one can use it to create a mbr boot record (magic number 0xaa55 at byte 511, 512).
  ```x86asm
  .section .boot, "ax", @progbits
  ; some code ..
  .4byte 0xff

  .fill 510 - (. - .boot), 1, 0x00
  .2byte 0xaa55

  ; as foo.s && objdump -j .boot -s
  ; Contents of section .boot:
  ; 0000 ff000000 00000000 00000000 00000000
  ; ..
  ; 01f0 00000000 00000000 00000000 000055aa
  ```
  > Here `.` stands for the current location counter.

## References
- [GNU Assembler][gas_doc]
- [GNU Assembler Directives][gas_directives]
- [GNU Assembler `x86_64` dependent features][gas_x86_64]

[gas_doc]: https://sourceware.org/binutils/docs/as
[gas_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html#Pseudo-Ops
[gas_x86_64]: https://sourceware.org/binutils/docs/as/i386_002dDependent.html
