# zsh(1)

## Keybindings

Change input mode:
```zsh
bindkey -v              change to vi keymap
bindkey -e              change to emacs keymap
```

Define key-mappings:
```zsh
bindkey                 list mappings in current keymap
bindkey in-str cmd      create mapping for `in-str` to `cmd`
bindkey -r in-str       remove binding for `in-str`

# C-v <key>             dump <key> code, which can be used in `in-str`
# zle -l                list all functions for keybindings
# man zshzle(1)         STANDARD WIDGETS: get description of functions
```

Access edit buffer in zle widget:
```zsh
$BUFFER       # Entire edit buffer content
$LBUFFER      # Edit buffer content left to cursor
$RBUFFER      # Edit buffer content right to cursor

# create zle widget which adds text right of the cursor
function add-text() {
    RBUFFER="some text $RBUFFER"
}
zle -N add-text

bindkey "^p" add-text
```

## Parameter

Default value:
```zsh
# default value
echo ${foo:-defval}   # defval
foo=bar
echo ${foo:-defval}   # bar
```

Alternative value:
```zsh
echo ${foo:+altval}   # ''
foo=bar
echo ${foo:+altval}   # altval
```

Check variable set, error if not set:
```zsh
echo ${foo:?msg}      # print `msg` and return errno `1`
foo=bar
echo ${foo:?msg}      # bar
```

Sub-string `${var:offset:length}`:
```zsh
foo=abcdef
echo ${foo:1:3}       # bcd
```

Trim prefix `${var#prefix}`:
```zsh
foo=bar.baz
echo ${foo#bar}       # .baz
```

Trim suffix `${var%suffix}`:
```zsh
foo=bar.baz
echo ${foo%.baz}      # bar
```

Substitute pattern `${var/pattern/replace}`:
```zsh
foo=aabbccbbdd
echo ${foo/bb/XX}    # aaXXccbbdd
echo ${foo//bb/XX}   # aaXXccXXdd
# replace prefix
echo ${foo/#bb/XX}   # aabbccbbdd
echo ${foo/#aa/XX}   # XXbbccbbdd
# replace suffix
echo ${foo/%bb/XX}   # aabbccbbdd
echo ${foo/%dd/XX}   # aabbccbbXX
```

> Note: `prefix`/`suffix`/`pattern` are expanded as pathnames.

## Variables

```zsh
# Variable with local scope
local var=val

# Read-only variable
readonly var=bal
```

Indexed arrays:
```zsh
arr=(aa bb cc dd)
echo $arr[1]           # aa
echo $arr[-1]          # dd

arr+=(ee)
echo $arr[-1]          # ee

echo $arr[1,3]         # aa bb cc
```

Associative arrays:
```zsh
typeset -A arr
arr[x]='aa'
arr[y]='bb'
echo $arr[x]           # aa
```

Tied arrays:
```zsh
typeset -T VEC vec=(1 2 3) '|'

echo $vec              # 1 2 3
echo $VEC              # 1|2|3
```

Unique arrays (set):
```
typeset -U vec=(1 2 3)

echo $vec             # 1 2 3
vec+=(1 2 4)
echo $vec             # 1 2 3 4
```

### Expansion Flags

Join array to string `j:sep:`:
```zsh
foo=(1 2 3 4)
echo ${(j:-:)foo}     # 1-2-3-4
echo ${(j:\n:)foo}    # join with new lines
```

Split string to array `s:sep`:
```zsh
foo='1-2-3-4'
bar=(${(s:-:)foo})    # capture as array
echo $bar             # 1 2 3 4
echo $bar[2]          # 2
```

Upper/Lower case string:
```zsh
foo=aaBB
echo ${(L)foo}        # aabb
echo ${(U)foo}        # AABB
```

## Argument parsing with `zparseopts`

```zsh
zparseopts [-D] [-E] [-A assoc] specs
```
Arguments are copied into the associative array `assoc` according to `specs`.
Each spec is described by an entry as `opt[:][=array]`.
- `opt` is the option without the `-` char. Passing `-f` is matched against `f`
  opt, `--long` is matched against `-long`.
