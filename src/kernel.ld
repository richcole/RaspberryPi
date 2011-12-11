ENTRY(__reset)
SECTIONS
{
	. = 0x0;
	.text : {
		*(.reset)
		*(.text)
	}
	
	.data : { *(.data) }
	__bss_start__ = .;
	.bss : { *(.bss) }
	__bss_end__ = .;
}
