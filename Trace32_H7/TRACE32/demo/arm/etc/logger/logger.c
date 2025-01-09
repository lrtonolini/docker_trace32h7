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

#include "logger.h"

T32_LoggerStruct_t T32_LoggerStruct;

void T32_TimerInit()
{
	// TODO: add here timer initialization
}

uint64_t T32_TimerGet()
{
#ifdef LOGGER_TIMER_DEMONSTRATION
	// for demonstration purpose use an upcounter as timer
	static uint64_t nCounterAsTimer = 0;
	return nCounterAsTimer++;
#else
	return 0; // TODO: return timestamp here
#endif
}

void T32_LoggerInit(void)
{
	T32_LoggerStruct.ptr = T32_LoggerStruct.buffer;
	T32_LoggerStruct.size = T32_LOGGER_SIZE;
	T32_LoggerStruct.tindex = -1; // -1: trigger not set
	T32_LoggerStruct.iflags = 0;
	T32_LoggerStruct.oflags = 0;
	T32_LoggerStruct.reserved1 = 0;
	T32_LoggerStruct.reserved2 = 0;
	T32_TimerInit();
}
#ifdef LOGGER_SMP
void T32_LoggerData(int cycletype, void* address, data_t data, int core)
#else
void T32_LoggerData(int cycletype, void* address, data_t data)
#endif
{
	int      nIndex;
	uint64_t nTimer;

	if (!(T32_LoggerStruct.iflags & T32LOGGERDATA_IFLAG_ARM))
		return;

	if (T32_LoggerStruct.index >= T32_LoggerStruct.size) {
		if (T32_LoggerStruct.iflags & T32LOGGERDATA_IFLAG_STACK)
			return;
		T32_LoggerStruct.oflags |= T32LOGGERDATA_OFLAG_OVERFLOW;
		T32_LoggerStruct.index = 0;
	}

#ifdef LOGGER_SMP
	cycletype |= core;
#endif

	nIndex = T32_LoggerStruct.index++;
	nTimer = T32_TimerGet();
#ifdef LOGGER_64BIT
	T32_LoggerStruct.ptr[nIndex].ts = ((uint64_t)cycletype << 48)|(nTimer&0xffffffffffffull);
#else
	T32_LoggerStruct.ptr[nIndex].tslow  = (uint32_t)(nTimer & 0xffffffff);
	T32_LoggerStruct.ptr[nIndex].tshigh = (cycletype << 16)|((uint32_t)(nTimer>>32)&0x0000ffff);
#endif
	T32_LoggerStruct.ptr[nIndex].address = (data_t) address;
	T32_LoggerStruct.ptr[nIndex].data = data;
	T32_LoggerStruct.index = nIndex + 1;
}

#ifdef LOGGER_SMP
void T32_LoggerDataFast(int cycletype, void* address, data_t data, int core)
#else
void T32_LoggerDataFast(int cycletype, void* address, data_t data)
#endif
{
	int nIndex = (T32_LoggerStruct.index++ & (T32_LOGGER_SIZE-1));

#ifdef LOGGER_SMP
	cycletype |= core;
#endif

#ifdef LOGGER_64BIT
	T32_LoggerStruct.ptr[nIndex].ts = (uint64_t)cycletype << 48;
#else
	T32_LoggerStruct.ptr[nIndex].tshigh = cycletype << 16;
#endif
	T32_LoggerStruct.ptr[nIndex].address = (data_t) address;
	T32_LoggerStruct.ptr[nIndex].data = data;
}


void T32_LoggerTrigger()
{
	if (T32_LoggerStruct.oflags & T32LOGGERDATA_OFLAG_TRIGGER)
		return;
	T32_LoggerStruct.tindex = T32_LoggerStruct.index;
	T32_LoggerStruct.oflags |= T32LOGGERDATA_OFLAG_TRIGGER;
}

