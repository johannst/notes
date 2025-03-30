OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_entry)

MEMORY {
    ROM : ORIGIN = 0x00100000, LENGTH = 0x4000
    RAM : ORIGIN = 0x00800000, LENGTH = 0x4000
}

SECTIONS {
    /* Create .text output section at ROM (vaddr) */
    .text : {
        *(.text*)
    } > ROM

    ASSERT(. == ORIGIN(ROM) + SIZEOF(.text), "inc loc counter automatically")

    /* Create .data output section at RAM (vaddr) */
    /* Set load addr to ROM, right after .text (paddr) */
    .data : {
        HIDDEN(_data_vaddr = .);
        HIDDEN(_data_paddr = LOADADDR(.data));
        *(.data*)
    } > RAM AT > ROM

    /* Append .rodata output section at ROM (vaddr) */
    .rodata : {
        *(.rodata*)
    } > ROM

    /* Append .stack output section at RAM (vaddr) aligned up to next 0x1000 */
    .stack : ALIGN (0x1000) {
        . += 0x1000;
        HIDDEN(_stack_top = .);
    } > RAM

    /DISCARD/ : {
        *(.*)
    }
}

/* Some example assertions */
ASSERT(ADDR(.data) != LOADADDR(.data), "DATA vaddr and paddr must be different")
ASSERT(ADDR(.rodata) == LOADADDR(.rodata), "RODATA vaddr and paddr must be euqal")
ASSERT(ADDR(.stack) == ORIGIN(RAM) + 0x1000, "STACK section must aligned to 0x1000")
ASSERT(SIZEOF(.stack) == 0x1000, "STACK section must be 0x1000")
