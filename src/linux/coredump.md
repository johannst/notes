# core(5)

There are multiple requirements that must be satisfied that `coredumps` are
being generated, a full list can be found in [core(5)][man-core].

An important one is to configure the soft resource limit `RLMIT_CORE`
(typically as unlimited during debugging).
In a typical bash/zsh this can be done as
```bash
ulimit -Sc unlimited
```

## Naming of coredump files

There are two important kernel configs to control the naming:
```config
/proc/sys/kernel/core_pattern
    <pattern>    => Specifies a name pattern for the coredump file. This can
                    include certain FORMAT specifier.
    |<cmdline>   => Coredump is pipe through stdin to the user space process
                    specified by the cmdline, this can also contain FORMAT specifier.

  FORMAT specifier (full list, see core(5)):
    %E      Pathname of the executable ('/' replaced by '!').
    %p      PID of the dumping process in its pid namespace.
    %P      PID of the dumping process in the initial pid namespace.
    %u      Real UID of dumping process.
    %s      Signal number causing the dump.


/proc/sys/kernel/core_uses_pid
    1  => Append ".<pid>" suffic to the coredump file name
          (pid of the dumping process).
    0  => Do not append the suffix.
```

## Control which segments are dumped

Each process has a coredump filter defined in `/proc/<pid>/coredump_filter`
which specifies which memory segments are being dumped.
Filters are preseved across `fork/exec` calls and hence child processes inherit
the parents filters.

The filter is a bitmask where `1` indicates to dump the given type.
```
From core(5):
  bit 0  Dump anonymous private mappings.
  bit 1  Dump anonymous shared mappings.
  bit 2  Dump file-backed private mappings.
  bit 3  Dump file-backed shared mappings.
  bit 4  Dump ELF headers.
  bit 5  Dump private huge pages.
  bit 6  Dump shared huge pages.
  bit 7  Dump private DAX pages.
  bit 8  Dump shared DAX pages.

Default filter 0x33.
```

# Some examples out there

## coredumpctl (systemd)

```bash
# List available coredumps.
coredumpctl list
    TIME                             PID  UID  GID SIG     COREFILE EXE               SIZE
    ...
    Fri 2022-03-11 12:10:48 CET     6363 1000 1000 SIGSEGV present  /usr/bin/sleep   18.1K

# Get detailed info on specific coredump.
coredumpctl info 6363

# Debug specific coredump.
coredumpctl debug 6363

# Dump specific coredump to file.
coredumpctl dump 6363 -o <file>
```

## apport (ubuntu)

Known crash report locations:
- `/var/crash`

To get to the raw coredump, crash reports can be unpacked as:
```bash
apport-unpack <crash_repot> <dest_dir>
```
The coredump resides under `<dest_dir>/CoreDump`.

[man-core]: https://man7.org/linux/man-pages/man5/core.5.html
