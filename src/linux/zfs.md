# zfs

Pools are managed with the [`zpool(8)`][man-zpool] command and have the
following hierarchy:
- `pool`: consists of one or more virtual devices (`vdev`)
- `vdev`: consists of one or more physical devices (`dev`) and come in
  different kinds such as [`disk`, `mirror`, `raidzX`, ...][man-zpool-vdev]
  - `disk`: single physical disk (`vdev == dev`)
  - `mirror`: data is identically replicated on all `devs` (requires at least 2
    physical devices).

Data stored in a pool is distributed and stored across all `vdevs` by zfs.
Therefore a total failure of a single `vdev` can lead to total loss of a pool.

A `dataset` is a logical volume which can be created on top of a `pool`. Each
`dataset` can be configured with its own set of `properties` like
[`encryption`, `quota`, ...][man-zfsprops].
Datasets are managed with the [`zfs(8)`][man-zfs] command.

## zfs pool management
Pools are by default mounted at `/<POOL>`.

### Create, modify and destroy zfs pools
```bash
# Create a pool MOOSE with a two mirror vdevs.
zpool create moose mirror <dev1> <dev2> mirror <dev3> <dev4>..

# Add new raidz1 vdev to a pool.
zpool add moose raidz1 <devA> <devB> <devC>..

# Remove a vdev from a pool.
zpool remove moose <vdevX>

# Destroy a pool.
zpool destroy moose
```
> For stable device names in small home setups it is recommended to use names
> from [`/dev/disk/by-id`][faq-which-dev].

### Inspect zfs pools
```bash
# Show status of all pools or a single one.
zpool status [<pool>]

# Show information / statistics about pools or single one.
zpool list [<pool>]

# Show statistics for all devices.
zpool list -v

# Show command history for pools.
zpool history
```

### Modify `vdevs`
```bash
# vdev MIRROR-0 with two devs.
zpool status
    NAME            STATE     READ WRITE CKSUM
    moose           ONLINE       0     0     0
      mirror-0      ONLINE       0     0     0
        virtio-200  ONLINE       0     0     0
        virtio-300  ONLINE       0     0     0

# Attach new device to an existing vdev.
zpool attach moose virtio-200 virtio-400

# vdev MIRROR-0 with three devs.
zpool status
    NAME            STATE     READ WRITE CKSUM
    moose           ONLINE       0     0     0
      mirror-0      ONLINE       0     0     0
        virtio-200  ONLINE       0     0     0
        virtio-300  ONLINE       0     0     0
        virtio-400  ONLINE       0     0     0

# Detach device from vdev.
zpool detach moose virtio-200
```

### Replace faulty disk
```bash
# MIRROR-0 is degraded as one disk failed, but still intact.
zpool status
    NAME            STATE     READ WRITE CKSUM
    moose           DEGRADED     0     0     0
      mirror-0      DEGRADED     0     0     0
        virtio-200  UNAVAIL      0     0     0  invalid label
        virtio-300  ONLINE       0     0     0

# Replace faulty disk, in mirror.
# No data is lost since mirror still has one good disk.
zpool replace moose virtio-200 virtio-400

# MIRROR-0 back in ONLINE (good) state.
zpool status
    NAME            STATE     READ WRITE CKSUM
    moose           ONLINE       0     0     0
      mirror-0      ONLINE       0     0     0
        virtio-400  ONLINE       0     0     0
        virtio-300  ONLINE       0     0     0
```

### Import or export zfs pools
When moving pools between hosts, the pool must be `exported` on the currently
active host and `imported` on the new host.

```bash
# Export a pool called MOOSE.
zpool export moose

# List pools that can be imported using BY-ID deivce names (for example).
zpool import -d /dev/disk/by-id

# Import pool MOOSE using BY-ID device names (for example).
zpool import -d /dev/disk/by-id moose
```
> Device names used by an existing pool can be changed by [exporting and
> importing][faq-rename-dev] a pool again.

## zfs dataset management
Datasets are by default mounted at `/<POOL>/<DATASET>`.

### Create and destroy zfs datasets
```bash
# Create dataset FOO on pool MOOSE.
zfs create moose/foo

# Destroy dataset.
zfs destroy moose/foo
```

### List all zfs datasets
```bash
# List all zfs datasets.
zfs list
```

### Mount zfs datasets
```bash
# List currently mounted datasets.
zfs mount

# Mount dataset.
zfs mount moose/foo

# Unmount dataset.
zfs unmount moose/foo
```

### Encrypted datasets
Encryption is a readonly property, can only be set when creating a dataset.

```bash
# Create encrypted dataset ENC on pool MOOSE.
zfs create -o encryption=on -o keyformat=passphrase moose/foo

# Mount encrypte dataset (if key is not loaded).
zfs mount -l moose/foo

# Unmount dataset and unload encryption key (unload is optional).
zfs umount -u moose/foo
```

### Manage zfs encryption keys
```bash
# Preload encryption key for dataset.
zfs load-key moose/foo

# Preload encryption key for all datasets.
zfs load-key -a

# Change encryption key for dataset.
zfs change-key moose/foo

# Unload encryption key for dataset.
zfs unload-key moose/foo
```

### Manage dataset properties
```bash
# Get all properties for dataset.
zfs get quota moose/foo

# Get single property for dataset.
zfs get all moose/foo

# Get single property for all datasets.
zfs get quota

# Set property on dataset.
zfs set quota=10G moose/foo
```

### Snapshots
```bash
# Create snapshot called V2 for dataset moose/foo.
zfs snapshot moose/foo@v2

# List all snapshots.
zfs list -t snapshot

# Make .zfs direcotry visible in the root of the dataset.
zfs set snapdir=visible moose/foo

# Browse available snapshots in visible .zfs direcotry (readonly).
ls /moose/foo/.zfs/snapshot
v1/  v2/

# Create a new dataset based on the V1 snapshot
zfs clone moose/foo@v1 moose/foov1

# Destroy snapshot.
zfs destroy moose/foo@v1
```

### Access control list
Focus on [posix acl](./acl.md).
```bash
# Set the ACL type for the FOO dataset to POSIXACL.
zfs set acltype=posixacl moose/foo

# Get the ACL type of a given dataset.
zfs get acltype moose/foo
```
> For performance reasons it is recommended to also set `zfs set xattr=sa
> moose/foo` [[ref][zfs-xattr-perf]].

## Example: zfs pool import during startup (`systemd`)
The default zpool cache file is `/etc/zfs/zpool.cache`. When pools are imported
the cache is updated.

Enable the following targets / services to automatically import pools from the
cache.
```bash
systemctl list-dependencies
  ...
    └─zfs.target
      └─zfs-import.target
        └─zfs-import-cache.service
```

[man-zpool]: https://openzfs.github.io/openzfs-docs/man/8/zpool.8.html
[man-zpool-vdev]: https://openzfs.github.io/openzfs-docs/man/7/zpoolconcepts.7.html#Virtual_Devices_(vdevs)
[man-zfs]: https://openzfs.github.io/openzfs-docs/man/8/zfs.8.html
[man-zfsprops]: https://openzfs.github.io/openzfs-docs/man/7/zfsprops.7.html
[faq-which-dev]: https://openzfs.github.io/openzfs-docs/Project%20and%20Community/FAQ.html#selecting-dev-names-when-creating-a-pool-linux
[faq-rename-dev]: https://openzfs.github.io/openzfs-docs/Project%20and%20Community/FAQ.html#changing-dev-names-on-an-existing-pool
[zfs-xattr-perf]: https://github.com/openzfs/zfs/issues/170#issuecomment-27348094
