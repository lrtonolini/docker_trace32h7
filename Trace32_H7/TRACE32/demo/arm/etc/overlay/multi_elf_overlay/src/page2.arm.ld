; BASE : ORIGIN =  1K, LENGTH =  4K
; STR1 : ORIGIN =  5K, LENGTH =  4K
; STR2 : ORIGIN =  9K, LENGTH = 10K
; OVL  : ORIGIN = 19K, LENGTH = 10K

STR2 0x2400 0x2800 
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
