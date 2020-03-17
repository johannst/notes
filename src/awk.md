# awk(1)

```markdown
awk [opt] program [input]
    -F <sepstr>        field separator string (can be regex)
    program            awk program
    input              file or stdin if not file given
```

## Input processing

Input is processed in two stages:
1. Splitting input into a sequence of `records`.
   By default split at `newline` character, but can be changed via the
   builtin `RS` variable.
2. Splitting a `record` into `fields`. By default strings without `whitespace`,
   but can be changed via the builtin variable `FS` or command line option
   `-F`.

Field are accessed as follows:
- `$0` whole `record`
- `$1` field one
- `$2` field two
- ...

## Program

An `awk` program is composed of pairs of the form:
```markdown
pattern { action }
```
The program is run against each `record` in the input stream. If a `pattern`
matches a `record` the corresponding `action` is executed and can access the
`fields`.

```markdown
INPUT
  |
  v
record ----> ∀ pattern matched
  |                   |
  v                   v
fields ----> run associated action
```
