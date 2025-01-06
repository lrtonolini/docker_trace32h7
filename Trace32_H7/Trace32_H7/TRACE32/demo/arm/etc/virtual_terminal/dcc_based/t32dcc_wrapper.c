
/**************************************************************************

	ARM DCC communication variant

	Compiler DEFINES:
		__aarch64__: ARMv8 AARCH64 state otherwise AARCH32/Thumb 
		__GNUC__   : GNU GCC/ARMCLANG otherwise ARMCC
	User DEFINES:
		ARM7   : Use Communication Registers for ARM7
		ARM9   : Use Communication Registers for ARM9
		ARM11  : Use Communication Registers for ARM11
		XSCALE : Use Communication Registers for XSCALE
		CORTEXR: Use Communication Registers for CortexRx
		CORTEXA: Use Communication Registers for CortexAx (default)


**************************************************************************/


#if !(defined(ARM9)||defined(ARM7)||defined(ARM11)||defined(CORTEXA)||defined(CORTEXR)||defined(XSCALE))
# define CORTEXA
#endif

#if defined(ARM9)||defined(ARM7)

unsigned int T32_Dcc_SendStatus(void)
{
    int             status;
#ifdef __GNUC__
    __asm__ volatile("mrc p14, 0, %0, c0, c0" : "=r" (status));
#else
    __asm
    {
        MRC p14, 0, status, c0, c0;
    }
#endif
    return (status & 2);
}


void T32_Dcc_SendWord(unsigned int data)
{
#ifdef __GNUC__
    __asm__ volatile("mcr p14, 0, %0, c1, c0" : :"r" (data));
#else
    __asm
    {
        MCR p14, 0, data, c1, c0
    }
#endif
}


unsigned int T32_Dcc_ReceiveStatus(void)
{
    int             status;
#ifdef __GNUC__
    __asm__ volatile("mrc p14, 0, %0, c0, c0" : "=r" (status));
#else
    __asm
    {
        MRC p14, 0, status, c0, c0;
    }
#endif
    return (status & 1);
}


unsigned int T32_Dcc_ReceiveWord(void)
{
    unsigned int    data;
#ifdef __GNUC__
    __asm__ volatile("mrc p14, 0, %0, c1, c0" : "=r" (data));
#else
    __asm
    {
        MRC p14, 0, data, c1, c0;
    }
#endif
    return data;
}

#endif

#if defined(ARM11)||defined(CORTEXA)||defined(CORTEXR)

unsigned int T32_Dcc_SendStatus(void)
{
    int             status;
#ifdef __GNUC__
# ifdef __aarch64__
    __asm__ volatile("mrs %0, mdccsr_el0" : "=r" (status));
# else
    __asm__ volatile("mrc p14, 0, %0, c0, c1" : "=r" (status));
# endif
#else
    __asm
    {
        MRC p14, 0, status, c0, c1;
    }
#endif
    return (status & 0x20000000);
}


void T32_Dcc_SendWord(unsigned int data)
{
#ifdef __GNUC__
# ifdef __aarch64__
    __asm__ volatile("msr dbgdtrtx_el0, %0" : : "r" (data));
# else
    __asm__ volatile("mcr p14, 0, %0, c0, c5" : : "r" (data));
# endif
#else
    __asm
    {
        MCR p14, 0, data, c0, c5
    }
#endif
}


unsigned int T32_Dcc_ReceiveStatus(void)
{
    int             status;
#ifdef __GNUC__
# ifdef __aarch64__
    __asm__ volatile("mrs %0, mdccsr_el0" : "=r" (status));
# else
    __asm__ volatile("mrc p14, 0, %0, c0, c1" : "=r" (status));
# endif
#else
    __asm
    {
        MRC p14, 0, status, c0, c1;
    }
#endif
    return (status & 0x40000000);
}


unsigned int T32_Dcc_ReceiveWord(void)
{
    unsigned int    data;
#ifdef __GNUC__
# ifdef __aarch64__
    __asm__ volatile("mrs %0, dbgdtrrx_el0" : "=r" (data));
# else
    __asm__ volatile("mrc p14, 0, %0, c0, c5" : "=r" (data));
# endif
#else
    __asm
    {
        MRC p14, 0, data, c0, c5;
    }
#endif
    return data;
}

#endif

#if defined(XSCALE)

unsigned int T32_Dcc_SendStatus(void)
{
    int             status;
#ifdef __GNUC__
    __asm__ volatile("mrc p14, 0, %0, c14, c0" : "=r" (status));
#else
    __asm
    {
        MRC p14, 0, status, c14, c0, 0;
    }
#endif
    return (status & 0x10000000);
}


void T32_Dcc_SendWord(unsigned int data)
{
#ifdef __GNUC__
    __asm__ volatile("mcr p14, 0, %0, c8, c0" : : "r" (data));
#else
    __asm
    {
        MCR p14, 0, data, c8, c0, 0
    }
#endif
}


unsigned int T32_Dcc_ReceiveStatus(void)
{
    int             status;
#ifdef __GNUC__
    __asm__ volatile("mrc p14, 0, %0, c14, c0" : "=r" (status));
#else
    __asm
    {
        MRC p14, 0, status, c14, c0, 0;
    }
#endif
    return (status & 0x80000000);
}


unsigned int T32_Dcc_ReceiveWord(void)
{
    unsigned int    data;
#ifdef __GNUC__
    __asm__ volatile("mrc p14, 0, %0, c9, c0" : "=r" (data));
#else
    __asm
    {
        MRC p14, 0, data, c9, c0, 0;
    }
#endif
    return data;
}

#endif



