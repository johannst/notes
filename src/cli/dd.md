# dd(1)

Copy data `block-wise`.

```
dd [opts]
    if=<path>         input file to read (stdin in case not specified)
    of=<path>         oputput file to write
    status=progress   show progress while copying
    bs=<bytes>        block size
    count=<n>         copy only <n> blocks
    skip=<n>          skip <n> blocks in input (seek input)
    seek=<n>          skip <n> blocks in oputput (seek output)
    conv=<conv>
         notrunc      dont truncate output file
         excl         fail if output already exists
         nocreat      fail if output does not exists
```

## Example: bootstick

```bash
dd bs=4M if=<iso> of=<blkdev> oflag=sync status=progress
```

## Example: patch file in place

```bash
# Create a 1024 bytes file filled with zeros.
dd if=/dev/zero of=disk bs=512 count=2

# Overwrite 4 bytes starting at byte 0.
printf "aaaa" | dd of=disk bs=1 seek=0 conv=notrunc

# Overwrite 4 bytes starting at byte 512.
printf "bbbb" | dd of=disk bs=1 seek=512 conv=notrunc

hexdump disk
# 0000000 6161 6161 0000 0000 0000 0000 0000 0000
# 0000010 0000 0000 0000 0000 0000 0000 0000 0000
# *
# 0000200 6262 6262 0000 0000 0000 0000 0000 0000
# 0000210 0000 0000 0000 0000 0000 0000 0000 0000
# *
# 0000400
```
