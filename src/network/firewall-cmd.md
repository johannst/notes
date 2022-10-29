# firewall-cmd(1)

Command line interface to the [firewalld(1)][man-firewalld] daemon.

## List current status of the firewall
```sh
# List all services and ports for all zones.
firewall-cmd --list-all
# List all services.
firewall-cmd --list-services
# List all ports.
firewall-cmd --list-ports
```
> Add `--zone <ZONE>` to limit output to a given `ZONE`. Use `--get-zones` to
> see all available zones.

## Add entries
```sh
# Add a service to the firewall, use `--get-services` to list all available
# service names.
firewall-cmd --add-service <SERVICE>
# Add a specific port.
firewall-cmd --add-port 8000/tcp
```

## Remove entries
```sh
# Remove service.
firewall-cmd --remove-service <SERVICE>
# Remove port.
firewall-cmd --remove-port 8000/tcp
```

## References
- man [firewall-cmd(1)][man-firewall-cmd]
- man [firewalld(1)][man-firewalld]

[man-firewalld]: https://firewalld.org/documentation/man-pages/firewalld.html
[man-firewall-cmd]: https://firewalld.org/documentation/man-pages/firewall-cmd.html
