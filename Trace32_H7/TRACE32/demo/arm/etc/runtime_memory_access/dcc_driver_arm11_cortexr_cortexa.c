/**************************************************************************

	ARM11, Cortex-R4, Cortex-A8 DCC communication variant

**************************************************************************/

#ifdef __GNUC__
# define MRC(CORPROC,OPCODE,RD,CRN,CRM) __asm__ ("mrc " #CORPROC "," #OPCODE ",%[dst]," #CRN "," #CRM : [dst] "=r" (RD))
# define MCR(CORPROC,OPCODE,RD,CRN,CRM) __asm__ ("mcr " #CORPROC "," #OPCODE ",%[src]," #CRN "," #CRM :: [src] "r" (RD))
#else
# define MRC(CORPROC,OPCODE,RD,CRN,CRM) __asm { mrc CORPROC,OPCODE,RD,CRN,CRM }
# define MCR(CORPROC,OPCODE,RD,CRN,CRM) __asm { mcr CORPROC,OPCODE,RD,CRN,CRM }
#endif


unsigned int DCC_SendStatus (void)
{
	unsigned int status;
	MRC( p14, 0, status, c0, c1 );
	return (status & 0x20000000);
}

void DCC_SendWord (unsigned int data)
{
	MCR( p14, 0, data, c0, c5 );
}

unsigned int DCC_ReceiveStatus (void)
{
	unsigned int status;
	MRC( p14, 0, status, c0, c1 );
	return (status & 0x40000000);
}

unsigned int DCC_ReceiveWord (void)
{
	unsigned int    data;
	MRC( p14, 0, data, c0, c5 );
	return data;
}

