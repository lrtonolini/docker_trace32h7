RAM 0 0x1000 
{
	.text +0
	{
		* (.first,+FIRST)
		* (+CODE)
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
}

