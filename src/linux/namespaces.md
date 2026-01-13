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

### Example: Move ethernet interface into network namespace
The creates a network namespace called `apple` and moves the ethernet
interface `eth0` into the network namespace.
```sh
# Create a new named network namespace, by convention the network ns file is
# located at /var/run/netns/apple.
ip netns add apple

# Move the interface eth0 into the network namespace.
ip link set dev eth0 netns apple

# Run 'ip addr' in the network namespace.
ip -n apple addr

# Eg, manually configure the interface in the namespace.
ip -n apple addr add 10.10.10.123/24 dev eth0
ip -n apple route add default via 10.10.10.1 dev eth0
ip -n apple link set dev eth0 up

# Execute a command in the network namespace.
ip netns exec apple firefox

# ip-netns(8) is considered a network namespace aware application.
# Such an application follows the convention to look for global configuration
# files in /etc/netns/<name>/ and then in /etc. This is done by ip-netns, by
# also unsharing the mount namespace and bind mounting the files in
# /etc/netns/<name> onto /etc.
echo 'nameserver 9.9.9.9' > /etc/netns/apple/resolv.conf
cat /etc/resolv.conf
# nameserver '1.2.3.4'
ip netns exec apple cat /etc/resolv.conf
# nameserver '9.9.9.9'

# Kill all processes in the network namespace.
ip netns pids apple | xargs kill
# Delete the network namespace.
ip netns del apple
```

The unsharing of the *network* and *mount* namespace with `ip-netns(8)` can be
seen in the example below.

```sh
ip netns exec apple bash -c 'echo $BASHPID; sleep 60' &
# 1689

lsns -p 1689
#         NS TYPE   NPROCS   PID USER COMMAND
# 4026531834 time      175     1 root /usr/lib/systemd/systemd --switched-root --s
# 4026531835 cgroup    175     1 root /usr/lib/systemd/systemd --switched-root --s
# 4026531836 pid       175     1 root /usr/lib/systemd/systemd --switched-root --s
# 4026531837 user      173     1 root /usr/lib/systemd/systemd --switched-root --s
# 4026531838 uts       171     1 root /usr/lib/systemd/systemd --switched-root --s
# 4026531839 ipc       175     1 root /usr/lib/systemd/systemd --switched-root --s
# 4026532622 mnt         1  1689 root sleep 60
# 4026532674 net         1  1689 root sleep 60
```

### Example: Move wireless interface into network namespace
Similar to the above example, wireless interfaces can be moved into a network
namespace. However, wireless interfaces are a bit special as the whole physical
device has to be moved.

```sh
# Find phy for given wireless interface 'wlan0'.
iw dev wlan0 info | grep phy
# wiphy 0

# Check if this phy supports network namespaces.
iw phy phy0 info | grep netns
# * set_wiphy_netns
```

The following moves the `wlan0` wireless interface with its physical device
(radio) `phy0` into a network namespace.

```sh
# Create network namespace.
ip netns add moose

# Move wireless radio into network namespace.
iw phy phy0 set netns name moose

# Ensure wireless radio is not blocked.
ip netns exec moose rfkill ...

# Create wpa config for password protected ssids.
# Provide password on stdin.
wpa_passphrase "<ssid>" > ssid_wpa.conf

# Start wpa supplicant in the network namespace.
ip netns exec moose wpa_supplicant -B -i wlan0 -c ssid_wpa.conf

# Then manually configure and bring up the wireless network in the network
# namespace ..
ip -n moose addr add 10.10.10.123/24 dev wlan0
ip -n moose route add default via 10.10.10.1 dev wlan0
ip -n moose link set dev wlan0 up
# .. or dynamically with a dhcp client for example.
ip netns exec moose dhclient wlan0

# One can run some commands now in the network namespace which isolates the
# wireless interface.
ip netns exec moose firefox

# Kill all processes in the network namespace and delete the network ns.
ip netns pids moose | xargs kill
ip netns del moose
```
