# objdump(1)

```markdown
  objdump [opts] <elf>
    -M intel                use intil syntax
    -d                      disassemble text section
    -D                      disassemble all sections
    -S                      mix disassembly with source code
    -C                      demangle
    -j <section>            display info for section
    --[no-]show-raw-insn    [dont] show object code next to disassembly
```

## Disassemble .plt section
```markdown
  objdump -j .plt -d <elf>
```
