/* =====================================
// linker.ld
// Linker
//
// Author: Ioane Baidoshvili
// Date 06-Sep-24
// ===================================== */

ENTRY(_start)
OUTPUT_FORMAT(binary) /* .bin */

SECTIONS
{
    . = 0x100000; /* 1 MiB (beyond the real mode) */

    .text : ALIGN(4096)
    {
        *(.text)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
        KEEP(*(.magic))
    }

    .data : ALIGN(4096)
    {
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }
}