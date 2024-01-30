# sed(1)

```
sed [opts] [script] [file]
  opts:
    -i          edit file in place
    -i.bk       edit file in place and create backup file
                (with .bk suffix, can be specified differently)
    --follow-symlinks
                follow symlinks when editing in place
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

# Delete lines matching pattern.
echo -e 'aa\nbb\ncc\ndd' | sed '/bb/d'
# aa
# cc
# dd

# Delete lines NOT matching pattern.
echo -e 'aa\nbb\ncc\ndd' | sed '/bb/!d'
# bb
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

# Insert before pattern match.
echo -e 'aa\nbb' | sed '/bb/i 123'
# aa
# 123
# bb
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

### Edit inplace through symlink
```sh
touch file
ln -s file link
ls -l link
# lrwxrwxrwx 1 johannst johannst 4 Feb  7 23:02 link -> file

sed -i --follow-symlinks '1iabc' link
ls -l link
# lrwxrwxrwx 1 johannst johannst 4 Feb  7 23:02 link -> file

sed -i '1iabc' link
ls -l link
# -rw-r--r-- 1 johannst johannst 0 Feb  7 23:02 link
```
