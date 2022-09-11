# swap

## List active swap areas

```sh
# procfs
cat /proc/swaps

# cli tool
swapon --show
```

## Manual swapfile setup

```sh
# One time:
#   Create and initialize swapfile.
#   mkswap will initialize swap area over full filesize by default.
sudo dd if=/dev/zero of=/swapfile bs=1G count=1
mkswap /swapfile

# Enable swap file (until next reboot).
swapon /swapfile

# Persistent setup of swap file.
echo "/swapfile none swap sw 0 0" | sudo tee -a /etc/fstab

# Disable swap file (until next reboot).
swapoff /swapfile
```
> Recommended file permissions `0600` and file owner `uid=0 (root)`.

## Using `dphys-swapfile` service.

Dynamically computes size of swap file based on installed RAM.

```sh
# Setup and enable swap based on /etc/dphys-swapfile.
dphys-swapfile setup
dphys-swapfile swapon

# Disable swap on configured file.
dphys-swapfile swapoff
```

> Usually comes with a script to be automatically run at system startup and shutdown.
> For example as `systemd` service:
> ```
> systemctl status dphys-swapfile
> ```
