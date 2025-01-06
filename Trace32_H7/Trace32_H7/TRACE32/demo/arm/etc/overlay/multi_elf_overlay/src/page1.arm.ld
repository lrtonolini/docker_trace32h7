; BASE : ORIGIN =  1K, LENGTH =  4K
; STR1 : ORIGIN =  5K, LENGTH =  4K
; STR2 : ORIGIN =  9K, LENGTH = 10K
; OVL  : ORIGIN = 19K, LENGTH = 10K

STR1 0x1400 0x1000 
{
	LOADER_TEXT +0
	{
		*loader*.o (.init, +FIRST)
		*loader*.o (+TEXT)
	}
	LOADER_DATA +0 ZEROPAD
	{
		*loader*.o (+DATA)
		*loader*.o (+BSS)
	}
	TEXT 0x4C00
	{
		* (.id, +FIRST)
		* (+TEXT)
		* (+CONST)
	}
	DATA +0
	{
		* (+DATA)
	}
	BSS +0
	{
		* (+BSS)
	}
}
