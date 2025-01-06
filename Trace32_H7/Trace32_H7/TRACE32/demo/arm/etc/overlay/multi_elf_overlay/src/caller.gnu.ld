MEMORY 
{
	BASE : ORIGIN =  1K, LENGTH =  4K
	STR1 : ORIGIN =  5K, LENGTH =  4K
	STR2 : ORIGIN =  9K, LENGTH = 10K
	OVL  : ORIGIN = 19K, LENGTH = 10K
}

SECTIONS
{
	.text : {
		* (.text)
	} > BASE

	.rodata : {
		* (.rodata)
	} > BASE

	.data : {
		* (.data)
	} > BASE

	_bss_start = .;
	.bss : {
		* (.bss)
		* (.sbss)
		* (COMMON)
	} > BASE
	_bss_end = .;

	_sp = .;
}

_sp = ORIGIN(BASE)+LENGTH(BASE);
