SECTIONS
{
        . = ALIGN(8);
        HIDDEN(_datatab_start = .);
        .datatab : {
               KEEP(*(.datatab));
        }
        HIDDEN(_datatab_end = .);
}
INSERT AFTER .data
