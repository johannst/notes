# sed(1)

```
sed [opts] [script] [file]
  opts:
    -i          edit file in place
    -i.bk       edit file in place and create backup file
                (with .bk suffix, can be specified differently)
    -e SCRIPT   add SCRIPT to commands to be executed
                (can be specified multiple times)
    -f FILE     add content of FILE to command to be executed

    --debug     annotate program execution
```

## Examples
### Delete lines
```sh
# Delete two lines.
echo -e 'aa\nbb\ncc\ndd' | sed '1d;3d'
# bb
# dd

# Delete last ($) line.
echo -e 'aa\nbb\ncc\ndd' | sed '$d'
# aa
# bb
# cc

# Delete range of lines.
echo -e 'aa\nbb\ncc\ndd' | sed '1,3d'
# dd
```

### Insert lines
```sh
# Insert before line.
echo -e 'aa\nbb' | sed '2iABC'
# aa
# ABC
# bb

# Insert after line.
echo -e 'aa\nbb' | sed '2aABC'
# aa
# bb
# ABC

# Replace line.
echo -e 'aa\nbb' | sed '2cABC'
# aa
# ABC
```

### Substitute lines
```sh
# Substitute by regex.
echo -e 'aafooaa\ncc' | sed 's/foo/MOOSE/'
# aaMOOSEaa
# cc
```

### Multiple scripts
```sh
echo -e 'foo\nbar' | sed -e 's/foo/FOO/' -e 's/FOO/BAR/'
# BAR
# bar
```
