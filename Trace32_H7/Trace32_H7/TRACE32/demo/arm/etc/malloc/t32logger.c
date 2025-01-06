
typedef struct
{
    unsigned long tshigh;	/* high part of timestamp and cycle info */
    unsigned long tslow;	/* low part of timestamp */
    unsigned long address;
    unsigned long data;
}
T32_loggerData;

#define T32_LOGGER_SIZE		1024

extern T32_loggerData T32_LoggerBuffer[];

struct
{
    T32_loggerData * ptr;	/* pointer to trace data */
    long size;			/* size of trace buffer */
    volatile long index;	/* current write pointer */
    long tindex;		/* index of trigger record */
    long iflags;		/* incoming flags, Bit 0: ARM, Bit 8: Stack Mode */
    long oflags;		/* outgoing flags, Bit 0: Overflow, Bit 8: Trigger, Bit 9: Break */
    long reserved1;
    long reserved2;
    T32_loggerData buffer[T32_LOGGER_SIZE];
}
T32_LoggerStruct;



void T32_LoggerInit()
{
    T32_LoggerStruct.ptr = T32_LoggerStruct.buffer;
    T32_LoggerStruct.size = T32_LOGGER_SIZE;
}


static unsigned long GetTimer()
{
    	return 0;
}



void T32_LoggerData(int cycletype, void* address, unsigned long data)
{
    int             index;

    if (!(T32_LoggerStruct.iflags & 0x01))
	return;

    if (T32_LoggerStruct.index >= T32_LoggerStruct.size) {
	if (T32_LoggerStruct.iflags & 0x100)
	    return;
	T32_LoggerStruct.oflags |= 0x01;
	T32_LoggerStruct.index = 0;
    }

    index = T32_LoggerStruct.index++;
    T32_LoggerStruct.ptr[index].tslow = GetTimer();
    T32_LoggerStruct.ptr[index].tshigh = cycletype << 24;
    T32_LoggerStruct.ptr[index].address = (unsigned long) address;
    T32_LoggerStruct.ptr[index].data = data;
    T32_LoggerStruct.index = index + 1;
}


void T32_LoggerDataFast(int cycletype, void* address, unsigned long data)
{
    int             index = (T32_LoggerStruct.index++ & (T32_LOGGER_SIZE-1));

    T32_LoggerStruct.ptr[index].tshigh = cycletype << 24;
    T32_LoggerStruct.ptr[index].address = (unsigned long) address;
    T32_LoggerStruct.ptr[index].data = data;
}


void T32_LoggerTrigger()
{
    if (T32_LoggerStruct.oflags & 0x100)
	return;
    T32_LoggerStruct.tindex = T32_LoggerStruct.index;
    T32_LoggerStruct.oflags |= 0x100;
}

