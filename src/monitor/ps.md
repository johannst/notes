# ps(1)

```
ps [opt]
  opt:
    --no-header .... do not print column header
    -o <OUT> ....... comma separated list of output columns
    -p <PID> ....... only show pid
    -T ............. list threads
```
> Set `PS_FORMAT` env variable to setup default output columns.

Frequently used output columns
```
pid        process id
pgid       process group id
tid        thread id

comm       name of process
cmd        name of process + args (full)

etime      elapsed time (since process started)
user       user owning process

pcpu       cpu utilization (percent)
pmem       physical resident set (rss) (percent)
rss        physical memory (in kb)
vsz        virtual memory (in kb)
```

## Example: Use output for scripting
```sh
for tid in $(ps -o tid --no-header -T -p 31084); do
    taskset -c -p $tid;
done
```
