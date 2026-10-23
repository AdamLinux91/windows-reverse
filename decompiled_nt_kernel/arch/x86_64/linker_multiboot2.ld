/*
 * Freestanding Bare-Metal x86-64 Linker Script (1 MiB Physical Load Base)
 * Places Multiboot2 + Multiboot1 headers in the first 4 KiB of the image.
 */
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
    . = 0x00100000;

    .text ALIGN(4K) : {
        KEEP(*(.multiboot))
        *(.text.boot)
        *(.text .text.*)
    }

    .rodata ALIGN(4K) : {
        *(.rodata .rodata.*)
    }

    .data ALIGN(4K) : {
        *(.data .data.*)
    }

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss .bss.*)
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
        *(.comment)
    }
}
