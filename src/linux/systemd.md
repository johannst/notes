# systemd

## systemctl

Inspect units:
```text
systemctl [opts] [cmd]
[opts]
    --user

[cmd]
    list-units <pattern>    List units in memory

    status <unit>           Show runtime status of unit

    start <unit>            Start a unit
    stop <unit>             Stop a unit
    restart <unit>          Restart a unit
    reload <unit>           Reload a unit

    enable <unit>           Enable a unit (persistent)
    disable <unit>          Disable a unit

    cat <unit>      Print unit file
    show <unit>     Show properties of unit
```

### Example: Trivial user unit

```bash
# Generate unit
mkdir -p ~/.config/systemd/user
echo '[Unit]
Description=Test logger

[Service]
Type=oneshot
ExecStart=logger "Hello from test unit"' > ~/.config/systemd/user/test.service

# Run unit
systemctl --user start test

# See log message
journalctl --user -u test -n 5
```

## journalctl

Inspect journal logs:
```text
journalctl [opts] [matches]
    --user          Current user journal (system by default)
    -u <unit>       Show logs for specified <unit>
    -n <lines>      Show only last <lines>
    -f              Follow journal
    -g <pattern>    Grep for <pattern>
```

Cleanup:
```text
journalctl [opts]
    --disk-usage            Show current disk usage
    --vacuum-size=<size>    Reduce journal log to <size> (K/M/G)
```

## References
- [man systemd.unit(5)](https://www.man7.org/linux/man-pages/man5/systemd.unit.5.html)
- [man systemd.service(5)](https://www.man7.org/linux/man-pages/man5/systemd.service.5.html)
