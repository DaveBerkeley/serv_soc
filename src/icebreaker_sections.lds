MEMORY
{
    ROM (xr)  : ORIGIN = 0x00100000, LENGTH = 0x20000
    RAM (rw)  : ORIGIN = 0x00000000, LENGTH = 0x20000
}
SECTIONS {
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        *(.text)
        *(.text*)
        *(.eh_frame)
        . = ALIGN(4);
        _etext = .;
        _sidata = _etext;
    } >ROM
    .data : AT ( _sidata )
    {
        . = ALIGN(4);
        _sdata = .;
        _ram_start = .;
        . = ALIGN(4);
        *(.data)
        *(.data*)
        *(.sdata)
        *(.sdata*)
        *(.rodata)
        *(.rodata*)
        *(.srodata)
        *(.srodata*)
        . = ALIGN(4);
        _edata = .;
    } >RAM
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    /* Specify the stack size */
    _stack_size = 0x1000;
    _estack = ORIGIN(RAM) + LENGTH(RAM);
    _sstack = _estack - _stack_size;

    /* The heap is everything else */
    .heap :
    {
        . = ALIGN(4);
        _sheap = .;
        _eheap = _sstack;
    } >RAM

}
