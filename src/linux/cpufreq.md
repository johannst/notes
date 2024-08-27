# cpufreq

The `sysfs` interface to cpu frequency settings and current state.
```
/sys/devices/system/cpu/cpu*/cpufreq/
```

## cpupower(1)

A CLI interface to peek and poke the cpu frequency settings.

```
# Show current frequency of all cores.
cpupower -c all frequency-info -f -m

# Show currently set frequency governor.
cpupower -c all frequency-info -p

# List available frequency governors.
cpupower -c all frequency-info -g

# Change frequency governor to POWERSAVE (eg).
cpupower -c all frequency-set -g powersave
```

## Example

Watch cpu frequency.
```sh
watch -n1 "cpupower -c all frequency-info -f -m | xargs -n2 -d'\n'"
```
