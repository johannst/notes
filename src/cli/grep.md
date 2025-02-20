# grep(1)

```
sort [opts] [pattern] [files]
    -e <pattern>        pattern to search for (can be supplied multiple times)
    -i                  ignore case in patterns
    -v                  invert match

    -n                  add line numbers to matched lines
    -H                  add file name to matched lines

    -r                  recursively read all files
    -I                  skip binary files
    --include <glob>    search only files matching glob
    --exclude <glob>    skip searching files matching glob

    -c                  count occurrence of matched patterns
    -l                  list only file name which contain the pattern
```
> `<glob>` patterns may need to be quoted or escaped if the shell also does glob expansion.