- Using `:` means the option will take an argument.
- The optional `=array` specifies an alternate storage container where this
  option should be stored.
> Documentation can be found in `man zshmodules`.

### Example
```zsh
#!/bin/zsh
function test() {
    zparseopts -D -E -A opts f=flag o: -long:
    echo "flag $flag"
    echo "o    $opts[-o]"
    echo "long $opts[--long]"
    echo "pos  $1"
}

test -f -o OPTION --long LONG_OPT POSITIONAL

# Outputs:
#   flag -f
#   o    OPTION
#   long LONG_OPT
#   pos  POSITIONAL
```

## Regular Expressions

Zsh supports regular expression matching with the binary operator `=~`.
The match results can be accessed via the `$MATCH` variable and
`$match` indexed array:
- `$MATCH` contains the full match
- `$match[1]` contains match of the first capture group

```zsh
INPUT='title foo : 1234'
REGEX='^title (.+) : ([0-9]+)$'
if [[ $INPUT =~ $REGEX ]]; then
    echo "$MATCH"       # title foo : 1234
    echo "$match[1]"    # foo
    echo "$match[2]"    # 1234
fi
```

## Completion

### Installation

Completion functions are provided via files and need to be placed in a location
covered by `$fpath`. By convention the completion files are names as `_<CMD>`.

A completion skeleton for the command `foo`, stored in `_foo`
```zsh
#compdef _foo foo

function _foo() {
    ...
}
```

Alternatively one can install a completion function explicitly by calling `compdef <FUNC> <CMD>`.

### Completion Variables

Following variables are available in Completion functions:
```zsh
$words              # array with command line in words
$#words             # number words
$CURRENT            # index into $words for cursor position
$words[CURRENT-1]   # previous word (relative to cursor position)
```

### Completion Functions
- `_describe`     simple completion, just words + description
- `_arguments`    sophisticated completion, allow to specify actions

#### Completion with [`_describe`][zsh-comp-fn]
```zsh
_describe MSG COMP
```
- `MSG` simple string with header message
- `COMP` array of completions where each entry is `"opt:description"`

```zsh
function _foo() {
    local -a opts
    opts=('bla:desc for bla' 'blu:desc for blu')
    _describe 'foo-msg' opts
}
compdef _foo foo

foo <TAB><TAB>
 -- foo-msg --
bla  -- desc for bla
blu  -- desc for blu
```

#### Completion with [`_arguments`][zsh-comp-fn]
```zsh
_arguments SPEC [SPEC...]
```
where `SPEC` can have one of the following forms:
- `OPT[DESC]:MSG:ACTION`
- `N:MSG:ACTION`

Available actions
```zsh
(op1 op2)   list possible matches
->VAL       set $state=VAL and continue, `$state` can be checked later in switch case
FUNC        call func to generate matches
{STR}       evaluate `STR` to generate matches
```

### Example
Skeleton to copy/paste for writing simple completions.

Assume a program `foo` with the following interface:
```zsh
foo -c green|red|blue -s low|high -f <file> -d <dir> -h
```

The completion handler could be implemented as follows in a file called `_foo`:
```zsh
#compdef _foo foo

function _foo_color() {
    local colors=()
    colors+=('green:green color')
    colors+=('red:red color')
    colors+=('blue:blue color')
    _describe "color" colors
}

function _foo() {
    _arguments                              \
        "-c[define color]:color:->s_color"  \
        "-s[select sound]:sound:(low high)" \
        "-f[select file]:file:_files"       \
        "-d[select dir]:dir:_files -/"      \
        "-h[help]"

    case $state in
        s_color) _foo_color;;
    esac
}
```

> `_files` is a zsh builtin utility function to complete files/dirs see
> - [zsh completion functions][zsh-comp-fn]
> - [zsh completion utility functions][zsh-comp-utility-fn]


[zsh-comp-fn]: http://zsh.sourceforge.net/Doc/Release/Completion-System.html#Completion-Functions
[zsh-comp-utility-fn]: https://github.com/zsh-users/zsh-completions/blob/master/zsh-completions-howto.org#utility-functions
