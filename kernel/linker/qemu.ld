MEMORY
{
    ram : ORIGIN = 0x100000, LENGTH = 0x30000
}

SECTIONS
{
    .text : { *(.text*) } > ram
	__bss_start = .;
    .bss : { *(.bss*) *(.rodata*) *(.data) } > ram
	__bss_end = .;
}

