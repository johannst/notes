# gdb(1)

# CLI

```markdown
  gdb [opts] [prg [-c coredump | -p pid]]
  gdb [opts] --args prg <prg-args>
    opts:
      -p <pid>        attach to pid
      -c <coredump>   use <coredump>
      -x <file>       execute script <file> before prompt
      -ex <cmd>       execute command <cmd> before prompt
      --tty <tty>     set I/O tty for debugee
```

# Interactive usage

```markdown
  tty <tty>
          Set <tty> as tty for debugee.
          Make sure nobody reads from target tty, easiest is to spawn a shell
          and run following in target tty:
          > while true; do sleep 1024; done

  set follow-fork-mode <child | parent>
          Specify which process to follow when debuggee makes a fork(2)
          syscall.

  sharedlibrary [<regex>]
          Load symbols of shared libs loaded by debugee. Optionally use <regex>
          to filter libs for symbol loading.

  break [-qualified] <sym> thread <tnum>
          Set a breakpoint only for a specific thread.
          -qualified: Tred <sym> as fully qualified symbol (quiet handy to set
          breakpoints on C symbols in C++ contexts)

  rbreak <regex>
          Set breakpoints matching <regex>, where matching internally is done
          on: .*<regex>.*

  command [<bp_list>]
          Define commands to run after breakpoint hit. If <bp_list> is not
          specified attach command to last created breakpoint. Command block
          terminated with 'end' token.

          <bp_list>: Space separates list, eg 'command 2 5-8' to run command
          for breakpoints: 2,5,6,7,8.

  info functions [<regex>]
          List functions matching <regex>. List all functions if no <regex>
          provided.

  info variables [<regex>]
          List variables matching <regex>. List all variables if no <regex>
          provided.

  info handle [<signal>]
          Print how to handle <signal>. If no <signal> specified print for all
          signals.

  handle <signal> <action>
          Configure how gdb handles <signal> sent to debugee.
          <action>:
            stop/nostop       Catch signal in gdb and break.
            print/noprint     Print message when gdb catches signal.
            pass/nopass       Pass signal down to debugee.

  catch signal <signal>
          Create a catchpoint for <signal>.
```

# User commands (macros)

Gdb allows to create & document user commands as follows:
```markdown
  define <cmd>
    # cmds
  end

  document <cmd>
    # docu
  end
```

To get all user commands or documentations one can use:
```markdown
  help user-defined
  help <cmd>
```

# Hooks

Gdb allows to create two types of command `hooks`
- `hook-` will be run before `<cmd>`
- `hookpost-` will be run after `<cmd>`
```markdown
  define hook-<cmd>
    # cmds
  end

  define hookpost-<cmd>
    # cmds
  end
```

# Examples

## Catch SIGSEGV and execute commands
This creates a `catchpoint` for the `SIGSEGV` signal and attached the `command`
to it.
```markdown
  catch signal SIGSEGV
  command
    bt
    c
  end
```

## Run `backtrace` on thread 1 (batch mode)
```markdown
  gdb --batch -ex 'thread 1' -ex 'bt' -p <pid>
```

## Script gdb for automating debugging sessions
To script gdb add commands into a file and pass it to gdb via `-x`.
For example create `run.gdb`:
```markdown
  set pagination off

  break mmap
  command
    info reg rdi rsi rdx
    bt
    c
  end

  #initial drop
  c
```
This script can be used as:
```markdown
  gdb --batch -x ./run.gdb -p <pid>
```

# Know Bugs

## Workaround `command + finish` bug
When using `finish` inside a `command` block, commands after `finish` are not
executed. To workaround that bug one can create a wrapper function which calls
`finish`.
```markdown
  define handler
    bt
    finish
    info reg rax
  end

  command
    handler
  end
```
