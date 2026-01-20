# pasta(1)

```sh
pasta [opts]
  opts:
    --ipv4-only            Enable only ipv4 operations.
    --outbound-if4 <if>    Bind ipv4 outbound sockets to this network interface.
                           This forces all outbound traffic via that interface.
                           If -i is not specified used this interface to derive
                           ipv4 address and routes.
    --ns-ifname <name>     Name of tap device in network namespace. Default is
                           the same as the one on the host which the ip addresses
                           and routes are derived from.
```

## Example: Pass extra arguments to pasta in podman

This shows an example how to bind outbound traffic to the host interface `wlp123`
and name the tap interface in the container `eth42`.
```
> podman run --rm -it \
    --network pasta:--ipv4-only,--outbound-if4,wlp123,--ns-ifname,eth42 \
    alpine
```

# References

- https://passt.top/passt/about/
