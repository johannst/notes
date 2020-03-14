# emacs(1)

## help
```markdown
  C-h f                 describe function
  C-h b                 list buffer available keymaps
  <kseq> C-h            list possible keymaps with <kseq>
                        eg C-x C-h -> list keymaps beginning with C-x
```

## window
```markdown
  C-x 0         kill focused window
  C-x 1         kill all other windows
  C-x 2         split horizontal
  C-x 3         split vertical
```

## block/rect
```markdown
  C-x <SPC>                     activate rectangle-mark-mode
  M-x string-rectangle <RET>    insert text in marked rect
```

## mass edit
```makrdown
  C-x h                                 mark whole buffer (mark-whole-buffer)
  M-x delete-matching-line <RET>        delete lines matching regex
  M-x %                                 search & replace region (query-replace)
  C-M-x %                               search & replace regex (query-replace-regexp)
```

## grep
```markdown
  M-x find-grep <RET>           run find-grep result in *grep* buffer
  n/p                           navigate next/previous match in *grep* buffer
```

## lisp mode
```markdown
  M-x lisp-interaction-mode     activate lisp mode
  C-M-x                         evaluate top expr under cursor
  C-x C-e                       eval-last-sexp
  C-u C-x C-e                   eval-last-sexp and prints result in current buffer
```

## narrow
```markdown
  C-x n n               show only focused region (narrow)
  C-x n w               show whole buffer (wide)
```

## org
```markdown
  M-up/M-down           re-arrange items in same hierarchy
  M-left/M-right        change item hierarchy
  C-RET                 create new item below current
  C-S-RET               create new TODO item below current
  S-left/S-right        cycle TODO states
```

### org source
```markdown
  <s TAB                generate a source block
  C-c '                 edit source block (in lang specific buffer)
  C-c C-c               eval source block
```
