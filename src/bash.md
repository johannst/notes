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
# if $foo set, then bar=$foo else bar=some_val
bar=${foo:-some_val}
```

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

[dup2]: http://man7.org/linux/man-pages/man2/dup.2.html
