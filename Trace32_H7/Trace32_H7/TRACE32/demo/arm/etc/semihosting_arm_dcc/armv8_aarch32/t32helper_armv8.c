/**************************************************************************

	ARM11, Cortex-R4, Cortex-A8 DCC communication variant
	
**************************************************************************/

unsigned int T32_TsMon_SendStatus(void)
{
	int status;

	__asm
	{
		MRC p14, 0, status, c0, c1;
	}
	
	return (status & 0x20000000);
}


void T32_TsMon_SendWord(unsigned int data)
{
	__asm
	{
		MCR p14, 0, data, c0, c5
	}
}


unsigned int T32_TsMon_ReceiveStatus(void)
{
	int status;

	__asm
	{
		MRC p14, 0, status, c0, c1;
	}

	return (status & 0x40000000);
}


unsigned int T32_TsMon_ReceiveWord(void)
{
	unsigned int    data;

	__asm
	{
		MRC p14, 0, data, c0, c5;
	}

	return data;
}
