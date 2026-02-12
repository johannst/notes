# fish(1)

## Quick Info
Fish initialization file `~/.config/fish/config.fish`

Switch between different key bindings:
- `fish_default_key_bindings` to use default key bindings
- `fish_vi_key_bindings` to use vi key bindings

## Variables
Available scopes
- `local` variable local to a block
- `global` variable global to shell instance
- `universal` variable universal to all shell instances + preserved across
  shell restart

### Set/Unset Variables
```text
set <name> [<values>]
    -l  local scope
    -g  global scope
    -U  universal scope
    -e  erase variable
    -S  show verbose info
    -x  export to ENV
    -u  unexport from ENV
```

### Special Variables ([ref](https://fishshell.com/docs/current/language.html#special-variables))
```sh
$status      # exit code of last command
$pipestatus  # list of exit codes of pipe chain

$fish_pid    # pid of parent fish shell    ($$ in bash)
$last_pid    # pid of last started process ($! in bash)

$CMD_DURATION   # runtime of last command in ms
```

### Lists
In `fish` all variables are lists (start with index `1`, but lists can't
contain lists.
```sh
set foo a b c d

echo $foo[1]      # a
echo $foo[-1]     # d
echo $foo[2..3]   # b c
echo $foo[1 3]    # a c
```

`$` can be seen as dereference operator.
```sh
set foo a; set a 1337
echo $$foo  # outputs 1337
```

Cartesian product.
```sh
echo file.{h,cc}
# file.h file.cc

echo {a,b}{1,2}
# a1 b1 a2 b2
```

#### `*PATH` variables ([ref](https://fishshell.com/docs/current/language.html#path-variables))
Lists ending with `PATH` are automatically split at `:` when used and joined
with `:` when quoted or exported to the environment.
```sh
set -x BLA_PATH a:b:c:d
echo $BLA_PATH           # a b c d
echo "$BLA_PATH"         # a:b:c:d          (quoted)
env | grep BLA_PATH      # BLA_PATH=a:b:c:d (env)

set FOO_PATH x y z
echo $FOO_PATH           # x y z
echo "$FOO_PATH"         # x:y:z
```

## Abbreviations ([ref](https://fishshell.com/docs/current/interactive.html#abbreviations))

Abbreviations are like aliases however they expand while typing.
```sh
# Show currently defined abbreviation.
abbr

# When typing "ec" <space> this will expand to emacsclient.
abbr ec emacsclient

# Only expand abbreviation when it appears in a position where a command is expected.
abbr rg --position command rg --no-ignore --hidden

# Place the cursor position when the abbreviation is expanded. The cursor is
# placed on the first occurrence of the given char and the char is deleted.
abbr e2 --set-cursor=, echo a , c
# Default char is '%'.
abbr e1 --set-cursor echo a % c
```

Following is an example to quickly lookup syscalls in a kernel source tree.
```
abbr sys --position command --set-cursor=, rg -A3 "'SYSCALL_DEFINE.?\(,[,)]'" ~/dev/oss/linux
```

## Command Handling
```sh
# sub-commands are not run in quotes
echo "ls output: "(ls)
```

### I/O redirection
```sh
# 'noclobber', fail if 'log' already exists
echo foo >? log

# redirect stdout+stderr to pipe
cmd &| grep abc
```

### Process substitution
Redirect output of multiple processes. Same as `<(..)` in bash.
```sh
diff (sort a | psub) (sort b | psub)
```

## Control Flow
### `if` / `else`
```sh
if grep foo bar
    # do sth
else if grep foobar bar
    # do sth else
else
    # do sth else
end
```

### `switch`
```sh
switch (echo foo)
case 'foo*'
    # do start with foo
case bar dudel
    # do bar and dudel
case '*'
    # do else
end
```

### `while` Loop
```sh
while true
    echo foo
end
```

### `for` Loop
```sh
for f in (ls)
    echo $f
end
```

## Functions
Function arguments are passed via `$argv` list.
```sh
function fn_foo
    echo $argv
end
```

### Autoloading
When running a command fish attempts to autoload a function. The shell looks
for `<cmd>.fish` in the locations defined by `$fish_function_path` and loads
the function lazily if found.

This is the preferred way over monolithically defining all functions in a
startup script.

### Helper
```sh
functions         # list al functions
functions foo     # describe function 'foo'
functions -e foo  # erase function 'foo'

funced foo        # edit function 'foo'
                  # '-e vim' to edit in vim
```

### Argument parsing and completion
`argparse` puts options into variables of name `_flag_NAME`.

References:
- [Argument Handling](https://fishshell.com/docs/current/language.html#argument-handling)
- [`argparse`](https://fishshell.com/docs/current/cmds/argparse.html)
- [Writing your own completions](https://fishshell.com/docs/current/completions.html)
- [`complete`](https://fishshell.com/docs/current/cmds/complete.html)

```sh
function moose --d "my moose fn"
    # h/help   : short / long option (boolean)
    # color    : only long option (boolean)
    # u/user=  : option with required argument, only last specified is taken
    # f/file+= : option with required argument, can be specified multiple times
    #
    argparse h/help color= u/user= f/file=+ -- $argv
    or return

    if set -ql _flag_help
        echo "usage ..."
        return 0
    end

    set -ql _flag_file
    and echo "file=$_flag_file | cnt:" (count $_flag_file)

    set -ql _flag_color
    and echo "color=$_flag_color"

    set -ql _flag_user
    and echo "user=$_flag_user"
end

# Delete all previous defined completions for 'moose'.
complete -c moose -e

# Don't complete files for command.
complete -c moose --no-files

# Help completion.
#   -n specifies a conditions. The completion is only active if the command
#      returns 0.
complete -c moose -s h -l help -n "not __fish_contains_opt -s h help" \
    --description "Print usage help and exit"

# File completion.
#   -F force complete files (overwrite --no-files).
#   -r requires argument.
complete -c moose -s f -l file -F -r \
    --description "Specify file (can be passed multiple times)"

# Color completion.
#    -a options for completion.
#    -x short for -r and --no-files (-f)
complete -c moose -x -l color -a "red blue" \
    --description "Specify a color."

# User completion.
#    -a options for completion. Call a function to generate arguments.
complete -c moose -x -s u -l user -a "(__fish_complete_users)" \
    --description "Specify a user"
```

### Example: add_tool

Sometimes there is need to add tools from a custom install location into the
shell environment. The following gives a convenience wrapper (starting point)
which sets up additional variables besides the path to the binary.
```sh
function add_tool
    set tool $argv[1]

    # Setup PATH.
    fish_add_path --path $tool/bin

    # Setup MANPATH.
    test -d $tool/share/man
    and ! contains $tool/share/man $MANPATH
    and if not set -q MANPATH
        # Add trailing ':' on MANATH to also include default paths,
        # else man -k or apropos dont use the default path.
        set -g -x MANPATH $tool/share/man:
    else
        # Prepend new path.
        set -p MANPATH $tool/share/man
    end
    or :

    # Setup INFOPATH.
    test -d $tool/share/info
    and ! contains $tool/share/info $INFOPATH
    and if not set -q INFOPATH
        set -g -x INFOPATH $tool/share/info:
    else
        # Add trailing ':' on INFOPATH for emacs info system. That way emacs
        # also adds its default directories.
        # https://github.com/emacs-mirror/emacs/blob/c091ff00ec45821b911a8fd83c4b672b9a6e4feb/lisp/info.el#L182-L186
        # https://github.com/emacs-mirror/emacs/blob/c091ff00ec45821b911a8fd83c4b672b9a6e4feb/lisp/info.el#L761-L763
        set -p INFOPATH $tool/share/info
    end
    or :
end
```

## Prompt
The prompt is defined by the output of the `fish_prompt` function.
```sh
function fish_prompt
    set -l cmd_ret
    echo "> "(pwd) $cmd_ret" "
end
```
> Use `set_color` to manipulate terminal colors and `set_color -c` to print the
> current colors.


## Useful Builtins
List all builtins with `builtins -n`.

```sh
# history
history search <str>   # search history for <str>
history merge          # merge histories from fish sessions

# list
count $var             # count elements in list

contains /bin $PATH    # return 0 (true) 1 (false)
contains -i /bin $PATH # additionally print index on stdout

# string
string split SEP STRING

# math
math -b hex 4096       # output dec as hex
math 0x1000            # output hex as dec
math "log2(1024)"      # call functions
math -s0 7/3           # integer division (by default float)

# status
status -f              # abs path of current file
```

## Keymaps
```text
  Shift-Tab .............. tab-completion with search
  Alt-Up / Alt-Down ...... search history with token under the cursor
  Alt-l .................. list content of dir under cursor
  Alt-p .................. append '2>&1 | less;' to current cmdline
  Alt-Left / Alt - Right . prevd / nextd, walk dir history
```

## Debug
```text
  status print-stack-trace .. prints function stacktrace (can be used in scripts)
  breakpoint ................ halt script execution and gives shell (C-d | exit
                              to continue)
```
