# ltrace(1)

```markdown
ltrace [opts] [prg]
  -f .......... follow child processes on fork(2)
  -p <pid> .... attach to running process
  -o <file> ... log output into <file>
  -l <filter> . show who calls into lib matched by <filter>
  -C .......... demangle
  -e <filter> . show calls symbols matched by <filter>
  -x <filter> . which symbol table entry points to trace
                (can be of form sym_pattern@lib_pattern)
  -n <num>      number of spaces to indent nested calls
```

# Example

List which program/libs call into `libstdc++`:
```bash
ltrace -l '*libstdc++*' -C -o ltrace.log ./main
```

List calls to specific symbols:
```bash
ltrace -e malloc -e free ./main
```

Trace symbols from `dlopen(3)`ed libraries.
```bash
# Assume libfoo.so would be dynamically loaded via dlopen.
ltrace -x '@libfoo.so'

# Trace all dlopened symbols.
ltrace -x '*'
# Trace all symbols from dlopened libraries which name match the
# pattern "liby*".
ltrace -x '@liby*'
# Trace symbol "foo" from all dlopened libraries matching the pattern.
ltrace -x 'foo@liby*'
```