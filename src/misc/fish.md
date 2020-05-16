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

### Lists
In `fish` all variables are lists (start with index `1`, but lists can't
contain lists.
```text
set foo a b c d

echo $foo[1]      # a
echo $foo[-1]     # d
echo $foo[2..3]   # b c
echo $foo[1 3]    # a c
```

`$` can be seen as dereference operator.
```text
set foo a; set a 1337
echo $$foo  # outputs 1337
```

Cartesian product.
```text
echo file.{h,cc}
# file.h file.cc

echo {a,b}{1,2}
# a1 b1 b2
```

### Special Variables (Lists)
```text
$status      # exit code of last command
$pipestatus  # list of exit codes of pipe chain

$CMD_DURATION   # runtime of last command in ms
```

#### `*PATH`
Lists ending with `PATH` are automatically split at `:` when used and joined
with `:` when exported to the environment.
```text
set -x BLA_PATH a:b:c:d
echo $BLA_PATH              # a b c d
env | grep BLA_PATH         # BLA_PATH=a:b:c:d
```

## Command Handling
```text
# sub-commands are not run in quotes
echo "ls output: "(ls)
```

### I/O redirection
```text
# 'noclobber', fail if 'log' already exists
echo foo >? log
```

## Control Flow
### `if` / `else`
```text
if grep foo bar
    # do sth
else if grep foobar bar
    # do sth else
else
    # do sth else
end
```

### `switch`
```text
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
```text
while true
    echo foo
end
```

### `for` Loop
```text
for f in (ls)
    echo $f
end
```

## Functions
Function arguments are passed via `$argv` list.
```text
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
```text
functions         # list al functions
functions foo     # describe function 'foo'
functions -e foo  # erase function 'foo'

funced foo        # edit function 'foo'
                  # '-e vim' to edit in vim
```

## Prompt
The prompt is defined by the output of the `fish_prompt` function.
```text
function fish_prompt
    set -l cmd_ret
    echo "> "(pwd) $cmd_ret" "
end
```
> Use `set_color` to manipulate terminal colors.


## Useful Builtins
```text
# history
history search <str>   # search history for <str>
history merge          # merge histories from fish sessions

# list
count $var            # count elements in list

# string
string split SEP STRING
```

## Keymaps
```text
  Shift-Tab ........... tab-completion with search
  Alt-Up / Alt-Down ... search history with token under the cursor
  Alt-l ............... list content of dir under cursor
  Alt-p ............... append '2>&1 | less;' to current cmdline
```

## Debug
```text
  status print-stack-trace .. prints function stacktrace (can be used in scripts)
  breakpoint ................ halt script execution and gives shell (C-d | exit
                              to continue)
```
