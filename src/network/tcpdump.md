# tcpdump(1)

# CLI

```markdown
tcpdump [opts] -i <if> [<filter>]
    -n              Don't covert host/port names.
    -w <file|->     Write pcap trace to file or stdout (-).
    -r <file>       Read & parse pcap file.
```

Some useful filters.
```markdown
src <ip>                Filter for source IP.
dst <ip>                Filter for destination IP.
host <ip>               Filter for IP (src + dst).
net <ip>/<range>        Filter traffic on subnet.
[src/dst] port <port>   Filter for port (optionally src/dst).
tcp/udp/icmp            Filter for protocol.
```

> Use `and/or/not` and `()` to build filter expressions.

# Examples

## Capture packets from remote host

```makrdown
# -k: Start capturing immediately.
ssh <host> tcpdump -i <IF> -w - | sudo wireshark -k -i -
```
