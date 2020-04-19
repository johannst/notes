# lsof(8)

```markdown
lsof
  -a ......... AND slection filters instead ORing (OR: default)
  -p <pid> ... filter by <pid>
  +fg ........ show file flags for file descripros
  -n ......... don't convert network addr to hostnames
  -P ......... don't convert network port to service names
  -i <@h[:p]>. show connections to h (hostname|ip addr) with optional port p
```

```markdown
file flags:
  R/W/RW ..... read/write/read-write
  CR ......... create
  AP ......... append
  TR ......... truncate
```

# Examples

## File flags
Show open files with file flags for process:
```markdown
lsof +fg -p <pid>
```
## Open TCP connections
Show open tcp connections for `$USER`:
```markdown
lsof -a -u $USER -i tcp
```
**Note**: `-a` _ands_ the results. If `-a` is not given all open files matching
`$USER` and all tcp connections are listed (_ored_).

## Open connection to specific host
Show open connections to `localhost` for `$USER`:
```markdown
lsof -a -u $USER -i @localhost
```
