# lsof(8)

```markdown
lsof
  -r <s> ..... repeatedly execute command ervery <s> seconds
  -a ......... AND slection filters instead ORing (OR: default)
  -p <pid> ... filter by <pid>
  +fg ........ show file flags for file descripros
  -n ......... don't convert network addr to hostnames
  -P ......... don't convert network port to service names
  -i <@h[:p]>. show connections to h (hostname|ip addr) with optional port p
  -s <p:s> ... in conjunction with '-i' filter for protocol <p> in state <s>
  -U ......... show unix domain sockets ('@' indicates abstract sock name, see unix(7))
```

```markdown
file flags:
  R/W/RW ..... read/write/read-write
  CR ......... create
  AP ......... append
  TR ......... truncate
```

```markdown
-s protocols
  TCP, UDP

-s states (TCP)
  CLOSED, IDLE, BOUND, LISTEN, ESTABLISHED, SYN_SENT, SYN_RCDV, ESTABLISHED,
  CLOSE_WAIT, FIN_WAIT1, CLOSING, LAST_ACK, FIN_WAIT_2, TIME_WAIT

-s states (UDP)
  Unbound, Idle
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
lsof -a -u $USER -i TCP
```
**Note**: `-a` _ands_ the results. If `-a` is not given all open files matching
`$USER` and all tcp connections are listed (_ored_).

## Open connection to specific host
Show open connections to `localhost` for `$USER`:
```markdown
lsof -a -u $USER -i @localhost
```

## IPv4 TCP connections in `ESTABLISHED` state
```markdown
lsof -i 4TCP -s TCP:ESTABLISHED
```
