# c++filt(1)

## Demangle symbol
```markdown
  c++-filt <symbol_str>
```

## Demangle stream
For example dynamic symbol table:
```markdown
  readelf -W --dyn-syms <elf> | c++filt
```
