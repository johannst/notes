# pgrep(1)

```markdown
pgrep [opts] <pattern>
  -n         only list newest matching process
  -u <usr>   only show matching for user <usr>
  -l         additionally list command
  -a         additionally list command + arguments
  -x         match exactly
```

## Debug newest process
For example attach gdb to newest zsh process from `$USER`.
```markdown
gdb -p $(pgrep -n -u $USER zsh)
```
