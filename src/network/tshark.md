# tshark (1)

```text
tshark [opts] -i <if>
    --color         Colorize output.
    -w <file|->     Write pcap trace to file or stdout (-).
    -r <file>       Read & parse pcap file.
    -f <filter>     Apply capture filter (see pcap-filter(7) or tcpdump).
                    Only applicable during capturing.
    -Y <filter>     Apply display filter.
                    Only applicable during viewing capture.
    -c <count>      Stop capturing after COUNT packets (INF by default).
```

Some useful display filters.
```text
ip.addr != 192.168.1.0/24      Filter out whole ip subnet (source + destination).
ip.dst == 192.168.1.42         Filter for destination ip address.
tcp.dstport == 80              Filter for tcp destinatio port.
!wg                            Filter out all wireguard traffic.

tcp/udp/ssh/wg/...             Filter for protocol.

"and/or/not/!" and "()" can be used to build filter expressions.
```
> Use `tshak -G` to list all fields that can be used in display filters.

# Examples

## Capture and filter packet to file
```bash
# Capture TCP traffic with port 80 on interface eth0 to file.
sudo tshark -i eht0 -f 'tcp and port 80' -w tx.pcap

# View captured packets.
sudo tshark -r tx.pcap

# View captured packets and apply additionaly display filters.
sudo tshark -r tx.pcap -Y 'ip.addr != 192.168.1.42'
```
