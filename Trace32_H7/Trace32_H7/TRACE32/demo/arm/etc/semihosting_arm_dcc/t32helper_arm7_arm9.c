
/**************************************************************************

	ARM7, ARM9 DCC communication variant
	
**************************************************************************/

unsigned int T32_TsMon_SendStatus(void)
{
    int             status;

    __asm
    {
	MRC             p14, 0, status, c0, c0;
    }
    return (status & 2);
}


void T32_TsMon_SendWord(unsigned int data)
{
    __asm
    {
	MCR             p14, 0, data, c1, c0
    }
}


unsigned int T32_TsMon_ReceiveStatus(void)
{
    int             status;

    __asm
    {
	MRC             p14, 0, status, c0, c0;
    }

    return (status & 1);
}


unsigned int T32_TsMon_ReceiveWord(void)
{
    unsigned int    data;

    __asm
    {
	MRC             p14, 0, data, c1, c0;
    }

    return data;
}

