/* =====================================
// linker.ld
// Linker
//
// Author: Ioane Baidoshvili
// Date 06-Sep-24
// ===================================== */

OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)

ENTRY(_start)

SECTIONS
{
    /* Bootloader section at 0x7C00 (BIOS loads the bootloader here) */
    . = 0x7C00;

    .boot : {
        *(.boot)      /* Include all .boot sections */
        *(.text)      /* Include all .text sections for bootloader code */
    }

    /* Align to 1 MiB (where kernel starts) */
    . = 0x100000;

    /* Kernel sections */
    .text : ALIGN(4096)
    {
        __kernel_start = .;  /* This is the start address of the kernel */
        *(.text)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
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

    __kernel_end = .;  /* This is the end address of the kernel */
}