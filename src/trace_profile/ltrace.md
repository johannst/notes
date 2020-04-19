# ltrace(1)

```markdown
ltrace [opts] [prg]
  -f .......... follow child processes on fork(2)
  -p <pid> .... attach to running process
  -o <file> ... log output into <file>
  -l <filter> . show who calls into lib matched by <filter>
  -C .......... demangle
```

# Example

List which program/libs call into `libstdc++`:
```bash
ltrace -l '*libstdc++*' -C -o ltrace.log ./main
```
