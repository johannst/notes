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
  C-h F         show emacs manual for command/function
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

  C-x r w  window-configuration-to-register
                                   save window configuration in a register
                                   (use C-x r j to jump to the windows config again)
  C-x w d toggle-window-dedicated  toggles windows dedicated state
                                   (dedicated windows cant be reused by display functions)
```

## minibuffer
```markdown
  key            description
----------------------------
  M-e            enter edit minibuffer edit mode

  M-up           focus previous completion
  M-down         focus next completion
  M-ret          select focused completion
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
  C-x x r    rename-buffer        renames a buffer (allows multiple shell, compile, grep, .. buffers)
```

## ibuffer
Builtin advanced buffer selection mode
```markdown
  key        fn            description
--------------------------------------
             ibuffer       enter buffer selection

  h                        ibuffer help

  d                        mark for deletion
  x                        kill buffers marked for deletion

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
  /i                       filter by modified buffers
  /E                       filter by process buffers
  //                       remove all filter

  /g                       create filter group
  /\                       remove all filter groups
```

## goto navigation
```markdown
  key      fn                description
----------------------------------------
  M-g g    goto-line         go to line
  M-g M-n  next-error        go to next error (grep, xref, compilation, ...)
  M-g M-p  previous-error    go to previous error

  M-g i    imenu             go to place in buffer (symbol, ...)

  M-<                        go to begin of buffer
  M->                        go to end of buffer
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
  M-e    isearch-edit-string          edit search string again
  M-s o  occur                        open search string in occur
```

## occur
```markdown
  key      fn           description
-----------------------------------
  M-s o    occur        get matches for regexp in buffer
                        use during `isearch` to use current search term
  e                     enter occur edit mode (C-c C-c to quit)

  n                     move to next entry and keep focus in occur buffer
  p                     move to previous entry and keep focus in occur buffer
  C-n                   goto next line
  C-p                   goto previous line
  o                     open match in other window
  C-o                   open match in other window keep focus in ibuffer
```
```markdown
  key      fn                                 description
---------------------------------------------------------
           multi-occur-in-matching-buffers    run occur in buffers matching regexp
           occur-edit-mode                    in occur buffer, enter edit mode.
                                              C-c C-c after editing to save changes.
```

## grep
```markdown
  key    fn           description
-----------------------------------
         rgrep        recursive grep
         lgrep        local dir grep
         grep         raw grep command
         find-grep    run find-grep result in *grep* buffer

  n/p                 navigate next/previous match in *grep* buffer
  q                   quit *grep* buffer
```

## highlight
```markdown
  key      fn                                  description
----------------------------------------------------------
  M-s h .  highlight-symbol-at-point           use C-u prefix to select color
  M-s h r  highlight-regexp                    use C-u prefix to select color
  M-s h l  highlight-lines-matching-regexp     use C-u prefix to select color

  M-s h u  unhighlight-regexp                  use C-u prefix to unselect all
```

## yank/paste
```markdown
  key         fn                      description
-------------------------------------------------
  C-<SPACE>   set-mark-command        set start mark to select text
  C-x C-x     exchange-point-and-mark swap mark and point position
  M-w         kill-ring-save          copy selected text
  C-w         kill-region             kill selected text
  C-y         yank                    paste selected text
  M-y         yank-pop                cycle through kill-ring (only after paste)
  M-y         yank-from-kill-ring     interactively select yank from kill ring
```


## register
```markdown
  key             fn                 description
------------------------------------------------
  C-x r s <reg>   copy-to-register   save region in register <reg>
  C-x r i <reg>   insert-register    insert content of register <reg>
```

## bookmarks
```
  key             fn            description
-------------------------------------------
  C-x r m  bookmark-set         set a bookmark
  C-x r b  bookmark-jump        jump to a bookmark
  C-x r l  bookmark-bmenu-list  list all bookmarks
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
            delete-matching-line     delete lines matchng regex
            kill-matching-line       kill lines matching regex (puts them in kill ring)
            keep-lines               keep matching lines
            replace-string           replace unconditional
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

## project
```markdown
  key       fn                                 description
----------------------------------------------------------
  C-x p p   project-switch-project             switch project
  C-x p f   project-find-file                  find file in project
  C-x p r   project-query-replace-regexp       query replace on project
  C-x p x   project-execute-extended-command   exec command on project
  C-x p !   project-shell-command              shell command on project
  C-x p &   project-async-shell-command        async shell command on project
```

## vc
```markdown
  key       fn                          description
---------------------------------------------------
  C-x v =   vc-diff                     show diff of buffer
  C-x v ~   vc-revision-other-window    show other revison of buffer
```

