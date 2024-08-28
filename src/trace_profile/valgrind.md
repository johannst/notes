# valgrind(1)

## Memcheck `--tool=memcheck`

Is the default tool when invoking `valgrind` without explicitly specifying
`--tool`.

Memory checker used to identify:
- memory leaks
- out of bound accesses
- uninitialized reads

```sh
valgrind [OPTIONS] PROGRAM [ARGS]
    --log-file=FILE             Write valgrind output to FILE.
    --leak-check=full           Enable full leak check.
    --track-origins=yes         Show origins of undefined values.
    --keep-debuginfo=no|yes     Keep symbols etc for unloaded code.

    --gen-suppressions=yes      Generate suppressions file from the run.
    --suppressions=FILE         Load suppressions file.
```
