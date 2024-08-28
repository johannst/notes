# screen(1)

```sh
# Create new session.
screen

# List active session.
screen -list

# Attach to specific session.
screen -r SESSION
```

## Options
```sh
# Enable logfile, default name screenlog.0.
screen -L
# Enable log and set logfile name.
screen -L -Logfile out.txt
```

## Keymaps
```sh
Ctrl-A d            # Detach from session.
Ctrl-A + \          # Terminate session.
Ctrl-A + :          # Open cmand prompt.
    kill            # Kill session.
```

## Examples
USB serial console.
```sh
# 1500000 -> baudrate
# cs8     -> 8 data bits
# -cstopb -> 1 stop bit
# -parenb -> no parity bit
# see stty(1) for all settings.
screen /dev/ttyUSB0 1500000,cs8,-cstopb,-parenb

# Print current tty settings.
sudo stty -F /dev/ttyUSB0 -a
```
