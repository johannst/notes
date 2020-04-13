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

#### Completion with [`_describe`][zsh-comp-utils]
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

#### Completion with [`_arguments`][zsh-comp-utils]
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
        "-s[select sound]:color:(low high)" \
        "-f[select file]:file:_files"       \
        "-d[select dir]:fir:_files -/"      \
        "-h[help]"

    case $state in
        s_color) _foo_color;;
    esac
}
```

[zsh-comp-utils]: http://zsh.sourceforge.net/Doc/Release/Completion-System.html#Completion-Functions
