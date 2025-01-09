MEMORY 
{
	RAM  : ORIGIN = 0x100, LENGTH = 0x1900
}

ENTRY(_start)

SECTIONS
{
	.text : {
		crt0* (.text)
		* (.text)
	} > RAM

	.rodata : {
		* (.rodata)
	} > RAM

	.data : {
		* (.data)
		* (.sdata)
	} > RAM

	__bss_start = .;
	.bss : {
		* (.sbss)
		* (.bss)
	} > RAM
	__bss_end = .;

	__heap_start = .;
}

__stack_end  = ORIGIN(RAM)+LENGTH(RAM);
