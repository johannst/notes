# tr(1)

```
tr [opt] str1 [str2]
    -d      delete characters in str1
    -s      squeeze repeating sequence of characters in str1
```

## Examples

### To lower
```sh
echo MoOsE | tr '[:upper:]' '[:lower:]'
# output: moose
```

### Replace characters
```sh
echo moose | tr 'o' '-'
# output: m--se

echo moose | tr 'os' '-'
# output: m---e
```

### Remove specific characters
```sh
echo moose | tr -d 'o'
# output: mse

echo moose | tr -d 'os'
# output: me
```

### Squeeze character sequences
```sh
echo moooooossse | tr -s 'os'
# output: mose
```
