# restic(1)

## Create new snapshot repository

```bash
# Create a local backup repository.
restic -r <path> init

# Create a backup repository on a remote host.
restic -r sftp:user@host:<path> init
```

## Example: Restore file pattern from `latest` snapshot
Restore files matching `<file_pattern>` from the `latest` snapshot (pseudo
snapshot ID) into `<dest>`.
```bash
restic -r <repo> restore -i <file_pattern> --target <dest> latest
```

## Mount snapshots
Mount snapshots as user filesystem (fuse) to given mount point.
```bash
restic -r <repo> mount <mntpoint>

# Mounted snapshots can be limited by host.
restic -r <repo> mount --host <host> <mntpoint>

# Mounted snapshots can be limited by path (abs path).
restic -r <repo> mount --path <abspath> <mntpoint>
```

## Repository maintenance
Check the repository for errors and report them.
```bash
restic -r <repo> check
```

Check the repository for non-referenced data and remove it.
```bash
restic -r <repo> prune
```

## References
- [`restic` read the docs][restic]
- [sftp][restic-sftp]

[restic]: https://restic.readthedocs.io/en/stable/index.html
[restic-sftp]: https://restic.readthedocs.io/en/stable/030_preparing_a_new_repo.html#sftp
