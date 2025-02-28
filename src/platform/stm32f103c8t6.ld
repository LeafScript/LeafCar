ENTRY(_start)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        _exit = .;
    } > FLASH

    .data :
    {
        __data_start = .;
        . = ALIGN(4);
        *(.data)
        . = ALIGN(4);
        __data_end = .;
    } > RAM AT > FLASH

    .bss :
    {
        __bss_start__ = .;
        . = ALIGN(4);
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    _estack = ORIGIN(RAM) + LENGTH(RAM);

    _end = .;
    PROVIDE(end = _end);
}