/**************************************************************************

	ARM7, ARM9 DCC communication variant
	
**************************************************************************/

unsigned int DCC_SendStatus (void)
{
	int status;

	__asm
	{
		MRC p14, 0, status, c0, c0;
	}

	return (status & 2);
}

void DCC_SendWord (unsigned int data)
{
	__asm
	{
		MCR p14, 0, data, c1, c0
	}
}

unsigned int DCC_ReceiveStatus (void)
{
	int  status;

	__asm
	{
		MRC p14, 0, status, c0, c0;
	}

	return (status & 1);
}

unsigned int DCC_ReceiveWord (void)
{
	unsigned int data;

	__asm
	{
		MRC p14, 0, data, c1, c0;
 	}

	return data;
}

