# ps(1)

```
ps [opt]
  opt:
    --no-header .... do not print column header
    -o <OUT> ....... comma separated list of output columns
    -p <PID> ....... only show pid
    -C <name> ...... only show processes matching name
    -T ............. list threads
    --signames ..... use short signames instead bitmasks
```
> Set `PS_FORMAT` env variable to setup default output columns.

Frequently used output columns
```
pid        process id
ppid       parent process id
pgid       process group id
tid        thread id

comm       name of process
cmd        name of process + args (full)

etime      elapsed time (since process started)
user       user owning process
thcount    thread count of process
nice       nice value (-20 highest priority to 19 lowest)

pcpu       cpu utilization (percent)
pmem       physical resident set (rss) (percent)
rss        physical memory (in kb)
vsz        virtual memory (in kb)

sig        mask of pending signals
sigcatch   mask of caught signals
sigignore  mask of ignored signals
sigmask    mask of blocked signals
```

## Example: Use output for scripting
```sh
for tid in $(ps -o tid --no-header -T -p 31084); do
    taskset -c -p $tid;
done
```

## Example: Watch processes by name
```sh
watch -n1 ps -o pid,pcpu,pmem,rss,vsz,state,user,comm -C fish
```

## Example: Show signal information
```sh
# With signal masks.
ps -o pid,user,sig,sigcatch,sigignore,sigmask,comm -p 66570

# With signal names.
ps --signames -o pid,user,sig,sigcatch,sigignore,sigmask,comm -p 66570
```
