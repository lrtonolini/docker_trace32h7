MEMORY
{
	BASE : ORIGIN =  1K, LENGTH =  4K
	STR1 : ORIGIN =  5K, LENGTH =  4K
	STR2 : ORIGIN =  9K, LENGTH = 10K
	OVL  : ORIGIN = 19K, LENGTH = 10K
}

PHDRS
{
	loader PT_LOAD;
	applic PT_LOAD;
}

SECTIONS
{
	.loader.text : {
		*loader*.o (.init)
		*loader*.o (.text)
	} >STR1	:loader

	.loader.data : {
		*loader*.o (.rodata .data .bss)
	} >STR1	:loader

	.text : {
		* (.id)
		* (.text)
		* (.rodata)
	} >OVL AT>STR1 :applic

	.data : {
		* (.data)
	} >OVL AT>STR1 :applic

	.bss : {
		* (.bss)
		* (.sbss)
		* (COMMON)
	} >OVL AT>STR1 :applic
}

ENTRY(init)

_loader      = ADDR(.loader.text); 

_text_start  = ADDR(.text);
_text_store  = LOADADDR(.text);
_text_length = SIZEOF(.text);

_data_start  = ADDR(.data);
_data_store  = LOADADDR(.data);
_data_length = SIZEOF(.data);

_bss_start   = ADDR(.bss);
_bss_store   = LOADADDR(.bss);
_bss_length  = SIZEOF(.bss);
