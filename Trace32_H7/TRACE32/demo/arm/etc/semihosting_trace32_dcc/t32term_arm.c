/**************************************************************************

	ARM DCC/DCC3 communication variant

	Compiler DEFINES:
		__aarch64__: ARMv8 AARCH64 state, otherwise AARCH32/Thumb
	User DEFINES:
		ARM7   : Use Communication Registers for ARM7
		ARM9   : Use Communication Registers for ARM9
		ARM11  : Use Communication Registers for ARM11
		XSCALE : Use Communication Registers for XSCALE
		CORTEXR: Use Communication Registers for CortexRx
		CORTEXA: Use Communication Registers for CortexAx (AArch64: DCC3 only)
		CORTEXM: Use 'BUFFERE', memory based interface (DCC channel is not available on Cortex-M)

**************************************************************************/

#if defined(ARM9)||defined(ARM7)

#if !(defined(DCC3)||defined(DCC))
	#define DCC3
#endif

/* DCC3 or DCC*/
#ifdef DCC3

void T32_Term_PutBlock(const unsigned char * data, int size)
{
	int             len;
	unsigned long   ch, k;

	if (!data)
		return;

	while (size > 0) {
	len = size;
	if (len > 3)
	    len = 3;
	ch = data[0] | (data[1] << 8) | (data[2]) << 16 | ((len - 1) << 24);
	do /* Wait for Terminal Ready */
	{
		__asm__ volatile ("MRC p14, 0, %0, c0, c0" : "=r" (k));
	} while (k&2);

	__asm__ volatile ("MCR p14, 0, %0, c1, c0" : : "r" (ch));

	data += len;
	size -= len;
	}
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
	static int      bufsize = 0;
	static int      bufindex;
	static unsigned char buf[4];
	int             i, j;
	unsigned long   ch;

	while (size > 0) {
	if (!bufsize) {
	    do /* Wait for input byte */
	    {
			__asm__ volatile ("MRC p14, 0, %0, c0, c0" : "=r" (ch));
	    } while (!(ch&1));

		__asm__ volatile ("MRC p14, 0, %0, c1, c0" : "=r" (ch));

	    buf[0] = ch & 0xff;
	    buf[1] = (ch >> 8) & 0xff;
	    buf[2] = (ch >> 16) & 0xff;
	    bufsize = (ch >> 24) + 1;
	    bufindex = 0;
	}
	i = bufsize;
	if (i > size)
	    i = size;

	for (j = 0; j < i; j++)
	    data[j] = buf[bufindex + j];

	bufindex += i;
	bufsize -= i;
	data += i;
	size -= i;
	}
}

#else /* DCC */

void T32_Term_PutBlock(const unsigned char * data, int size)
{
	register unsigned int k, len;

	if (!data)
		return;

	while (size > 0)
	{
		do /* Wait for Terminal Ready */
		{
			__asm__ volatile ("MRC p14, 0, %0, c0, c0" : "=r" (k));
		}while (k&2);
		k = *data;

		__asm__ volatile ("MCR p14, 0, %0, c1, c0" : : "r" (k));

		size--;
		data++;
	}
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
	register unsigned int k, len;

	while (size > 0)
	{
		do /* Wait for input byte */
		{
			__asm__ volatile ("MRC p14, 0, %0, c0, c0" : "=r" (k));
		}while (!(k&1));
		__asm__ volatile ("MRC p14, 0, %0, c1, c0" : "=r" (k));
		*data = k;
		size--;
		data++;
	}
}

#endif

#endif


#if defined(ARM11)||defined(CORTEXA)||defined(CORTEXR)

#if !(defined(DCC3)||defined(DCC))
	#define DCC3
#endif

/* DCC3 or DCC*/
#ifdef DCC3

//=============================================================================
// DCC3 method

