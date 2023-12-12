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

Fields are accessed as follows:
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

Any valid awk `expr` can be a `pattern`.

### Special pattern

awk provides two special patterns, `BEGIN` and `END`, which can be used
multiple times. Actions with those patterns are **executed exactly once**.
- `BEGIN` actions are run before processing the first record
- `END` actions are run after processing the last record

### Special variables

- `RS` _record separator_: first char is the record separator, by default
  <newline>
- `FS` _field separator_: regex to split records into fields, by default
  <space>
- `NR` _number record_: number of current record
- `NF` _number fields_: number of fields in the current record

### Special statements & functions

- `printf "fmt", args...`

  Print format string, args are comma separated.
  - `%s` string
  - `%d` decimal
  - `%x` hex
  - `%f` float

  Width can be specified as `%Ns`, this reserves `N` chars for a string.
  For floats one can use `%N.Mf`, `N` is the total number including `.` and
  `M`.

- `sprintf("fmt", expr, ...)`

    Format the expressions according to the format string. Similar as `printf`,
    but this is a function and return value can be assigned to a variable.

- `strftime("fmt")`

  Print time stamp formatted by `fmt`.
  - `%Y` full year (eg 2020)
  - `%m` month (01-12)
  - `%d` day (01-31)
  - `%F` alias for `%Y-%m-%d`
  - `%H` hour (00-23)
  - `%M` minute (00-59)
  - `%S` second (00-59)
  - `%T` alias for `%H:%M:%S`


## Examples

### Filter records
```bash
awk 'NR%2 == 0 { print $0 }' <file>
```
The pattern `NR%2 == 0` matches every second record and the action `{ print $0 }`
prints the whole record.

### Negative patterns
```bash
awk '!/^#/ { print $1 }' <file>
```
Matches records not starting with `#`.

### Range patterns
```bash
echo -e "a\nFOO\nb\nc\nBAR\nd" | awk '/FOO/,/BAR/ { print }'
```
`/FOO/,/BAR/` define a range pattern of `begin_pattern, end_pattern`. When
`begin_pattern` is matched the range is **turned on** and when the
`end_pattern` is matched the range is **turned off**. This matches every record
in the range _inclusive_.

An _exclusive_ range must be handled explicitly, for example as follows.
```bash
echo -e "a\nFOO\nb\nc\nBAR\nd" | \
      awk '/FOO/,/BAR/ { if (!($1 ~ "FOO") && !($1 ~ "BAR")) { print } }'
```

### Access last fields in records
```bash
echo 'a b c d e f' | awk '{ print $NF $(NF-1) }'
```
Access last fields with arithmetic on the `NF` number of fields variable.

### Capture in variables
```bash
# /proc/<pid>/status
#   Name:    cat
#   ...
#   VmRSS:   516 kB
#   ...

for f in /proc/*/status; do
    cat $f | awk '
             /^VmRSS/ { rss = $2/1024 }
             /^Name/ { name = $2 }
             END { printf "%16s %6d MB\n", name, rss }';
done | sort -k2 -n
```
We capture values from `VmRSS` and `Name` into variables and print them at the
`END` once processing all records is done.

### Capture in array
```bash
echo 'a 10
b 2
b 4
a 1' | awk '{
    vals[$1] += $2
    cnts[$1] += 1
}
END {
    for (v in vals)
        printf "%s %d\n", v, vals[v] / cnts [v]
}'
```
Capture keys and values from different columns and some up the values.
At the `END` we compute the average of each key.

### Run shell command and capture output
```bash
cat /proc/1/status | awk '
                     /^Pid/ {
                        "ps --no-header -o user " $2 | getline user;
                         print user
                     }'
```
We build a `ps` command line and capture the first line of the processes output
in the `user` variable and then print it.
