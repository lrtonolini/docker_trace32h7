using namespace std;

#include <Arduino.h>

#ifndef LOGGER_H
#define LOGGER_H

#define T32_LOGGER_SIZE           1024 //!< Size of the LOGGER Ringbuffer, must be a power of 2

/* program cycle types */
#define T32_FETCH               0x1000 //!< FETCH CYCLE,   add a trace record for a program fetch cycle
#define T32_EXECUTE             0xf000 //!< EXECUTE CYCLE, add a trace record for a program execute cycle, data holds number of executed bytes

/* data cycle types */
#define T32_DATA_READ           0x2000 //!< READ CYCLE,    add a trace record with a read transaction (load), see @ref T32_BYTE @ref T32_WORD @ref T32_LONG
#define T32_DATA_WRITE          0x3000 //!< WRITE CYCLE,   add a trace record with a write transaction (store), see @ref T32_BYTE @ref T32_WORD @ref T32_LONG

/* data cycle - data width specifier, use together with T32_DATA_* */
#define T32_BYTE                0x0100 //!< BYTE access - use together with @ref T32_DATA_READ @ref T32_DATA_WRITE
#define T32_WORD                0x0200 //!< WORD access - use together with @ref T32_DATA_READ @ref T32_DATA_WRITE
#define T32_LONG                0x0400 //!< LONG access - use together with @ref T32_DATA_READ @ref T32_DATA_WRITE

#define T32LOGGERDATA_IFLAG_ARM      0x00000001
#define T32LOGGERDATA_IFLAG_STACK    0x00000100

#define T32LOGGERDATA_OFLAG_OVERFLOW 0x00000001
#define T32LOGGERDATA_OFLAG_TRIGGER  0x00000100
#define T32LOGGERDATA_OFLAG_BREAK    0x00000200
#define T32LOGGERDATA_OFLAG_BP       0x00010000 // signal "core hit breakpoint" event

typedef struct
{
    unsigned long tshigh;       //!< high part of timestamp (upper 16 bit) and cycle info (lower 16 bit)
    unsigned long tslow;        //!< low part of timestamp
    unsigned long address;      //!< program instruction address or address of data load/store transaction
    unsigned long data;         //!< number of executed bytes for T32_EXECUTE, data value of load/store transaction
}
T32_loggerData_t;

class T32_LoggerC
{
  public:
	//Default constructor
    T32_LoggerC();

	//Destructor
    ~T32_LoggerC();

	//Member Function

    void Arm(void);
    void Off(void);

    void Function(void (*address)());
    void LogPC(void);
        
    void Write(unsigned int* address,   unsigned int data);
    void Write(unsigned long* address,  unsigned long data);
    void Write(unsigned short* address, unsigned short data);
    void Write(unsigned char* address,  unsigned char data);
    void Write(unsigned int* address);
    void Write(unsigned long* address);
    void Write(unsigned short* address);
    void Write(unsigned char* address);

    void Write(int* address,   int data);
    void Write(long* address,  long data);
    void Write(short* address, short data);
    void Write(char* address,  char data);
    void Write(int* address);
    void Write(long* address);
    void Write(short* address);
    void Write(char* address);

    void Read(unsigned int* address,   unsigned int data);
    void Read(unsigned long* address,  unsigned long data);
    void Read(unsigned short* address, unsigned short data);
    void Read(unsigned char* address,  unsigned char data);
    void Read(unsigned int* address);
    void Read(unsigned long* address);
    void Read(unsigned short* address);
    void Read(unsigned char* address);
    
	  void Read(int* address,   int data);
    void Read(long* address,  long data);
    void Read(short* address, short data);
    void Read(char* address,  char data);
	  void Read(int* address);
    void Read(long* address);
    void Read(short* address);
    void Read(char* address);

  private:

    void T32_LoggerInit();
    void T32_LoggerData(int cycletype, void* address, unsigned long data);
    unsigned long long T32_TimerGet();
    void * GetPC();
    
    T32_loggerData_t * ptr;     //!< pointer to trace data
    long size;                  //!< size of trace buffer
    volatile long index;        //!< current write pointer
    long tindex;                //!< index of trigger record
    long iflags;                //!< incoming flags, Bit 0: ARM, Bit 8: Stack Mode
    long oflags;                //!< outgoing flags, Bit 0: Overflow, Bit 8: Trigger, Bit 9: Break
    long reserved1;
    long reserved2;
    T32_loggerData_t buffer[T32_LOGGER_SIZE];
};

/**
 * @brief Add a new event to the LOGGER.
 *
 * @details
 *
 * @sa T32_FETCH T32_DATA_READ T32_DATA_WRITE T32_EXECUTE T32_LONG T32_WORD T32_BYTE
 * @param cycletype Type of the event, e.g. @ref T32_FETCH or (@ref T32_DATA_READ|@ref T32_LONG).
 * @param address   Address of the event, in case of @ref T32_FETCH and @ref T32_EXECUTE its a instruction address otherwise a data address.
 * @param data      Data related to the event, needed with @ref T32_DATA_READ and @ref T32_EXECUTE.
 * @return void
 */
void T32_LoggerData(int cycletype, void* address, unsigned long data);

/**
 * @brief Get current timestamp of architecture specific timer.
 *
 * @details This routine is architecture specific and must be implemented by
 * by the customer.
 *
 * @param  none
 * @return current timestamp of the timer, 56bit width.
 */
unsigned long long T32_TimerGet();

extern T32_LoggerC T32_Logger;

#endif
