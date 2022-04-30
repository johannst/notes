# radare2(1)

## print
```markdown

  pd <n> [@ <addr>]     # print disassembly for <n> instructions
                        # with optional temporary seek to <addr>
```

## flags
```markdown
  fs            # list flag-spaces
  fs <fs>       # select flag-space <fs>
  f             # print flags of selected flag-space
```

## help
```markdown
  ?*~<kw>       # '?*' list all commands and '~' grep for <kw>
  ?*~...        # '..' less mode /'...' interactive search
```

## relocation
```markdown
  > r2 -B <baddr> <exe>         # open <exe> mapped to addr <baddr>
  oob <addr>                    # reopen current file at <baddr>
```

# Examples

## Patch file (alter bytes)
```markdown
  > r2 [-w] <file>
  oo+           # re-open for write if -w was not passed
  s <addr>      # seek to position
  wv <data>     # write 4 byte (dword)
```

## Assemble / Disassmble (rasm2)
```markdown
  rasm2 -L      # list supported archs

  > rasm2 -a x86 'mov eax, 0xdeadbeef'
  b8efbeadde

  > rasm2 -a x86 -d "b8efbeadde"
  mov eax, 0xdeadbeef
```
