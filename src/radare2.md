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
