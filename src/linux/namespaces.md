# namespaces(7)

## List namespaces
```sh
# List all namespaces.
lsns
# List all namespaces of specific type.
lsns -t net
lsns -t pid
```

## Enter namespaces
```sh
 # Enter all namespaces of a pid.
 nsenter -t <pid> --all
 # Enter specific namespaces of a pid.
 nsenter -t <pid> --net
 nsenter -t <pid> --pid --mount
```

### Example: Enter podman container
Running `podman exec -it webserver /bin/sh` is basically equivalent to the
following.
```sh
sudo nsenter -t (podman inspect -f '{{.State.Pid}}' webserver) --all /bin/sh
```
