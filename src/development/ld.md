# ld(1)

```
ld [opts] files...
    -T <script>        use <script> as linker script
    --trace            report each file the linker touches
    --start-group archives --end-group
                       search archives repearepeatedly until no new
                       undefined  references are created
                       (eg helpfull with list of static libraries)
    --verbose          dump the default linker script
```

## Linker Script

Generally speaking, the linker script describes how to map a set of *input
sections* from different *input files* to a set of *output sections* in the
*output file*.

The output sections are defined as follows (full description at [output
section][ld-out] and [input section][ld-in]).

```
section_name [vaddr] : [AT(paddr)] {
    file_pattern (section_pattern)
}
```

The following gives an example of an `output` section with two `input` section rules.
```
.foo : {
    abc.o (.foo)
    *.o (.foo.*)
}
```
The first rule includes the section `.foo` from the object file `abc.o`.
The second rule is a wildcard rule, which includes all sections match the glob
`.foo.*` from all object files matching the `*.o` glob.

### Common linker script *commands* and *functions*

The `OUTPUT_FORMAT` defines the format of the output file the linker is
creates. This command takes up to three arguments and possible values can be
found by running `objdump -i`.
```
OUTPUT_FORMAT(default, little, big)
```

The `ENTRY` command takes a symbols name, which will be used as entry point.
```
ENTRY(sym)
```

## Example: virtual vs physical (load) address

Sometimes code is initially located at a different location as when being run.
For example in embedded cases, where code may initially resides in a _rom_ and
startup code will copy a section with writable _data_ into _ram_. Code accessing
the writable data accesses the data in the _ram_.

In this case we need different addresses for the same data.
- The `virtual` or _runtime_ address, this is the address used when the linker
  resolves accesses to the data. Hence, this is the address the data will have
  when the code is running.
- The `physical` or _load_ address, this is the address the data is stored at
  initially. Startup code typically copies the initial values from the
  `physical` to the `virtual` address.

The following shows an example linker script which uses _virtual_ and _physical_
addresses. The full source files can be found [here][src].

```
{{#include ld/link-nomem.ld}}
```

We can use the following assembly snippet to explore the linker script.

```x86asm
{{#include ld/data.S}}
```
> `gcc -c data.S && ld -o link-nomem -T link-nomem.ld data.o`

The elf load segments show the difference in _physical_ and _virtual_ address
for the segment containing the `.data` section.
```sh
> readelf -W -l link-nomem
# There are 4 program headers, starting at offset 64
#
# Program Headers:
#   Type   Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
#   LOAD   0x001100 0x0000000000800100 0x0000000000100000 0x000020 0x000020 RW  0x1000
#   LOAD   0x002000 0x0000000000800000 0x0000000000800000 0x000018 0x000018 R E 0x1000
#   LOAD   0x003000 0x0000000000804000 0x0000000000804000 0x000004 0x000004 R   0x1000
#   LOAD   0x000000 0x0000000000805000 0x0000000000805000 0x000000 0x001000 RW  0x1000
#
#  Section to Segment mapping:
#   Segment Sections...
#   00     .data
#   01     .text
#   02     .rodata
#   03     .stack
```

Startup code could copy data from `_data_paddr` to `_data_vaddr`.
```sh
> nm link-nomem
# 0000000000800100 d asm_array
# 0000000000804000 r asm_len
# 0000000000100000 a _data_paddr
# 0000000000800100 d _data_vaddr
# 0000000000800000 T _entry
# 0000000000806000 b _stack_top
```

The linker resolves symbols to their _virtual_ address, this can be seen by the
access to the `asm_array` variable.

```sh
> objdump -d link-nomem
# Disassembly of section .text:
#
# 0000000000800000 <_entry>:
#   800000:	48 c7 c4 00 60 80 00 	mov    $0x806000,%rsp
#   800007:	48 c7 c0 00 01 80 00 	mov    $0x800100,%rax   ;; mov $asm_array, %rax
#   80000e:	8b 04 25 00 40 80 00 	mov    0x804000,%eax
#   800015:	f4                   	hlt
#   800016:	eb e8                	jmp    800000 <_entry>
```

The following linker script shows an example with the `MEMORY` command.
```
{{#include ld/link-mem.ld}}
```

## References
- [ld manual][ld]
- [ld script: input sections][ld-in]
- [ld script: output sections][ld-out]
- [ld script: simple commands][ld-cmd]
- [ld script: builtin functions][ld-buil]
- [notes/ld example files][src]

[ld]: https://sourceware.org/binutils/docs/ld/
[ld-in]: https://sourceware.org/binutils/docs/ld/Input-Section.html
[ld-out]: https://sourceware.org/binutils/docs/ld/Output-Section-Attributes.html
[ld-cmd]: https://sourceware.org/binutils/docs/ld/Simple-Commands.html
[ld-builtin]: https://sourceware.org/binutils/docs/ld/Builtin-Functions.html
[src]: https://github.com/johannst/notes/tree/master/src/development/ld
