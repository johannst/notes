.code16
.intel_syntax noprefix

.section .boot, "ax", @progbits
    // Disable interrupts.
    cli

    // Clear segment selectors.
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    // Set cs to 0x0000, as some BIOSes load the MBR to either 07c0:0000 or 0000:7c000.
    jmp 0x0000:entry_rm16

entry_rm16:
    // Set video mode 3, see [1].
    //   * 80x25 text mode
    //   * 640x200 pixel resolution (8x8 pixel per char)
    //   * 16 colors (4bit)
    //   * 4 pages
    //   * 0xB800 screen address
    //
    // [1] http://www.ctyme.com/intr/rb-0069.htm
    mov ax, 0x3
    int 0x10

    // Move cursor to second row.
    // http://www.ctyme.com/intr/rb-0087.htm
    mov ah, 0x02
    mov bh, 0  // page
    mov dh, 1  // row
    mov dl, 0  // col
    int 0x10

    // Clear direction flag for lodsb below.
    cld

    // Load pointer to msg_rm string (null terminated).
    lea si, [msg_rm]

    // Teletype output char at current cursor position.
    // http://www.ctyme.com/intr/rb-0106.htm
    mov ah, 0x0e
1:
    lodsb         // al <- ds:si ; si+=1 ; (al char to write)
    test al,al    // test for null terminator
    jz 2f
    int 0x10
    jmp 1b
2:

    // Enable A20 address line.
    in al, 0x92
    or al, 2
    out 0x92, al

    // Load GDT descriptor.
    lgdt [gdt_desc]

    // Enable protected mode (set CR0.PE bit).
    mov eax, cr0
    or  eax, (1 << 0)
    mov cr0, eax

    // Far jump which loads segment selector (0x0008) into cs.
    // 0x0008 -> RPL=0, TI=0(GDT), I=1
    jmp 0x0008:entry_pm32

.code32
entry_pm32:
    // Select data segment selector (0x0010) for ds.
    mov ax, gdt_data - gdt
    mov ds, ax

    // Write through VGA interface (video memory).
    // Each character is represented by 2 bytes.
    //   4 bit bg | 4 bit fg | 8 bit ascii char
    //
    // Start writing at third line.
    mov edi, 0xb8000 + (80 * 2 * 2) //

    lea esi, [msg_pm]
1:
    lodsb           // al <- ds:esi ; esi+=1
    test al, al     // test for null terminator
    jz 2f
    or eax, 0x1f00  // blue bg, white fg
    stosw           // ds:[edi] <- ax; edi+=2
    jmp 1b
2:
    hlt
    jmp 2b

// For simplicity keep data used by boot sector in the same section.
.balign 8
msg_rm:
    .asciz "Hello from Real Mode!"
msg_pm:
    .asciz "Hello from Protected Mode!"

.balign 8
gdt:
    .8byte 0x0000000000000000 // 0x00 | null descriptor
    .8byte 0x00cf9a000000ffff // 0x08 | 32 bit, code (rx), present, dpl=0, g=4K, base=0, limit=fffff
gdt_data:
    .8byte 0x00cf92000000ffff // 0x10 | 32 bit, data (rw), present, dpl=0, g=4K, base=0, limit=fffff
gdt_desc:
    .2byte .-gdt-1  // size
    .4byte gdt      // address

// Write MBR boot magic value.
.fill 510 - (. - .boot), 1, 0x00
.2byte 0xaa55
