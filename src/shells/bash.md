# bash(1)

## Expansion

### Generator

```bash
# generate sequence from n to m
{n..m}
# generate sequence from n to m step by s
{n..m..s}

# expand cartesian product
{a,b}{c,d}
```

### Parameter
```bash
# default value
bar=${foo:-some_val}  # if $foo set, then bar=$foo else bar=some_val

# alternate value
bar=${foo:+bla $foo}  # if $foo set, then bar="bla $foo" else bar=""

# check param set
bar=${foo:?msg}  # if $foo set, then bar=$foo else exit and print msg

# indirect
FOO=foo
BAR=FOO
bar=${!BAR}  # deref value of BAR -> bar=$FOO

# prefix
${foo#prefix}  # remove prefix when expanding $foo
# suffix
${foo%suffix}  # remove suffix when expanding $foo

# substitute
${foo/pattern/string}  # replace pattern with string when expanding foo
# pattern starts with
# '/'   replace all occurences of pattern
# '#'   pattern match at beginning
# '%'   pattern match at end

# set programmatically with priintf builtin
printf -v "VAR1" "abc"
NAME=VAR2
printf -v "$NAME" "%s" "def"
```

> Note: `prefix`/`suffix`/`pattern` are expanded as [pathnames](#pathname).

### Pathname

```bash
*           match any string
?           match any single char
\\          match backslash
[abc]       match any char of 'a' 'b' 'c'
[a-z]       match any char between 'a' - 'z'
[^ab]       negate, match all not 'a' 'b'
[:class:]   match any char in class, available:
              alnum,alpha,ascii,blank,cntrl,digit,graph,lower,
              print,punct,space,upper,word,xdigit
```

With `extglob` shell option enabled it is possible to have more powerful
patterns. In the following `pattern-list` is one ore more patterns separated
by `|` char.

```bash
?(pattern-list)   matches zero or one occurrence of the given patterns
*(pattern-list)   matches zero or more occurrences of the given patterns
+(pattern-list)   matches one or more occurrences of the given patterns
@(pattern-list)   matches one of the given patterns
!(pattern-list)   matches anything except one of the given patterns
```
> Note: `shopt -s extglob`/`shopt -u extglob` to enable/disable `extglob`
> option.

## I/O redirection

> Note: The trick with bash I/O redirection is to interpret from left-to-right.

```bash
# stdout & stderr to file
command >file 2>&1
# equivalent
command &>file

# stderr to stdout & stdout to file
command 2>&1 >file
```
> The article [Bash One-Liners Explained, Part III: All about
> redirections](https://catonmat.net/bash-one-liners-explained-part-three)
> contains some nice visualization to explain bash redirections.

### Explanation

```bash
j>&i
```
Duplicate `fd i` to `fd j`, making `j` a copy of `i`. See [dup2(2)][dup2].

Example:
```bash
command 2>&1 >file
```
1. duplicate `fd 1` to `fd 2`, effectively redirecting `stderr` to `stdout`
2. redirect `stdout` to `file`

## Process substitution ([ref][psub])

Process substitution allows to redirect the stdout of multiple processes at
once.
```bash
vim -d <(grep foo bar) <(grep foo moose)
```

## Command grouping

Execute commands in a group with or without subshell. Can be used to easily
redirect stdout/stderr of all commands in the group into one file.
```bash
# Group commands without subshell.
v=abc ; { v=foo; echo $v; } ; echo $v
# foo
# foo

# Group commands with subshell.
v=abc ; ( v=foo; echo $v; ) ; echo $v
# foo
# abc
```

## Argument parsing with `getopts`

The `getopts` builtin uses following global variables:
- `OPTARG`, value of last option argument
- `OPTIND`, index of the next argument to process (user must reset)
- `OPTERR`, display errors if set to `1`

```bash
getopts <optstring> <param> [<args>]
```
- `<optstring>` specifies the names of supported options, eg `f:c`
  - `f:` means `-f` option with an argument
  - `c` means `-c` option without an argument
- `<param>` specifies a variable name which `getopts` fills with the last parsed option argument
- `<args>` optionally specify argument string to parse, by default `getopts` parses `$@`

### Example
```bash
#!/bin/bash
function parse_args() {
    while getopts "f:c" PARAM; do
        case $PARAM in
            f) echo "GOT -f $OPTARG";;
            c) echo "GOT -c";;
            *) echo "ERR: print usage"; exit 1;;
        esac
    done
    # users responsibility to reset OPTIND
    OPTIND=1
}

parse_args -f xxx -c
parse_args -f yyy
```

## Regular Expressions

Bash supports regular expression matching with the binary operator `=~`.
The match results can be accessed via the `$BASH_REMATCH` variable:
- `${BASH_REMATCH[0]}` contains the full match
- `${BASH_REMATCH[1]}` contains match of the first capture group

```bash
INPUT='title foo : 1234'
REGEX='^title (.+) : ([0-9]+)$'
if [[ $INPUT =~ $REGEX ]]; then
    echo "${BASH_REMATCH[0]}"    # title foo : 1234
    echo "${BASH_REMATCH[1]}"    # foo
    echo "${BASH_REMATCH[2]}"    # 1234
fi
```
> **Caution**: When specifying a `regex` in the `[[ ]]` block directly, quotes will be treated as part of the pattern.
> `[[ $INPUT =~ "foo" ]]` will match against `"foo"` not `foo`!

## Completion

The `complete` builtin is used to interact with the completion system.
```bash
complete                    # print currently installed completion handler
complete -F <func> <cmd>    # install <func> as completion handler for <cmd>
complete -r <cmd>           # uninstall completion handler for <cmd>
```

Variables available in completion functions:
```bash
# in
$1              # <cmd>
$2              # current word
$3              # privous word

COMP_WORDS      # array with current command line words
COMP_CWORD      # index into COMP_WORDS with current cursor position

# out
COMPREPLY       # array with possible completions
```

The `compgen` builtin is used to generate possible matches by comparing `word`
against words generated by `option`.
```bash
compgen <option> <word>

# usefule options:
# -W <list>    specify list of possible completions
# -d           generate list with dirs
# -f           generate list with files
# -u           generate list with users
# -e           generate list with exported variables

# compare "f" against words "foo" "foobar" "bar" and generate matches
compgen -W "foo foobar bar" "f"

# compare "hom" against file/dir names and generate matches
compgen -d -f "hom"
```

### Example
Skeleton to copy/paste for writing simple completions.

Assume a program `foo` with the following interface:
```bash
foo -c green|red|blue -s low|high -f <file> -h
```

The completion handler could be implemented as follows:
```bash
function _foo() {
    local curr=$2
    local prev=$3

    local opts="-c -s -f -h"
    case $prev in
        -c) COMPREPLY=( $(compgen -W "green red blue" -- $curr) );;
        -s) COMPREPLY=( $(compgen -W "low high" -- $curr) );;
        -f) COMPREPLY=( $(compgen -f -- $curr) );;
        *)  COMPREPLY=( $(compgen -W "$opts" -- $curr) );;
    esac
}

complete -F _foo foo
```

[dup2]: http://man7.org/linux/man-pages/man2/dup.2.html
[psub]: https://tldp.org/LDP/abs/html/process-sub.html
