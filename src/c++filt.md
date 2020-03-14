# c++filt(1)

## demangle symbol
```markdown
  c++-filt <symbol_str>
```

## demangle stream (eg dynamic symbol table)
```markdown
  readelf -W --dyn-syms <elf> | c++filt
```
