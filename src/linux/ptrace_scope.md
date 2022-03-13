# ptrace_scope

In case the kernel was compiled with the `yama` security module
(`CONFIG_SECURITY_YAMA`), tracing processes with `ptrace(2)` can be restricted.

```config
/proc/sys/kernel/yama/ptrace_scope
    0 => No restrictions.
    1 => Restricted attach, only the following can attach
            - A process in the parent hierarchy.
            - A process with CAP_SYS_PTRACE.
            - A process with the PID that the tracee allowed by via
              PR_SET_PTRACER.
    2 => Only processes with CAP_SYS_PTRACE in the user namespace of the tracee
         can attach.
    3 => No tracing allowed.
```

Further details in [`ptrace(2)`][man-ptrace].

[man-ptrace]: https://man7.org/linux/man-pages/man2/ptrace.2.html
