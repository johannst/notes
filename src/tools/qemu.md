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
```bash
qemu-system-x86_64                              \
        -cpu host -enable-kvm -smp 4            \
        -m 8G                                   \
        -vga virtio -display sdl,gl=on          \
        -boot menu=on                           \
        -cdrom <iso>                            \
        -hda <disk>                             \
        -device qemu-xhci,id=xhci               \
        -device usb-host,bus=xhci.0,vendorid=0x05e1,productid=0x0408,id=capture-card
```

### CPU & RAM

- `-cpu host` emulate host CPU in guest VM
- `-enable-kvm` use KVM instead software models (requires KVM on host machine)
- `-smp <N>` number of guest CPUs
> List available CPUs `qemu-system-x86_64 -cpu help`.

- `-m 8G` size of guest RAM

### Graphic & Display

- `-vga virtio` use virtio as 3D video graphic accelerator (requires virgl in guest)
- `-display sdl,gl=on` use sdl window and enable openGL context

### Boot Menu

- `-boot menu=on` enables boot menu to select boot device (enter with `ESC`)

### Block devices

- `-cdrom <iso>` attach cdrom drive with iso to a VM
- `-hda <disk>` attach disk drive to a VM
- `-drive file=<file>,format=qcow2` generic way to configure & attach a drive to a VM

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
- `-device qemu-xhci,id=xhci` add XHCI USB controller to the VM (supports USB 3.0, 2.0, 1.1). `id=xhci` creates a usb bus named `xhci`.

#### USB Device
- `-device usb-host,bus=xhci.0,vendorid=0x05e1,productid=0x0408` pass-through USB device from host identified by vendorid & productid and attach to usb bus `xhci.0` (defined with controller `id`)

## Debugging

- `-gdb tcp::<port>` open gdbstub on tcp `<port>` (`-s` shorthand for `-gdb tcp::1234`).
- `-S` freeze CPU at startup.

## References
- [QEMU USB][doc-qemu-usb]
- [QEMU IMG][doc-qemu-img]
- [QEMU Tools][doc-qemu-tools]
- [QEMU System][doc-qemu-system]


[doc-qemu-usb]: https://github.com/qemu/qemu/blob/master/docs/usb2.txt
[doc-qemu-img]: https://qemu.readthedocs.io/en/latest/tools/qemu-img.html
[doc-qemu-tools]: https://qemu.readthedocs.io/en/latest/tools/index.html
[doc-qemu-system]: https://www.qemu.org/docs/master/system/index.html
