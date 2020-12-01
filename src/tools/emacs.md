# emacs(1)

## help
```markdown
  C-h ?         list available help modes
  C-h e         show message output (`*Messages*` buffer)
  C-h f         describe function
  C-h v         describe variable
  C-h w         describe which key invoke function (where-is)
  C-h c <KEY>   print command bound to <KEY>
  C-h k <KEY>   describe command bound to <KEY>
  C-h b         list buffer local key-bindings
  <kseq> C-h    list possible key-bindings with <kseq>
                eg C-x C-h -> list key-bindings beginning with C-x
```

## package manager
```markdown
  key    fn                          description
------------------------------------------------
         package-refresh-contents    refresh package list
         package-list-packages       list available/installed packages
                                     `U x` to mark packages for Upgrade & eXecute
```

## window
```markdown
  key      fn                      description
----------------------------------------------
  C-x 0    delete-window           kill focused window
  C-x 1    delete-other-windows    kill all other windows
  C-x 2    split-window-below      split horizontal
  C-x 3    split-window-right      split vertical
  C-x o    other-window            other window (cycle)
```

## buffer
```markdown
  key        fn                   description
---------------------------------------------
  C-x C-q    read-only-mode       toggle read-only mode for buffer
  C-x k      kill-buffer          kill buffer
  C-x s      save-some-buffers    save buffer
  C-x w      write-file           write buffer (save as)
  C-x b      switch-to-buffer     switch buffer
  C-x C-b    list-buffers         buffer list
```

## ibuffer
Builtin advanced buffer selection mode
```markdown
  key        fn            description
--------------------------------------
             ibuffer       enter buffer selection

  h                        ibuffer help

  o                        open buffer in other window
  C-o                      open buffer in other window keep focus in ibuffer

  s a                      sort by buffer name
  s f                      sort by file name
  s v                      sort by last viewed
  s v                      sort by major mode
  ,                        cycle sorting mode

  =                        compare buffer against file on disk (if file is dirty `*`)

  /m                       filter by major mode
  /n                       filter by buffer name
  /f                       filter by file name
  //                       remove all filter

  /g                       create filter group
  /\                       remove all filter groups
```

## isearch
```markdown
  key    fn                           description
-------------------------------------------------
  C-s    isearch-forward              search forward from current position (C-s to go to next match)
  C-r    isearch-backward             search backwards from current position (C-r to go to next match)
  C-w    isearch-yank-word-or-char    feed next word to current search (extend)
  M-p    isearch-ring-advance         previous search input
  M-n    isearch-ring-retreat         next search input
```

## occur
```markdown
  key      fn           description
-----------------------------------
  M-s o    occur        get matches for regexp in buffer
                        use during `isearch` to use current search term

  C-n                   goto next line
  C-p                   goto previous line
  o                     open match in other window
  C-o                   open match in other window keep focus in ibuffer
```
```markdown
  key      fn                                 description
---------------------------------------------------------
           multi-occur-in-matching-buffers    run occur in buffers matching regexp
```

## grep
```markdown
  key    fn           description
-----------------------------------
         rgrep        recursive grep
         find-grep    run find-grep result in *grep* buffer

  n/p                 navigate next/previous match in *grep* buffer
  q                   quit *grep* buffer
```

## yank/paste
```markdown
  key         fn                  description
---------------------------------------------
  C-<SPACE>   set-mark-command    set start mark to select text
  M-w         kill-ring-save      copy selected text
  C-w         kill-region         kill selected text
  C-y         yank                paste selected text
  M-y         yank-pop            cycle through kill-ring (only after paste)
```

## register
```markdown
  key             fn                 description
------------------------------------------------
  C-x r s <reg>   copy-to-register   save region in register <reg>
  C-x r i <reg>   insert-register    insert content of register <reg>
```

## block/rect
```markdown
  key          fn                    description
------------------------------------------------
  C-x <SPC>    rectangle-mark-mode   activate rectangle-mark-mode
               string-rectangle      insert text in marked rect
```

## mass edit
```markdown
  key       fn                       description
------------------------------------------------
  C-x h     mark-whole-buffer        mark whole buffer
            delete-matching-line     delete lines matching regex
  M-%       query-replace            search & replace
  C-M-%     query-replace-regexp     search & replace regex
```

## narrow
```markdown
  key       fn                    description
---------------------------------------------
  C-x n n   narrow-to-region      show only focused region (narrow)
  C-x n w   widen                 show whole buffer (wide)
```

## org
```markdown
  key              fn   description
------------------------------------
  M-up/M-down           re-arrange items in same hierarchy
  M-left/M-right        change item hierarchy
  C-RET                 create new item below current
  C-S-RET               create new TODO item below current
  S-left/S-right        cycle TODO states
```

### org source
```markdown
  key       fn     description
------------------------------
  <s TAB           generate a source block
  C-c '            edit source block (in lang specific buffer)
  C-c C-c          eval source block
```

## comapny
```markdown
  key         fn   description
-------------------------------
  C-s              search through completion candidates
  C-o              filter completion candidates based on search term
  <f1>             get doc for completion condidate
  M-<digit>        select completion candidate
```

## tags
To generate `etags` using `ctags`
```markdown
  ctags -R -e .         generate emacs tag file (important `-e`)
```
Navigate using tags
```markdown
  key      fn                       description
-----------------------------------------------
           xref-find-definitions    find definition of tag
           xref-find-apropos        find symbols matching regexp
           xref-find-references     find references of tag
```

## lisp
```markdown
  key   fn        description
------------------------------
        ielm      open interactive elips shell
```

In `lisp-interaction-mode` (`*scratch*` buffer by defult)
```markdown
  key              fn                        description
--------------------------------------------------------
  C-j              eval-print-last-sexp      evaluate & print preceeding lisp expr

  C-x C-e          eval-last-sexp            evaluate lisp expr
  C-u C-x C-e      eval-last-sexp            evaluate & print
```

## ido
Builtin fuzzy completion mode (eg buffer select, dired, ...).
```markdown
  key              fn          description
------------------------------------------
                  ido-mode     toggle ido mode
  <Left>/<Right>               cycle through available competions
  <RET>                        select completion
```

## evil
```markdown
  key    fn    description
--------------------------
  C-z          toggle emacs/evil mode
  C-^          toggle between previous and current buffer
  C-p          after paste cycle kill-ring back
  C-n          after paste cycle kill-ring forward
```

## dired
```markdown
  key    fn    description
--------------------------
  i            open sub-dir in same buffer
  +            create new directory
  C            copy file/dir

  q            quit
```
