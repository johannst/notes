# ss(8)

```markdown
ss [option] [filter]
```

```markdown
[option]
  -p ..... Show process using socket
  -l ..... Show sockets in listening state
  -4/-6 .. Show IPv4/6 sockets
  -x ..... Show unix sockets
  -n ..... Show numeric ports (no resolve)
  -O ..... Oneline output per socket
```

```markdown
[filter]
  dport/sport PORT .... Filter for destination/source port
  dst/src ADDR ........ Filter for destination/source address

  and/or .............. Logic operator
  ==/!= ............... Comparison operator

  (EXPR) .............. Group exprs
```

# Examples

Show all tcp IPv4 sockets connecting to port `443`:
```markdown
ss -4 'dport 443'
```

Show all tcp IPv4 sockets that don't connect to port `443` or connect to address `1.2.3.4`.
```markdown
ss -4 'dport != 443 or dst 1.2.3.4'
```

