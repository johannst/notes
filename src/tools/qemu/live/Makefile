BUILD   := $(PWD)/build
INSTALL := $(PWD)/install
ISO     := debian-live-13.4.0-amd64-xfce.iso
DISK    := $(INSTALL)/disk.qcow2

QEMUIMG := $(INSTALL)/bin/qemu-img
QEMU    := $(INSTALL)/bin/qemu-system-x86_64
QEMU-y  += -accel kvm -cpu host
QEMU-y  += -m 8G
QEMU-y  += -monitor stdio
QEMU-y  += -nic user
QEMU-y  += -cdrom $(BUILD)/$(ISO)
QEMU-y  += -hda $(DISK)
QEMU-y  += -vnc :80
# guest: mount -t 9p shared /mnt
QEMU-y  += -virtfs local,id=shared,path=$(PWD)/shared,mount_tag=shared,security_model=none

run: $(QEMU) $(BUILD)/$(ISO) $(DISK)
	$(QEMU) $(QEMU-y)

vnc:
	vncviewer :80

$(QEMU): | $(BUILD)
	test -d $(BUILD)/qemu-src || git clone https://gitlab.com/qemu/qemu $(BUILD)/qemu-src
	mkdir -p $(BUILD)/qemu-build shared
	cd $(BUILD)/qemu-build \
		&& ../qemu-src/configure --target-list=x86_64-softmmu --prefix=$(INSTALL) --enable-slirp \
		&& make -j8 install

$(BUILD)/$(ISO): | $(BUILD)
	wget --no-clobber -O $@ https://cdimage.debian.org/debian-cd/current-live/amd64/iso-hybrid/$(ISO)

$(DISK):
	$(QEMUIMG) create -f qcow2 $@ 50G

snapshots:
	$(QEMUIMG) snapshot -l $(DISK)

$(BUILD):
	mkdir -p $@
