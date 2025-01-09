/**************************************************************************

	ARM7, ARM9 DCC communication variant

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
	MRC( p14, 0, status, c0, c0 );
	return (status & 2);
}

void DCC_SendWord (unsigned int data)
{
	MCR( p14, 0, data, c1, c0 );
}

unsigned int DCC_ReceiveStatus (void)
{
	unsigned int  status;
	MRC( p14, 0, status, c0, c0 );
	return (status & 1);
}

unsigned int DCC_ReceiveWord (void)
{
	unsigned int data;
	MRC( p14, 0, data, c1, c0 );
	return data;
}

