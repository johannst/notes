# tmux(1)

Terminology:
- `session` is a collection of pseudo terminals which can have multiple
  `windows`
- `window` uses the entire screen and can be split into rectangular `panes`
- `pane` is a single pseudo terminal instance

# Tmux cli
```markdown
# Session
tmux                        creates new session
tmux ls                     list running sessions
tmux kill-session -t <s>    kill running session <s>
tmux attach -t <s> [-d]     attach to session <s>, detach other clients [-d]
tmux detach -s <s>          detach all clients from session <s>

# Environment
tmux showenv -g             show global tmux environment variables
tmux setenv -g <var> <val>  set variable in global tmux env

# Misc
tmux source-file <file>     source config <file>
tmux lscm                   list available tmux commnds
tmux show -g                show global tmux options
tmux display <msg>          display message in tmux status line
```
## Scripting
```markdown
# Session
tmux list-sessions -F '#S'           list running sessions, only IDs

# Window
tmux list-windows -F '#I' -t <s>     list window IDs for session <s>
tmux selectw -t <s>:<w>              select window <w> in session <s>

# Pane
tmux list-panes -F '#P' -t <s>:<w>   list pane IDs for window <w> in session <s>
tmux selectp -t <s>:<w>.<p>          select pane <p> in window <w> in session <s>

# Run commands
tmux send -t <s>:<w>.<p> "ls" C-m    send cmds/keys to pane
tmux run -t <p> <sh-cmd>             run shell command <sh-cmd> in background and report output on pane -t <p>
```

For example cycle through all panes in all windows in all sessions:
```bash
# bash
for s in $(tmux list-sessions -F '#S'); do
    for w in $(tmux list-windows -F '#I' -t $s); do
        for p in $(tmux list-panes -F '#P' -t $s:$w); do
            echo $s:$w.$p
        done
    done
done
```

# Bindings

```markdown
prefix d    detach from current session
prefix c    create new window
prefix w    open window list
prefix $    rename session
prefix ,    rename window
prefix .    move current window
```

Following bindings are specific to my [`tmux.conf`](https://github.com/johannst/dotfiles/blob/master/tmux.conf):
```markdown
C-s         prefix

# Panes
prefix s    horizontal split
prefix v    vertical split
prefix f    toggle maximize/minimize current pane

# Movement
prefix Tab  toggle between window

prefix h    move to pane left
prefix j    move to pane down
prefix k    move to pane up
prefix l    move to pane right

# Resize
prefix C-h  resize pane left
prefix C-j  resize pane down
prefix C-k  resize pane up
prefix C-l  resize pane right

# Copy/Paste
prefix C-v    enter copy mode
prefix C-p    paste yanked text
prefix C-b    open copy-buffer list

# In Copy Mode
v     enable visual mode
y     yank selected text
```

# Command mode

To enter command mode `prefix :`.

Some useful commands are:
```markdown
setw synchronize-panes on/off       enables/disables synchronized input to all panes
list-keys -t vi-copy                list keymaps for vi-copy mode
```
