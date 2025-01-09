using namespace std;

#include "t32logger.h"

T32_LoggerC :: T32_LoggerC ()
{
    this->T32_LoggerInit();
}

T32_LoggerC :: ~T32_LoggerC ()
{
}

void T32_LoggerC :: T32_LoggerInit()
{
  int i;

    for (i=0; i<T32_LOGGER_SIZE; i++) {
      this->buffer[i].tshigh = 0;
      this->buffer[i].tslow = 0;
      this->buffer[i].address = 0;
      this->buffer[i].data = 0;
    }
    this->ptr = this->buffer;
    this->size = T32_LOGGER_SIZE;
    this->tindex = -1; // -1: trigger not set
    this->iflags = 0;
    this->oflags = 0;
    this->reserved1 = 0;
    this->reserved2 = 0;
}

void T32_LoggerC :: T32_LoggerData(int cycletype, void* address, unsigned long data)
{
    int                nIndex;
    unsigned long long nTimer;

    if (!(this->iflags & T32LOGGERDATA_IFLAG_ARM))
	return;

    if (this->index >= this->size) {
	if (this->iflags & T32LOGGERDATA_IFLAG_STACK)
	    return;
	this->oflags |= T32LOGGERDATA_OFLAG_OVERFLOW;
	this->index = 0;
    }

    if (cycletype == T32_FETCH) 
      address = (void*) ((int)address & (int)0xFFFFFFFE); // zero thumb LSB
    nIndex = this->index++;
    nTimer = this->T32_TimerGet();
    this->ptr[nIndex].tslow = (unsigned long)(nTimer & 0xffffffff);
    this->ptr[nIndex].tshigh = (cycletype << 16)|((unsigned long)(nTimer>>32)&0x0000ffff);
    this->ptr[nIndex].address = (unsigned long) address;
    this->ptr[nIndex].data = data;
    this->index = nIndex + 1;
}

unsigned long long T32_LoggerC :: T32_TimerGet()
{
    return (long long) micros();
}

void T32_LoggerC :: Off(void)
{
  this->iflags &= ~T32LOGGERDATA_IFLAG_ARM;
}

void T32_LoggerC :: Arm(void)
{
  this->iflags |= T32LOGGERDATA_IFLAG_ARM;
}

void * T32_LoggerC :: GetPC()
{
  return __builtin_return_address(0);
}

void T32_LoggerC :: LogPC(void)
{
    this->T32_LoggerData(T32_FETCH, (void *) this->GetPC(), 0);
}





void T32_LoggerC :: Function(void (*address)())
{
    this->T32_LoggerData(T32_FETCH, (void *) address, 0);
}

void T32_LoggerC :: Write(unsigned int* address, unsigned int data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Write(unsigned int* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(unsigned long* address, unsigned long data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Write(unsigned long* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(unsigned short* address, unsigned short data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_WORD, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Write(unsigned short* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_WORD, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(unsigned char* address, unsigned char data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_BYTE, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Write(unsigned char* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_BYTE, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(int* address, int data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Write(int* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(long* address, long data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Write(long* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_LONG, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(short* address, short data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_WORD, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Write(short* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_WORD, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Write(char* address, char data)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_BYTE, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Write(char* address)
{
    this->T32_LoggerData(T32_DATA_WRITE|T32_BYTE, (void *) address, (unsigned long) *address);
}


void T32_LoggerC :: Read(unsigned int* address, unsigned int data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Read(unsigned long* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_LONG, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Read(unsigned short* address, unsigned short data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_WORD, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Read(unsigned short* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_WORD, (void *) address, (unsigned long) *address);
}

void T32_LoggerC :: Read(unsigned char* address, unsigned char data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_BYTE, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Read(unsigned char* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_BYTE, (void *) address, (unsigned long) *address);
}


void T32_LoggerC :: Read(int* address, int data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Read(int* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_LONG, (void *) address, (unsigned long) *address);
}


void T32_LoggerC :: Read(long* address, long data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_LONG, (void *) address, data);
}

void T32_LoggerC :: Read(long* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_LONG, (void *) address, (unsigned long) *address);
}


void T32_LoggerC :: Read(short* address, short data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_WORD, (void *) address, (unsigned long) data);
}


void T32_LoggerC :: Read(short* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_WORD, (void *) address, (unsigned long) *address);
}


void T32_LoggerC :: Read(char* address, char data)
{
    this->T32_LoggerData(T32_DATA_READ|T32_BYTE, (void *) address, (unsigned long) data);
}

void T32_LoggerC :: Read(char* address)
{
    this->T32_LoggerData(T32_DATA_READ|T32_BYTE, (void *) address, (unsigned long) *address);
}
