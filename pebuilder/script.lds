SECTIONS
{
	. = 0x10000;
	.text : {
		*(.pehdr)
		*(.text)
	}
	.data : {
		*(.data)
		*(.rodata)
		*(.bss)
		*(.got)
		*(.zimage)
	}
}
