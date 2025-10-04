OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_entry)

SECTIONS {
    /* Set the initial location counter (vaddr) */
    . = 0x00800000;

    /* Create .text output section at current vaddr */
    .text : {
        *(.text*)
    }

    ASSERT(. == 0x00800000 + SIZEOF(.text), "inc loc counter automatically")

    /* Create .data section at location counter aligned to the next 0x100 (vaddr) */
    /* Set the load address to  0x00100000 (paddr) */
    .data ALIGN(0x100) : AT(0x00100000) {
        HIDDEN(_data_vaddr = .);
        HIDDEN(_data_paddr = LOADADDR(.data));
        *(.data*)
    }

    /* Create .rodata with explicit vaddr */
    /* Re-adjust the paddr location counter */
    .rodata 0x00804000 : AT(ADDR(.rodata)) {
        *(.rodata*)
    }

    ASSERT(. == 0x00804000 + SIZEOF(.rodata), "inc loc counter automatically")

    .stack ALIGN (0x1000) : {
        . += 0x1000;
        HIDDEN(_stack_top = .);
    }

    /DISCARD/ : {
        *(.*)
    }
}

/* Some example assertions */
ASSERT(ADDR(.data) != LOADADDR(.data), "DATA vaddr and paddr must be different")
ASSERT(ADDR(.rodata) == LOADADDR(.rodata), "RODATA vaddr and paddr must be euqal")
ASSERT(ADDR(.stack) == 0x00805000, "STACK section must aligned to 0x1000")
ASSERT(SIZEOF(.stack) == 0x1000, "STACK section must be 0x1000")
