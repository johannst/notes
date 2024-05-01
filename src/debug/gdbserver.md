# gdbserver(1)

# CLI

```markdown
  gdbserver [opts] comm prog [args]
    opts:
      --disable-randomization
      --no-disable-randomization
      --wrapper W --

    comm:
      host:port
      tty
```

## Example

```markdown
# Start gdbserver.
gdbserver localhost:1234 /bin/ls

# Attach gdb.
gdb -ex 'target remote localhost:1234'
```

## Wrapper example: Set environment variables just for the debugee
Set `env` as execution wrapper with some variables.
The wrapper will be executed before the debugee.

```markdown
gdbserver --wrapper env FOO=123 BAR=321 -- :12345 /bin/ls
```
