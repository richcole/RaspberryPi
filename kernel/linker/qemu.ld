MEMORY
{
    ram : ORIGIN = 0x00010000, LENGTH = 0x30000
}

SECTIONS
{
    .text : { *(.text*) } > ram
	__bss_start = .;
    .bss : { *(.bss*) *(.rodata*) *(.data) } > ram
	__bss_end = .;
}

