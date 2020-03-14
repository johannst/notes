# lsof(8)

```markdown
lsof
  -a ......... AND slection filters instead ORing (OR: default)
  -p <pid> ... list open file descriptors for process
  +fg ........ show file flags for file descripros
  -n ......... don't convert network addr to hostnames
  -P ......... don't convert network port to know service names
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

Show open files with file flags:
```markdown
lsof +fg -p <pid>
```

Show open tcp connections from user:
```markdown
lsof -a -u $USER -i tcp
```

Show open connections to 'localhost' for user:
```markdown
lsof -a -u $USER -i @localhost
```
