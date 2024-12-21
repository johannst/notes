OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

SECTIONS {
    . = 0x7c00;
    .boot     : { *(.boot) }
    _boot_end = .;
    /DISCARD/ : { *(.*) }

    ASSERT(_boot_end - 0x7c00 == 512, "boot sector must be exact 512 bytes")
}
