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
# default param
bar=${foo:-some_val}  # if $foo set, then bar=$foo else bar=some_val

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

Wit `extglob` shell option enabled it is possible to have more powerful
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

`compgen` builtin is used to generate possible matches for `word` out of possible `option`s.
The syntax is as follows:
```bash
compgen [option] [word]

# usefule options:

# -W <list>    compare against word-list
compgen -W "foo foobar bar" "f"

# -d    compare against dir names
# -f    compare against file names
compgen -d -f "hom"
```

[dup2]: http://man7.org/linux/man-pages/man2/dup.2.html
