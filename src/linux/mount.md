# mount(8)

## List supported filesystems

If the kernel is compiled with [`CONFIG_PROC_FS`][proc-filesystems] one can
list the supported filesystems via the following procfs node.
```sh
cat /proc/filesystems
```

## Mount options

The mount operation allows to specify additional *mout options*.
- The [`mount(8)`][mount-8] manpage provides some generic options under
  `FILESYSTEM-INDEPENDENT MOUNT OPTIONS` as well as some filesystem specific
  options under `FILESYSTEM-SPECIFIC MOUNT OPTIONS`.
- The manpage for the specific filesystems list additional mount options, as
  for example [`tmpfs(5)`][tmpfs-5].

## Example: Mount tmpfs with given user id

This creates a in memory filesystems with a size limit where the root folder is
owned by `uid=gid=1000`.
```sh
sudo mount -t tmpfs -o size=1G,uid=1000,gid=1000,mode=0755 none ./tmp
```

## Example: Bind mount

```sh
# Bind mount src onto dir, this does not rebind and recursive mountpoints.
mount --bind <src> <dst>
# Similar but rebind also recursive mountpoints.
mount --rbind <src> <dst>

# Mount file ./resolv.conf onto /etc/resolv.conf.
mount --bind ./resolv.conf /etc/resolv.conf
# Mount dir ./etc onto /etc.
mount --bind ./etc /etc
```

[mount-8]: https://www.man7.org/linux/man-pages/man8/mount.8.html
[tmpfs-5]: https://www.man7.org/linux/man-pages/man5/tmpfs.5.html
[proc-filesystems]: https://elixir.bootlin.com/linux/v6.18.6/source/fs/filesystems.c#L240-L263