void T32_Term_PutBlock(const unsigned char * data, int size)
{
	int             len;
	unsigned long   ch, k;

	if (!data)
		return;

	while (size > 0) {
	len = size;
	if (len > 3)
	    len = 3;
	ch = data[0] | (data[1] << 8) | (data[2]) << 16 | ((len - 1) << 24);

#ifdef __aarch64__
	do /* Wait for Terminal Ready */
	{
		__asm__ volatile ("MRS %0, MDSCR_EL1": "=r" (k));
	} while (k & 0x20000000);  // TXfull

	__asm__ volatile ("MSR DBGDTR_EL0, %0" : : "r" (ch));
#else
	do /* Wait for Terminal Ready */
	{
		__asm__ volatile ("MRC p14, 0, %0, c0, c1" : "=r" (k));
	} while (k & 0x20000000);

	__asm__ volatile ("MCR p14, 0, %0, c0, c5" : : "r" (ch));

#endif
	data += len;
	size -= len;
	}
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
	static int      bufsize = 0;
	static int      bufindex;
	static unsigned char buf[4];
	int             i, j;
	unsigned int    ch;

	while (size > 0)
	{
		if (!bufsize)
		{

#ifdef __aarch64__
			do /* Wait for input byte */
	    	{
				__asm__ volatile ("MRS %0, MDSCR_EL1": "=r" (ch));
	    	} while (!(ch & 0x40000000));  // RXfull

			__asm__ volatile ("MRS %0, DBGDTR_EL0" : "=r" (ch));
#else
			do /* Wait for input byte */
			{
				__asm__ volatile ("MRC p14, 0, %0, c0, c1" : "=r" (ch));
			} while (!(ch & 0x40000000));

			__asm__ volatile ("MRC p14, 0, %0, c0, c5" : "=r" (ch));
#endif

	    buf[0] = ch & 0xff;
	    buf[1] = (ch >> 8) & 0xff;
	    buf[2] = (ch >> 16) & 0xff;
	    bufsize = (ch >> 24) + 1;
	    bufindex = 0;
	}
	i = bufsize;
	if (i > size)
	    i = size;

	for (j = 0; j < i; j++)
	    data[j] = buf[bufindex + j];

	bufindex += i;
	bufsize -= i;
	data += i;
	size -= i;
	}
}

#elif !defined(__aarch64__)

//=============================================================================
// DCC method

void T32_Term_PutBlock(const unsigned char * data, int size)
{
	register unsigned int k, len;

	if (!data)
		return;

		while (size > 0)
	{
		do /* Wait for Terminal Ready */
		{
			__asm__ volatile ("MRC p14, 0, %0, c0, c1" : "=r" (k));
		}while (k & 0x20000000);
		k = *data;

		__asm__ volatile ("MCR p14, 0, %0, c0, c5" : : "r" (k));

		size--;
		data++;
	}
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
	register unsigned int k, len;

	while (size > 0)
	{
		do /* Wait for input byte */
		{
			__asm__ volatile ("MRC p14, 0, %0, c0, c1" : "=r" (k));
		}while (!(k & 0x40000000));

		__asm__ volatile ("MRC p14, 0, %0, c0, c5" : "=r" (k));

		*data = k;
		size--;
		data++;
	}
}

#endif

#endif


#if defined(CORTEXM)

unsigned char InBuffer[256];
unsigned char OutBuffer[256];

void T32_Term_PutBlock(unsigned char * data, int size)
{
	// volatile unsigned char * pout = (unsigned char *) 0x1000;
	// requires 'TERM.METHOD BUFFERE e:0x1000 e:0x2000' in the *.cmm script

	volatile unsigned char * pout = OutBuffer;

	int i, j;

	if (!data)
		return;

	while (size > 0)
	{
		i = size;
		if ( i > 252)
			i = 252;
		while (*pout) ;	/* wait for ready */
		for (j = 0 ; j < i; j++)
			pout[j+4] = data[j];
		*pout = i;
		data += i;
		size -= i;
	}
}

void T32_Term_GetBlock(unsigned char * data, int size)
{
	// volatile unsigned char * pin = (unsigned char *) 0x2000;
	// requires 'TERM.METHOD BUFFERE e:0x1000 e:0x2000' in the *.cmm script

	volatile unsigned char * pin = InBuffer;

	static int bufsize = 0;
	static int bufindex;
	int i, j;

	while (size > 0)
	{
		if (!bufsize)
		{
			while (!*pin) ;	/* wait for ready */
			bufsize = *pin;
			bufindex = 0;
		}

		i = bufsize;
		if ( i > size)
			i = size;

		for ( j = 0 ; j < i ; j++)
			data[j] = pin[4+bufindex+j];

		bufindex += i;
		if (bufindex >= bufsize) {
			*pin = 0;	/* ready for next */
			bufsize = 0;
		}
		data += i;
		size -= i;
	}
}

#endif
