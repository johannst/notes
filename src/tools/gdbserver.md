# gdbserver(1)

# CLI

```markdown
  gdbserver [opts] comm prog [args]
    opts:
      --disable-randomization
      --no-disable-randomization

    comm:
      host:port
      tty
```

# Example

```markdown
# Start gdbserver.
gdbserver localhost:1234 /bin/ls

# Attach gdb.
gdb -ex 'target remote localhost:1234'
```
