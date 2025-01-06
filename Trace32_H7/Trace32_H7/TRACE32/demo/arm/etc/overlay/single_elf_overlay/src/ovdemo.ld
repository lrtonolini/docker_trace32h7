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

	__PAGE = 0x1000;
	__PAGE_STORAGE = .;
	OVERLAY __PAGE : AT(__PAGE_STORAGE)
	{
		.page1 {
			LONG(0x41)
			*/task1.o   (.text)
			*/task.o    (.text)
			*/funcasm.o (.text)
			*/sieve.o   (.text)
			* (.page1)
		}

		.page2 {
			LONG(0x42)
			*/blubber.o (.text)
			* (.page2)
		}
	} > RAM

	__bss_start = .;
	.bss : {
		* (.bss)
		* (.sbss)
	} > RAM
	__bss_end = .;

	__stack_end = 0x4000;
}

