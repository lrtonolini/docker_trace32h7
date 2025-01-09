MEMORY 
{
	RAM : ORIGIN =  1K, LENGTH = 15K
}

SECTIONS
{
	. = 1K;
	.text : {
		*/crt0.o (.text)
		*/ovdemo.o (.text)
		* (.glue_7)
		* (.glue_7t)
	} > RAM

	.rodata : {
		* (.rodata)
	} > RAM

	.data : {
		* (.data)
	} > RAM

	__PAGE_SLOT_A = 0x1000;
	__PAGE_STORAGE_A = .;
	OVERLAY __PAGE_SLOT_A : AT(__PAGE_STORAGE_A) {
		.pageA0 { LONG(0xA0)  */ovl_a0.*o  (.text) }
		.pageA1 { LONG(0xA1)  */ovl_a1.*o  (.text) }
	} > RAM

	__PAGE_SLOT_B = 0x2000;
	__PAGE_STORAGE_B = .;
	OVERLAY __PAGE_SLOT_B : AT(__PAGE_STORAGE_B) {
		.pageB0 { LONG(0xB0)  */ovl_b0.*o  (.text) }
		.pageB1 { LONG(0xB1)  */ovl_b1.*o  (.text) }
	} > RAM

	.bss : {
		__bss_start = .;
		* (.bss)
		* (.sbss)
		__bss_end = .;
	} > RAM

	__stack_end = 0x4000;
}

