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

## Misc
```markdown
  tty <tty>
          Set <tty> as tty for debugee.
          Make sure nobody reads from target tty, easiest is to spawn a shell
          and run following in target tty:
          > while true; do sleep 1024; done

  sharedlibrary [<regex>]
          Load symbols of shared libs loaded by debugee. Optionally use <regex>
          to filter libs for symbol loading.

  display [/FMT] <expr>
          Print <expr> every time debugee stops. Eg print next instr, see
          examples below.

  undisplay [<num>]
          Delete display expressions either all or one referenced by <num>.

  info display
          List display expressions.
```

## Breakpoints
```markdown
  break [-qualified] <sym> thread <tnum>
          Set a breakpoint only for a specific thread.
          -qualified: Treat <sym> as fully qualified symbol (quiet handy to set
          breakpoints on C symbols in C++ contexts)

  break <sym> if <cond>
          Set conditional breakpoint (see examples below).

  delete [<num>]
          Delete breakpoint either all or one referenced by <num>.

  info break
          List breakpoints.

  cond <bp> <cond>
          Make existing breakpoint <bp> conditional with <cond>.

  tbreak
          Set temporary breakpoint, will be deleted when hit.
          Same syntax as `break`.

  rbreak <regex>
          Set breakpoints matching <regex>, where matching internally is done
          on: .*<regex>.*

  command [<bp_list>]
          Define commands to run after breakpoint hit. If <bp_list> is not
          specified attach command to last created breakpoint. Command block
          terminated with 'end' token.

          <bp_list>: Space separates list, eg 'command 2 5-8' to run command
          for breakpoints: 2,5,6,7,8.

  save break <file>
          Save breakpoints to <file>. Can be loaded with the `source` command.
```

## Watchpoints
```markdown
  watch [-location|-l] <expr> [thread <tnum>]
          Create a watchpoint for <expr>, will break if <expr> is written to.
          Watchpoints respect scope of variables, -l can be used to watch the
          memory location instead.
  rwatch ...
          Sets a read watchpoint, will break if <expr> is read from.
  awatch ...
          Sets an access watchpoint, will break if <expr> is written to or read
          from.
```

## Inspection
```markdown
  info functions [<regex>]
          List functions matching <regex>. List all functions if no <regex>
          provided.

  info variables [<regex>]
          List variables matching <regex>. List all variables if no <regex>
          provided.
```

## Signal handling
```markdown
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

## Source file locations
```markdown
  dir <path>
          Add <path> to the beginning of the searh path for source files.

  show dir
          Show current search path.

  set substitute-path <from> <to>
          Add substitution rule checked during source file lookup.

  show substitute-path
          Show current substitution rules.
```

## Configuration

```markdown
  set disassembly-flavor <intel | att>
          Set the disassembly style "flavor".

  set follow-fork-mode <child | parent>
          Specify which process to follow when debuggee makes a fork(2)
          syscall.

  set pagination <on | off>
          Turn on/off gdb's pagination.

  set breakpoint pending <on | off | auto>
          on: always set pending breakpoints.
          off: error when trying to set pending breakpoints.
          auto: interatively query user to set breakpoint.

  set print pretty <on | off>
          Turn on/off pertty printing of structures.

  set logging <on | off>
          Enable output logging to file (default gdb.txt).

  set logging file <fname>
          Change output log file to <fname>

  set logging redirect <on/off>
          on: only log to file.
          off: log to file and tty.
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

## Automatically print next instr
When ever the debugee stops automatically print the memory at the current
instruction pointer (`$rip` x86) and format as instruction `/i`.
```markdown
  # rip - x86
  display /i $rip

  # step instruction, after the step the next instruction is automatically printed
  si
```

## Conditional breakpoints
Create conditional breakpoints for a function `void foo(int i)` in the debugee.
```markdown
  # Create conditional breakpoint
  b foo if i == 42

  b foo     # would create bp 2
  # Make existing breakpoint conditional
  cond 2 if i == 7
```

## Set breakpoint on all threads except one
Create conditional breakpoint using the `$_thread` [convenience
variable][gdb-convenience-vars].
```markdown
  # Create conditional breakpoint on all threads except thread 12.
  b foo if $_thread != 12
```

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

## Hook to automatically save breakpoints on `quit`
```markdown
define break-save
    save breakpoint $arg0.gdb.bp
end
define break-load
    source $arg0.gdb.bp
end

define hook-quit
    break-save quit
end
```

## Watchpoint on struct / class member
A symbolic watchpoint defined on a member variable for debugging is only valid
as long as the expression is in scope. Once out of scope the watchpoint gets
deleted.

When debugging some memory corruption we want to keep the watchpoint even the
expression goes out of scope to find the location that overrides the variable
and introduces the corruption.

```markdown
(gdb) l
1  struct S { int v; };
2
3  void set(struct S* s, int v) {
4      s->v = v;
5  }
6
7  int main() {
8      struct S s;
9      set(&s, 1);
10     set(&s, 2);
11     set(&s, 3);
...

(gdb) s
set (s=0x7fffffffe594, v=1) at test.c:4
4    s->v = v;

# Define a new watchpoint on the member of the struct. The expression however
# is only valid in the current functions scope.

(gdb) watch s->v
Hardware watchpoint 2: s->v

(gdb) c
Hardware watchpoint 2: s->v
Old value = 0
New value = 1
set (s=0x7fffffffe594, v=1) at test.c:5
5   }

# The watchpoint gets deleted as soon as we leave the function scope.

(gdb) c
Watchpoint 2 deleted because the program has left the block in
which its expression is valid.
main () at test.c:10
10      set(&s, 2);

(gdb) p &s->v
$1 = (int *) 0x7fffffffe594

# Define a watchpoint o the address of the member variable of the s instance.
# This of course only makes sense as long as the s instance is not moved in memory.

(gdb) watch *0x7fffffffe594
Hardware watchpoint 3: *0x7fffffffe594

(gdb) c
Hardware watchpoint 3: *0x7fffffffe594
Old value = 1
New value = 2
set (s=0x7fffffffe594, v=2) at test.c:5
5   }

(gdb) c
Hardware watchpoint 3: *0x7fffffffe594
Old value = 2
New value = 3
set (s=0x7fffffffe594, v=3) at test.c:5
5   }
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

[gdb-convenience-vars]: https://sourceware.org/gdb/onlinedocs/gdb/Convenience-Vars.html#Convenience-Vars
