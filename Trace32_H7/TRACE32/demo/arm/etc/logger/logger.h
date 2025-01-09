/*
 * TRACE32 LOGGER
 *
 * Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
 * All rights reserved
 *
 * Link logger.c
 * with your application to use the TRACE32 LOGGER.
 *
 * Please refer to app_logger.pdf for more information 
 *
 * $LastChangedRevision: 17054 $
 *
 */

#include <stdint.h>

#ifndef LOGGER_H
#define LOGGER_H

#define T32_LOGGER_SIZE           1024 //!< Size of the LOGGER Ringbuffer, must be a power of 2

#define T32LOGGERDATA_IFLAG_ARM      0x00000001
#define T32LOGGERDATA_IFLAG_STACK    0x00000100

#define T32LOGGERDATA_OFLAG_OVERFLOW 0x00000001
#define T32LOGGERDATA_OFLAG_TRIGGER  0x00000100
#define T32LOGGERDATA_OFLAG_BREAK    0x00000200
#define T32LOGGERDATA_OFLAG_BP       0x00010000 // signal "core hit breakpoint" event

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
#define T32_QUAD                0x0800 //!< QUAD access - use together with @ref T32_DATA_READ @ref T32_DATA_WRITE

#ifdef LOGGER_64BIT
typedef uint64_t data_t;
typedef uint64_t addr_t;
#else
typedef uint32_t data_t;
typedef uint32_t addr_t;
#endif


typedef struct 
{
#ifdef LOGGER_64BIT
	uint64_t ts;                //!< timestamp (lower 48 bit) and cycle info (upper 16 bit)
#else	
	uint32_t tshigh;            //!< high part of timestamp (lower 16 bit) and cycle info (upper 16 bit)
	uint32_t tslow;             //!< low part of timestamp
#endif
    addr_t        address;      //!< program instruction address or address of data load/store transaction
    data_t        data;         //!< number of executed bytes for T32_EXECUTE, data value of load/store transaction
}
T32_loggerData_t;


typedef struct 
{
    T32_loggerData_t * ptr;     //!< pointer to trace data
    uint32_t size;              //!< size of trace buffer
    volatile uint32_t index;    //!< current write pointer
    uint32_t tindex;            //!< index of trigger record
    uint32_t iflags;            //!< incoming flags, Bit 0: ARM, Bit 8: Stack Mode
    uint32_t oflags;            //!< outgoing flags, Bit 0: Overflow, Bit 8: Trigger, Bit 9: Break
    uint32_t reserved1;
    uint32_t reserved2;
    T32_loggerData_t buffer[T32_LOGGER_SIZE];
}
T32_LoggerStruct_t;

/**
 * @brief Initialize the LOGGER internal data structures.
 *
 * @details This routine must be called before using any other LOGGER related
 * routines. It initializes the logger internal data structures and calls 
 * T32_TimerInit().
 *
 * @param void
 * @return void
 */
extern void T32_LoggerInit(void);

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
#ifdef LOGGER_SMP
void T32_LoggerData(int cycletype, void* address, data_t data, int core);
#else
void T32_LoggerData(int cycletype, void* address, data_t data);
#endif

/**
 * @brief Add a new event to the LOGGER without timestamp.
 *
 * @sa T32_FETCH T32_DATA_READ T32_DATA_WRITE T32_EXECUTE T32_LONG T32_WORD T32_BYTE
 * @param cycletype Type of the event, e.g. @ref T32_FETCH or (@ref T32_DATA_READ|@ref T32_LONG).
 * @param address   Address of the event, in case of @ref T32_FETCH and @ref T32_EXECUTE its an instruction address otherwise a data address.
 * @param data      Data related to the event, needed with @ref T32_DATA_READ, @ref T32_DATA_WRITE and @ref T32_EXECUTE.
 * @return void
 */
 #ifdef LOGGER_SMP
void T32_LoggerDataFast(int cycletype, void* address, data_t data, int core);
#else
void T32_LoggerDataFast(int cycletype, void* address, data_t data);
#endif


/**
 * @brief Initialize the architecture specific timer.
 *
 * @details This routine is architecture specific and must be implemented by
 * by the customer. 
 *
 * Please initialize the timer used in T32_TimerGet() in this routine which is 
 * automatically called by T32_LoggerInit().
 *
 * @param  none
 * @return void
 */
void T32_TimerInit();

/**
 * @brief Get current timestamp of architecture specific timer.
 *
 * @details This routine is architecture specific and must be implemented by
 * by the customer. 
 * 
 * @param  none
 * @return current timestamp of the timer, 56bit width.
 */
uint64_t T32_TimerGet();

void T32_LoggerTrigger();

#endif
