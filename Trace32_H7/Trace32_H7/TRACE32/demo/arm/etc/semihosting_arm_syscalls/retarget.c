/*
 * Copyright (C) ARM Limited, 2006. All rights reserved.
 * 
 * This is a retargeted I/O example which implements the functions required
 * for communication through an UART. The implementation relies on two UART
 * functions which the user must provide (UART_write and UART_read) for 
 * sending and receiving single characters to and from the UART.
 *
 * See the "rt_sys.h" header file for complete function descriptions.
 */

#include <rt_sys.h>
#include <time.h>
#include <string.h>
#include "t32term.h"

#define TRUE 1
#define FALSE 0

#ifdef __cplusplus
extern "C" {
#endif


/*
 * These names are special strings which will be recognized by 
 * _sys_open and will cause it to return the standard I/O handles, instead
 * of opening a real file.
 */
const char __stdin_name[] =":STDIN";
const char __stdout_name[]=":STDOUT";
const char __stderr_name[]=":STDERR";


/*
 * Open a file. May return -1 if the file failed to open. We do not require
 * this function to do anything. Simply return a dummy handle.
 */
FILEHANDLE _sys_open(const char * name, int openmode)
{
	FILEHANDLE handle = 0;
	
	if ((unsigned) openmode > (unsigned) 11)
		handle = -1;
	
	if (strlen(name) > 255)
		handle = -1;
	
	if (strcmp(__stdin_name, name) == 0)
	{
		switch (openmode)
		{
			case OPEN_R:
				handle = 1;
				break;
			case OPEN_W:
				handle = 2;
				break;
			default:
				handle = -1;
				break;
		}
	}
	else if (strcmp(__stdout_name, name) == 0)
	{
		switch (openmode)
		{
			case OPEN_R:
				handle = 1;
				break;
			case OPEN_W:
				handle = 2;
				break;
			default:
				handle = -1;
				break;
		}
	}
	else if (strcmp(__stderr_name, name) == 0)
	{
		switch (openmode)
		{
			case OPEN_R:
				handle = 1;
				break;
			case OPEN_W:
				handle = 2;
				break;
			default:
				handle = -1;
				break;
		}
	}
	else
	{
		handle = T32_Term_OpenFileANSI(name, openmode);
		if (handle != -1)
			handle = handle + 4;
	}	
	return handle;
}



/*
 * Close a file. Should return 0 on success or a negative value on error.
 * Not required in this implementation. Always return success.
 */
int _sys_close(FILEHANDLE fh)
{
	if (fh > 4)
		return T32_Term_CloseFile(fh - 4);
	else
		return 0;
}

/*
 * Write to a file. Returns 0 on success, negative on error, and the number
 * of characters _not_ written on partial success. This implementation sends
 * a buffer of size 'len' to the UART.
 */
int _sys_write(FILEHANDLE fh, const unsigned char * buf, unsigned len, int mode)
{
	int result = 0;
	if (fh > 4) {
		result = T32_Term_WriteFile(fh-4, (const char*) buf, len);
		if (result < 0)
			result = 0;
		result = len - result;
	} else
		T32_Term_PutBlock(buf, len);	
	return result;
}

/*
 * Read from a file. Can return:
 *  - zero if the read was completely successful
 *  - the number of bytes _not_ read, if the read was partially successful
 *  - the number of bytes not read, plus the top bit set (0x80000000), if
 *    the read was partially successful due to end of file
 *  - -1 if some error other than EOF occurred
 * This function receives a character from the UART, processes the character
 * if required (backspace) and then echo the character to the Terminal 
 * Emulator, printing the correct sequence after successive keystrokes.  
 */
int _sys_read(FILEHANDLE fh, unsigned char * buf, unsigned len, int mode)
{
	int result = 0;
	if (fh > 4) {
		result = T32_Term_ReadFile(fh-4, (char*) buf, len);
		if (result < 0)
			result = 0;
		result = len - result;
	}
	else
		T32_Term_Getline((char*) buf, len);	
	return result;
}

/*
 * Writes a character to the output channel. This function is used
 * for last-resort error message output.
 */
void _ttywrch(int ch)
{
	// Convert correctly for endianness change
	unsigned char ench = (unsigned char)ch;
	
	T32_Term_PutBlock(&ench, 1);
}

/*
 * Return non-zero if the argument file is connected to a terminal.
 */
int _sys_istty(FILEHANDLE fh)
{
	return (fh < 4); // no interactive device present
}

/*
 * Move the file position to a given offset from the file start.
 * Returns >=0 on success, <0 on failure. Seeking is not supported for the 
 * UART.
 */
int _sys_seek(FILEHANDLE fh, long pos)
{
	if (fh > 4)
		return T32_Term_SeekFile(fh-4, pos);
	else
		return -1;
}

/*
 * Flush any OS buffers associated with fh, ensuring that the file
 * is up to date on disk. Result is >=0 if OK, negative for an
 * error.
 */
int _sys_ensure(FILEHANDLE fh)
{
	return 0; // success
}

/*
 * Return the current length of a file, or <0 if an error occurred.
 * _sys_flen is allowed to reposition the file pointer (so Unix can
 * implement it with a single lseek, for example), since it is only
 * called when processing SEEK_END relative fseeks, and therefore a
 * call to _sys_flen is always followed by a call to _sys_seek.
 */
long _sys_flen(FILEHANDLE fh)
{
	if (fh > 4)
		return T32_Term_TellSizeFile(fh-4);
	else
		return -1;
}

/*
 * Return the name for temporary file number sig in the buffer
 * name. Returns 0 on failure. maxlen is the maximum name length
 * allowed.
 */
int _sys_tmpnam(char * name, int sig, unsigned maxlen)
{
	return T32_Term_GetTempName(name);
}

/*
 * Terminate the program, passing a return code back to the user.
 * This function may not return.
 */
void _sys_exit(int returncode)
{
	while(1) {};
}

/*
* determines the current calendar time. The encoding of the value is
* unspecified.
* Returns: the implementations best approximation to the current calendar
*          time. The value (time_t)-1 is returned if the calendar time is
*          not available. If timer is not a null pointer, the return value
*          is also assigned to the object it points to.
*/
time_t time(time_t *timer)
{
	time_t time = T32_Term_GetTime();
	if (timer)
		*timer = time;
	
	return time;
}


#ifdef __cplusplus
}
#endif
