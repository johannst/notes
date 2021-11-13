# strace(1)

```markdown
strace [opts] [prg]
  -f .......... follow child processes on fork(2)
  -p <pid> .... attach to running process
  -s <size> ... max string size, truncate of longer (default: 32)
  -e <expr> ... expression for trace filtering
  -o <file> ... log output into <file>
  -c .......... dump syscall statitics at the end
  -k .......... dump stack trace for each syscall
  -P <path> ... only trace syscall accesing path
  -y .......... print paths for FDs
  -tt ......... print absolute timestamp (with us precision)
  -r .......... print relative timestamp
```

```markdown
<expr>:
  trace=syscall[,syscall] .... trace only syscall listed
  trace=file ................. trace all syscall that take a filename as arg
  trace=process .............. trace process management related syscalls
  trace=signal ............... trace signal related syscalls
  signal ..................... trace signals delivered to the process
```

# Examples

Trace `open(2)` & `socket(2)` syscalls for a running process + child processes:
```markdown
strace -f -e trace=open,socket -p <pid>
```

Trace signals delivered to a running process:
```markdown
strace -e signal -e 'trace=!all' -p <pid>
```
