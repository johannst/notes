SECTIONS
{
        . = ALIGN(8);
        HIDDEN(_fntab_start = .);
        .fntab : {
               /* Sort entries for numerical priorities in section names. */
               KEEP(*(SORT(.fntab*)));
        }
        HIDDEN(_fntab_end = .);
}
INSERT AFTER .data
