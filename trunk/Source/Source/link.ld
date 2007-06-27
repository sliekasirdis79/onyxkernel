OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(start)
SECTIONS
{
	. = 0x00100000;
	.text :	
	{
		code = .; _code = .;
		*(.text)
		. = ALIGN(4096);
	}

	.data :
	{
		data = .; _data = .;
		*(.data)
		. = ALIGN(4096);
	}
	
	.bss :
	{
		bss = .; _bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	.rodata : 
	{
		rodata = .; _rodata = .;
		*(.rodata)
		. = ALIGN(4096);
	}
	. = ALIGN(4096);
	end = .; _end = .;
}