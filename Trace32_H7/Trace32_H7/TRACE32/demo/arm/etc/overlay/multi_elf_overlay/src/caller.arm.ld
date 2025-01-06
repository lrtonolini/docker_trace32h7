; BASE : ORIGIN =  1K, LENGTH =  4K
; STR1 : ORIGIN =  5K, LENGTH =  4K
; STR2 : ORIGIN =  9K, LENGTH = 10K
; OVL  : ORIGIN = 19K, LENGTH = 10K

BASE 0x0400 0x1000
{
	.text +0
	{
		* (+CODE,+ENTRY)
	}
	.rodata +0
	{
		* (+CONST)
	}
	.data +0
	{
		* (+DATA)
	}
	.bss +0
	{
		* (+BSS)
	}
	.stack +0 EMPTY 0xC00
	{
	}
}