## tags / lsp
To generate `etags` using `ctags`
```markdown
  ctags -R -e .         generate emacs tag file (important `-e`)
```
Navigate using tags
```markdown
  key      fn                       description
-----------------------------------------------
  M-.      xref-find-definitions    find definition of tag
                                    (C-u prefix to enter symbol manually)
           xref-find-apropos        find symbols matching regexp
  M-?      xref-find-references     find references of tag
```

## completion
```markdown
  key     fn                   description
------------------------------------------
  M-/     dabbrev-expand       expand dynamically at point
  C-M-/   dabbrev-completion   provide completion at point
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

There is also `fido`, which is the successor of `ido`, which also
supports `fido-vertical-mode` in case vertical mode is preferred.

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
  R            move file/dir (rename)
  S            absolute symbolic link
  Y            relative symbolic link

  d            mark for deletion
  m            mark file/dir at point
  * %          mark by regex
  * *          mark all executables
  * /          mark all dirs
  u            un-mark file/dir
  U            un-mark all
  t            toggle marks

  x            execute marked actions
  !            run shell command on marked files
  &            run shell command on marked files (async)

  q            quit
```

## info
```markdown
  key fn                    description
---------------------------------------
  n   Info-next             next page
  p   Info-prev             previous page

  l   Info-history-back     history go back
  r   Info-history-forward  history go forward

  ^   Info-Up               up in info node tree

  s   Info-search           search info
  g   Info-goto-node        goto info node (by minibuf completion)
                            a node is any section in a node tree
  m   Info-menu             goto menu (by minibuf completion)
                            a node in the current nodes menu list

      Info-history          open info history in buffer
```

## shell commands
```markdown
  key   fn                        description
---------------------------------------------
  M-!   shell-command             run shell command synchronously
  M-&   async-shell-command       run shell command asynchronously
  M-|   shell-command-on-region   run shell command on region;
                                  prefix with C-u to replace region with
                                  output of the command
```

## interactive shell

Set `ESHELL` environment variable before starting emacs to change
default shell, else customize the `explicit-shell-file-name` variable.
```markdown
  key      fn                    description
---------------------------------------------
  M-x      shell                 start interactive shell
  C-u M-x  shell                 start interactive shell & rename


  M-r      comint-history-isearch-backward-regexp
                                 search history, invoke at end of shell buffer
  M-p      comint-previous-input go one up in history
  C-<UP>
  M-n      comint-next-input     go one down in history
  C-<DOWN>

  C-c C-a                        go begin of line (honors prompt)
  C-c C-e                        go to end of line
  C-c C-c                        interrupt active command
```

## gdb

```markdown
  key           fn                  description
------------------------------------------------
  C-x C-a C-b   gud-break           create breakpoint

                gdb-many-windows    toggle gdb many window view
```


## lisp
```markdown
  key   fn        description
------------------------------
        ielm      open interactive elips shell
```

In `lisp-interaction-mode` (`*scratch*` buffer by default)
```markdown
  key              fn                           description
-----------------------------------------------------------
  C-j              eval-print-last-sexp         evaluate & print symbolic expression
                                                (last before the cursor)
  C-x C-e          eval-last-sexp               evaluate symbolic expression (last)

  C-c C-e          elisp-eval-region-or-buffer  eval buffer or region (elisp mode)

  C-M-x            eval-defun                   evaluate top-level function
                                                (surrounding cursor position)
```

### elisp

When evaluating a symbol by itself, its value is returned.
```lisp
(setq foo "bar")

foo
"bar"          ;; C-j
```
When evaluating a list, the first symbol in the list is interpreted as function.
```lisp
(defun foo (a b)
  (+ 1 2 a b))

(foo 1 2)
6              ;; C-j

;; Expressions are evaluated left-to-right and from inside-to-outside.
(+ 1 (string-to-number "2"))
3              ;; C-j
```
A symbol can be bound to a `value` and a `function` at the same time.

```lisp
(setq moose 3)

(defun moose ()
  (concat "abc" "def"))

moose
3              ;; C-j eval as value

(moose)
"abcdef"       ;; C-j eval as function
```
If a symbol is `quoted` it is not evaluated.
```lisp
'foo
foo            ;; C-j dont eval as value

'(a b c)
(a b c)        ;; C-j dont eval as function

(setq foo '(a b)) ;; the inner list is not evaluated
foo            ;; C-j eval as value, contains un-evaluated list
(a b)

;; (quote ..) is equivalent to '
(quote bar)
bar            ;; C-j

(quote (c d))
(c d)          ;; C-j

;; print argument and print evaluated result
(defun p (arg)
  (message "arg =%s" arg)
  (message "eval=%s" (eval arg)))

;; pass quoted variable
(setq foo '(a b))
(p 'foo)
"arg =foo"     ;; C-j and take print from *Message* buffer
"eval=(a b)"

;; pass quoted function
(p '(concat "a" "b"))
"arg =(concat a b)"    ;; C-j and take print from *Message* buffer
"eval=ab"
```
