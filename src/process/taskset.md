# taskset(1)

Set cpu affinity for new processes or already running ones.

```sh
# Pin all (-a) tasks of new command on cores 0,1,2,4.
taskset -ac 0-2,4 CMD [ARGS]

# Pin all tasks of running PID onto cores 0,2,4.
taskset -ac 0-5:2 -p PID
```

### Example
Utility script to extract cpu lists grouped by the last-level-cache.
```python
import subprocess

res = subprocess.run(["lscpu", "-p=cpu,cache"], capture_output=True, check=True)

LLC2CPU = dict()

for line in res.stdout.decode().splitlines():
    if line.startswith("#"):
        continue

    cpu, cache = line.split(",")
    llc        = cache.split(":")[-1]

    LLC2CPU.setdefault(llc, list()).append(int(cpu))

LLC2RANGE = dict()

for llc, cpus in LLC2CPU.items():
    first_cpu = cpus[0]
    prev_cpu  = cpus[0]
    for cpu in cpus[1:]:
        if cpu != prev_cpu + 1:
            LLC2RANGE.setdefault(llc, list()).append(f"{first_cpu}-{prev_cpu}")
            # New range begins.
            first_cpu = cpu
        prev_cpu = cpu
    # Trailing range.
    LLC2RANGE.setdefault(llc, list()).append(f"{first_cpu}-{prev_cpu}")

print(LLC2RANGE)
```
