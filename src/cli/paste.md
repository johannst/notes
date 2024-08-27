# paste(1)

```
# Concatenate input files linewise and join them by a TAB char.

paste FILE [FILE]
    -d CHAR     delimiter to join each line
```

# Examples

```sh
# Read two files.
paste <(echo -e 'a1\na2') <(echo -e 'b1\nb2')
a1	b1
a2	b2

# Can read from stdin.
echo -e 'a1 a2\nb1 b2\nc1 c2\nd1 d2' | paste - -
# a1 a2	b1 b2
# c1 c2	d1 d2
```
