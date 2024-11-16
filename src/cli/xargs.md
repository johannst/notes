# xargs(1)

```
xargs [opts] [cmd [init-args]]
    -l [<num>]   maximal number of lines per cmd invocation
                 if <num> it not provided, num=1 is assumed
    -I <str>     replace <str> in the [init-args] with the arg;
                 this implies -l, and hence processes one arg at a time
```

## Example

Collect arguments and prefix them with another option.
```sh
# Using -l to process one arg at a time.
eval strace -f (find /dev -name 'std*' | xargs -l echo -P | xargs) ls

# Using -I to place the arg at the specified location.
eval strace -f (find /dev -name 'std*' | xargs -I {} echo -P {}) ls

# Both commands achieve the same thing and result in something like:
#   eval strace -f -P /dev/stdin -P /dev/stdout -P /dev/stderr ls
```