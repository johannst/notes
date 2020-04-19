# emacs(1)

## help
```markdown
  C-h ?         list available help modes
  C-h f         describe function
  C-h v         describe variable
  C-h c <KEY>   print command bound to <KEY>
  C-h k <KEY>   describe command bound to <KEY>
  C-h b         list buffer local key-bindings
  <kseq> C-h    list possible key-bindings with <kseq>
                eg C-x C-h -> list key-bindings beginning with C-x
```

## package manager
```markdown
  package-refresh-contents    refresh package list
  package-list-packages       list available/installed packages
```

## window
```markdown
  C-x 0         kill focused window
  C-x 1         kill all other windows
  C-x 2         split horizontal
  C-x 3         split vertical
```

## yank/paste
```markdown
  C-<SPACE>  set start mark to select text
  M-w        copy selected text
  C-w        kill selected text
  C-y        paste selected text
  M-y        cycle through kill-ring
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
