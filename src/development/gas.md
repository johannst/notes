# gas

## Frequently used directives
- `.byte`, `.2byte`, `.4byte`, `.8byte` to define a N byte value
  ```x86asm
  .byte 0xaa
  .2byte 0xaabb
  .2byte 0xaa, 0xbb
  .4byte 0xaabbccdd
  .8byte 0xaabbccdd11223344
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


- [GNU Assembler][gas_doc]
- [GNU Assembler Directives][gas_directives]
- [GNU Assembler `x86_64` dependent features][gas_x86_64]

[gas_doc]: https://sourceware.org/binutils/docs/as
[gas_directives]: https://sourceware.org/binutils/docs/as/Pseudo-Ops.html#Pseudo-Ops
[gas_x86_64]: https://sourceware.org/binutils/docs/as/i386_002dDependent.html
