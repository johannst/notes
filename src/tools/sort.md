# sort(1)

```
sort [opts] [file]
  opts:
    -r      reverse output
    -b      ignore leading blanks

    -n      sort by numeric
    -h      sort by human numeric
    -V      sort by version

    -k<N>  sort by Nth key
    -t<S>  field separator
```

## Examples
```sh
# Sort by directory sizes.
du -sh * | sort -h
```

```sh
# Sort numeric by second key.
# The default key separator is non-blank to blank transition.
echo 'a 4
d 10
c 21' | sort -k2 -n

# Sort numeric by second key, split at comma.
echo 'a,4
d,10
c,21' | sort -k2 -n -t,
```
> Use `--debug` to annotate part of the line used to sort and hence debug the key usage.
