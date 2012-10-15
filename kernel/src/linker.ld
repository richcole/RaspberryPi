MEMORY
{
    ram : ORIGIN = 0x8000, LENGTH = 0x30000-0x8000
}

SECTIONS
{
    .text : { *(.text*) } > ram
	__bss_start__ = .;
    .bss : { *(.bss*) *(.rodata) *(.data) } > ram
	__bss_end__ = .;
}

