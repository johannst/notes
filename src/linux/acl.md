# access control list (acl)

> This describes `POSIX` acl.

The access control list provides a flexibel permission mechanism next to the
`UNIX` file permissions. This allows to specify fine grained permissions for
users/groups on filesystems.

Filesystems which support acl typically have an `acl` option, which must be
specified while mounting when it is not a default option.
Filesystems must be mounted with the `acl` option if not enabled as default
option.

Files or folder that have an `acl` defined, can be identified by the `+` sign
next to the UNIX permissions.

The following shows on example for a zfs filesystem.
```bash
# mount | grep tank
tank on /tank type zfs (rw,xattr,noacl)
tank/foo on /tank/foo type zfs (rw,xattr,posixacl)

# ls -h /tank
drwxrwxr-x+ 2 root root 4 11. Jun 14:26 foo/
```

## Show acl entries
```bash
# List current acl entries.
getfacl /tank/foo
```

## Modify acl entries
```bash
# Add acl entry for user "user123".
setfacl -m "u:user123:rwx" /tank/foo

# Remove entry for user "user123".
setfacl -x "u:user123" /tank/foo

# Add acl entry for group "group456".
setfacl -m "g:group456:rx" /tank/foo

# Add acl entry for others.
setfacl -m "o:rx" /tank/foo

# Remove extended acl entries.
setfacl -b /tank/foo
```

## Masking of acl entries
The `mask` defines the maximum access rights that can be given to **users** and
**groups**.

```bash
# Update the mask.
setfacl -m "m:rx" /tank/foo

# List acl entries.
getfacl /tank/foo
# file: tank/foo
# owner: root
# group: root
user::rwx
user:user123:rwx     # effective:r-x
group::r-x
mask::r-x
other::rwx
```

## References
- [acl(5)][man-acl]
- [getfacl(1)][man-getfacl]
- [setfacl(1)][man-setfacl]

[man-acl]: https://www.man7.org/linux/man-pages/man5/acl.5.html
[man-getfacl]: https://www.man7.org/linux/man-pages/man1/getfacl.1.html
[man-setfacl]: https://www.man7.org/linux/man-pages/man1/setfacl.1.html
