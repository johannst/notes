# xxd(1)

```markdown
  xxd [opts]
    -p          dump continuous hexdump
    -r          convert hexdump into binary ('revert')
    -e          dump as little endian mode
    -i          output as C array
```

## ASCII to hex stream
```markdown
  echo -n 'aabb' | xxd -p
    >> 61616262
```

## Hex to binary stream
```markdown
  echo -n '61616262' | xxd -p -r
    >> aabb
```

## ASCII to binary
```markdown
  echo -n '\x7fELF' | xxd -p | xxd -p -r | file -p -
    >> ELF
```

## ASCII to `C` array (hex encoded)
```markdown
  xxd -i <(echo -n '\x7fELF')
    >> unsigned char _proc_self_fd_11[] = {
    >>   0x7f, 0x45, 0x4c, 0x46
    >> };
    >> unsigned int _proc_self_fd_11_len = 4;
```
