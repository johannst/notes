mbr: mbr.ld mbr.o
	ld -o $@.elf -nostdlib -T $^
	objcopy -O binary $@.elf $@

mbr.o: mbr.S
	gcc -c -o $@ -m32 -ffreestanding $^

clean:
	$(RM) mbr.o mbr.elf mbr

run: mbr
	qemu-system-i386 -hda mbr $(QEMU_ARGS)

debug:
	make run QEMU_ARGS="-s -S"

gdb:
	gdb -ex 'target remote :1234' mbr.elf -ex 'layout asm' -ex 'b entry_pm32' -ex 'c'

