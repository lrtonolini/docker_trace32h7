MEMORY 
{
	RAM : ORIGIN = 0, LENGTH = 4K
}

SECTIONS
{
	.text : {
		* (.first)
		* (.init)
		* (.text)
		* (.fini)
	} > RAM

	.rodata : {
		* (.rodata)
		* (.rodata*)
	} > RAM

	.data : {
		* (.ctors)
		* (.dtors)
		* (.jcr)
		* (.data)
	} > RAM 

	__SDATA_BEGIN__ = .;
	.sdata : {
		* (.sdata)
	} > RAM

	__bss_start = .;
	__sbss_start = .;
	.sbss : {
		* (.sbss)
	} > RAM

	.bss : {
		* (.bss)
	} > RAM
	_end = .;

	.arcextmap : { 
		* (.gnu.linkonce.arcextmap.*)
	}
}
