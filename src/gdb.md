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

```markdown
  define <cmd>
    # cmds
  end

  document <cmd>
    # docu
  end

  help user-defined             List user defined commands.
  help <cmd>                    List documentation for command <cmd>.
```

# Hooks

Gdb allows to create two types of command `hooks` which will be either executed
before or after a certain command.

```markdown
  define hook-<cmd>             Run commands defined in hook before
    # cmds                      executing <cmd>.
  end

  define hookpost-<cmd>         Run commands defined in hookpost after
    # cmds                      executing <cmd>.
  end
```

# Flows

## Catch SIGSEGV and execute commands on occurrence
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
```markdown
# run.gdb
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
  gdb -p <pid> -x ./run.gdb  --batch &> run.log
```

## Workaround `command + finish` bug
When using `finish` action inside a `command` block, actions after `finish` are
not executed anymore. To workaround that bug one can create a wrapper function
which calls `finish`.
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
