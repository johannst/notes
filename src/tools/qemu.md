# qemu(1)

All the examples & notes use `qemu-system-x86_64` but in most cases
this can be swapped with the system emulator for other architectures.

## Keybindings

Graphic mode:
```markdown
Ctrl+Alt+g         release mouse capture from VM

Ctrl+Alt+1         switch to display of VM
Ctrl+Alt+2         switch to qemu monitor
```

No graphic mode:
```markdown
Ctrl+a h           print help
Ctrl+a x           exit emulator
Ctrl+a c           switch between monitor and console
```

## VM config snippet

Following command-line gives a good starting point to assemble a VM:
```
qemu-system-x86_64                 \
    -cpu host -enable-kvm -smp 4   \
    -m 8G                          \
    -vga virtio -display sdl,gl=on \
    -boot menu=on                  \
    -cdrom <iso>                   \
    -hda <disk>                    \
    -device qemu-xhci,id=xhci      \
    -device usb-host,bus=xhci.0,vendorid=0x05e1,productid=0x0408,id=capture-card
```

### CPU & RAM

```bash
# Emulate host CPU in guest VM, enabling all supported host featured (requires KVM).
# List available CPUs `qemu-system-x86_64 -cpu help`.
-cpu host

# Enable KVM instead software emulation.
-enable-kvm

# Configure number of guest CPUs.
-smp <N>

# Configure size of guest RAM.
-m 8G
```

### Graphic & Display

```bash
# Use sdl window as display and enable openGL context.
-display sdl,gl=on

# Use vnc server as display (eg on display `:42` here).
-display vnc=localhost:42

# Confifure virtio as 3D video graphic accelerator (requires virgl in guest).
-vga virtio
```

### Boot Menu

```bash
# Enables boot menu to select boot device (enter with `ESC`).
-boot menu=on
```

### Block devices

```bash
# Attach cdrom drive with iso to a VM.
-cdrom <iso>

# Attach disk drive to a VM.
-hda <disk>

# Generic way to configure & attach a drive to a VM.
-drive file=<file>,format=qcow2
```

#### Create a disk with [`qemu-img`][doc-qemu-img]

To create a `qcow2` disk (qemu copy-on-write) of size `10G`:
```bash
qemu-img create -f qcow2 disk.qcow2 10G
```

The disk does not contain any `partitions` or a `partition table`.
We can format the disk from __within the **guest**__ as following example:
```bash
# Create `gpt` partition table.
sudo parted /dev/sda mktable gpt

# Create two equally sized primary partitions.
sudo parted /dev/sda mkpart primary 0% 50%
sudo parted /dev/sda mkpart primary 50% 100%

# Create filesystem on each partition.
sudo mkfs.ext3 /dev/sda1
sudo mkfs.ext4 /dev/sda2

lsblk -f /dev/sda
  NAME   FSTYPE LABEL UUID FSAVAIL FSUSE% MOUNTPOINT
  sda
  ├─sda1 ext3         ....
  └─sda2 ext4         ....
```

### USB
#### Host Controller

```bash
# Add XHCI USB controller to the VM (supports USB 3.0, 2.0, 1.1).
# `id=xhci` creates a usb bus named `xhci`.
-device qemu-xhci,id=xhci
```

#### USB Device

```bash
# Pass-through USB device from host identified by vendorid & productid and
# attach to usb bus `xhci.0` (defined with controller `id`).
-device usb-host,bus=xhci.0,vendorid=0x05e1,productid=0x0408
```

## Debugging

```bash
# Open gdbstub on tcp `<port>` (`-s` shorthand for `-gdb tcp::1234`).
-gdb tcp::<port>

# Freeze guest CPU at startup and wait for debugger connection.
-S
```

## IO redirection

```bash
# Create raw tcp server for `serial IO` and wait until a client connects
# before executing the guest.
-serial tcp:localhost:12345,server,wait

# Create telnet server for `serial IO` and wait until a client connects
# before executing the guest.
-serial telnet:localhost:12345,server,wait

# Configure redirection for the QEMU `mointor`, arguments similar to `-serial`
# above.
-monitor ...
```
> In `server` mode use `nowait` to execute guest without waiting for a client
> connection.

## Network

```bash
# Redirect host tcp port `1234` to guest port `4321`.
-nic user,hostfwd=tcp:localhost:1234-:4321
```

## Shared drives

```bash
# Attach a `virtio-9p-pci` device to the VM.
# The guest requires 9p support and can mount the shared drive as:
#   mount -t 9p -o trans=virtio someName /mnt
-virtfs local,id=someName,path=<someHostPath>,mount_tag=someName,security_model=none
```

## Tracing

```bash
# List name of all trace points.
-trace help

# Enable trace points matching pattern and optionally write trace to file.
-trace <pattern>[,file=<file>]

# Enable trace points for all events listed in the <events> file.
# File must contain one event/pattern per line.
-trace events=<events>
```

## VM snapshots

VM snapshots require that there is at least on `qcow2` disk attached to the VM
([VM Snapshots][qemu-doc-snapshot]).

Commands for qemu [Monitor][qemu-doc-monitor] or [QMP][qemu-doc-qmp]:
```bash
# List available snapshots.
info snapshots

# Create/Load/Delete snapshot with name <tag>
savevm <tag>
loadvm <tag>
delvm <tag>
```

The snapshot can also be directly specified when invoking qemu as:
```bash
qemu-system-x86_64 \
    -loadvm <tag>  \
    ...
```

## Appendix: Direct `Kernel` boot

Example command line to directly boot a `Kernel` with an `initrd` ramdisk.
```bash
qemu-system-x86_64                                                     \
    -cpu host                                                          \
    -enable-kvm                                                        \
    -kernel <dir>/arch/x86/boot/bzImage                                \
    -append "earlyprintk=ttyS0 console=ttyS0 nokaslr init=/init debug" \
    -initrd <dir>/initramfs.cpio.gz                                    \
    ...
```
Instructions to build a minimal [`Kernel` and `initrd`][blog-qemu-dbg].

## References

- [QEMU USB][doc-qemu-usb]
- [QEMU IMG][doc-qemu-img]
- [QEMU Tools][doc-qemu-tools]
- [QEMU System][doc-qemu-system]
- [QEMU Invocation (command line args)][doc-qemu-invocation]
- [QEMU Monitor][doc-qemu-monitor]
- [QEMU machine protocol (QMP)][doc-qemu-qmp]
- [QEMU VM Snapshots][doc-qemu-snapshot]


[doc-qemu-usb]: https://github.com/qemu/qemu/blob/master/docs/usb2.txt
[doc-qemu-img]: https://qemu-project.gitlab.io/qemu/tools/qemu-img.html
[doc-qemu-tools]: https://qemu-project.gitlab.io/qemu/tools/index.html
[doc-qemu-system]: https://qemu-project.gitlab.io/qemu/system/index.html
[doc-qemu-invocation]: https://qemu-project.gitlab.io/qemu/system/invocation.html
[doc-qemu-monitor]: https://qemu-project.gitlab.io/qemu/system/monitor.html
[doc-qemu-qmp]: https://qemu-project.gitlab.io/qemu/interop/qemu-qmp-ref.html
[doc-qemu-snapshot]: https://qemu-project.gitlab.io/qemu/system/images.html#vm-005fsnapshots
[blog-qemu-dbg]: https://blog.memzero.de/kernel-debugging-qemu
