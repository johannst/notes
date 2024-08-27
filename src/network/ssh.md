# ssh (1)

## ssh tunnel
Abbreviations used:
- `LPORT`: local port
- `LADDR`: local address
- `RPORT`: remote port
- `RADDR`: remote address

The `-L` flag sets up a ssh tunnel to forward port `LPORT` on the local host to
`RADDR:RPORT` via the machine `gateway` (ssh tunnel endpoint).

```sh
# Forward local port to remote port on gateway.
ssh -L LPORT:RPORT gateway

# Forward local port to remote port on remote address via gateway.
ssh -L LPORT:RADDR:RPORT gateway
```

In this scenario, requests are issued on the local machine and target some
remote resource, effectively making a remote resource accessible on the local
machine, which may be hidden behind the tunnel endpoint (gateway).

The `-R` flag sets up a ssh tunnel to expose the local port `LPORT` as `RPORT`
on the remote machine `gateway`.

```sh
# Expose local port via remote port on gateway.
ssh -R RPORT:LPORT gateway

# Expose local port of machine with local address via remote port on gateway.
ssh -R RPORT:LADDR:LPORT gateway
```

In this scenario, requests are issued on the gateway and target some resource
in the local network, effectively exposing the local resource on the remote
machine (gateway).

The trick to memorize the syntax is to read the forwarding rules left
(source) to right (destination) while `-L` means that requests are issued
locally and `-R` means that requests are issued remotely.

The following flags are useful for setting up ssh tunnels:
- `-N` just stop before running the command on the remote side (w/o cmd dont
  drop into shell)
- `-f` run `ssh` command in the background


### Example
```sh
# Forward requests on localhost:8080 to moose:1234 and keep ssh in forground
# but dont drop into a shell.
ssh -N -L 8080:1234 moose

# Forward requests on moose:80 to localhost:8080 and keep ssh in forground but
# dont drop into a shell.
ssh -N -R 80:8080 moose
```

## ssh keys
Utility script to generate ssh key pairs.
```sh
NAME=${1:?Pass new keyname as first arg}

TYPE=ed25519
FILE=${HOME}/.ssh/${NAME}-${TYPE}

if [[ -f ${FILE} || -f ${FILE}.pub ]]; then
    echo "Key with name '${NAME}' already exists, remove following files explicitly:"
    echo "  ${FILE} ${FILE}.pub"
    exit 1;
fi

set -x
ssh-keygen -C "${NAME}.${USER}@${HOSTNAME}" -f ${FILE} -t ${TYPE} -a 100
```
> In case one needs to generate many keys at one, one can provide a passphrase
> by `-N "toor"` or an empty one as `-N ""`.

## ssh config - `~/.ssh/config`
Frequently used configs for single match.
```nginx
# When ssh-ing into FOO or BAR do it as user git with given key.
host foo bar
    user git
    identityfile ~/.ssh/some-key

# When ssh-ing into moose actually log into host with ip addr 1.2.3.4.
# Can be used as alias for machines w/o DNS entries.
host moose
    user root
    port 8022
    hostname 1.2.3.4
    identityfile ~/.ssh/some-key
```

Pattern matching and evaluation order.
```nginx
# For parameters, the first valued obtained will be used.
# Therefore, more host-specific blocks should come first.

host tree7
    user banana

hoste tree*
    user cherry
    # can reference matched hostname with %h
    hostname %h.some-dns-path

# ssh tree7 -> banana@tree7.some-dns-path
# ssh tree5 -> cherry@tree5.some-dns-path
```
