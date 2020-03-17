# od(1)

```markdown
  od [opts] <file>
    -An         don't print addr info
    -tx4        print hex in 4 byte chunks
    -ta         print as named character
    -tc         printable chars or backslash escape
    -w4         print 4 bytes per line
    -j <n>      skip <n> bytes from <file> (hex if start with 0x)
    -N <n>      dump <n> bytes (hex of start with 0x)
```

## ASCII to hex string
```markdown
  echo -n AAAABBBB | od -An -w4 -tx4
    >> 41414141
    >> 42424242

  echo -n '\x7fELF\n' | od -tx1 -ta -tc
    >> 0000000  7f  45  4c  46  0a      # tx1
    >>         del   E   L   F  nl      # ta
    >>         177   E   L   F  \n      # tc
```

## Extract parts of file
For example `.rodata` section from an elf file. We can use `readelf` to get the
offset into the file where the `.rodata` section starts.
```markdown
  readelf -W -S foo
    >> Section Headers:
    >> [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
    >> ...
    >> [15] .rodata           PROGBITS        00000000004009c0 0009c0 000030 00   A  0   0 16
```

With the offset of `-j 0x0009c0` we can dump `-N 0x30` bytes from the beginning of
the `.rodata` section as follows:
```markdown
  od -j 0x0009c0 -N 0x30 -tx4 -w4 foo
    >> 0004700 00020001
    >> 0004704 00000000
    >> *
    >> 0004740 00000001
    >> 0004744 00000002
    >> 0004750 00000003
    >> 0004754 00000004
```
**Note**: Numbers starting with `0x` will be interpreted as hex by `od`.
