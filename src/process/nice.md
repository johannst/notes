# nice(1)

Adjust `niceness` of a new command or running process.

Niceness influences the scheduling priority and ranges between:
- `-20` most favorable
- `19` least favorable

```sh
# Adjust niceness +5 for the launched process.
nice -n 5 yes

# Adjust niceness of running process.
renice -n 5 -p PID
```
