/**************************************************************************

	TRACE32 ARM SWI handler for Semihosting via t32term

	origin file:
	  files/demo/arm/etc/semihosting_arm_dcc/t32swi.c

	03/12/30		initial release
	09/10/22		cmdline support added
	15/10/29		usage of t32term.c/t32term.h files
		
**************************************************************************/


#include <string.h>
#include "t32term.h"

#ifdef __ARMCC_VERSION
int t32swi_armcc_version = __ARMCC_VERSION;
#endif

//imported internal functions and variables from t32term.c
extern int T32_Term_PutPacket(const unsigned char * data, int len);
extern int T32_Term_GetPacket(unsigned char * data);

//exported internal functions and variables to t32term.c
extern int T32_Term_Tar2HostBufferSize;
extern int T32_Term_Host2TarBufferSize;

#if defined(SWI2DCC3)

extern unsigned long T32_TsMon_SendStatus(void);
extern void T32_TsMon_SendWord(unsigned long data);
extern unsigned long T32_TsMon_ReceiveStatus(void);
extern unsigned long T32_TsMon_ReceiveWord(void);

/**************************************************************************

	Receive/Send raw data blocks
			
**************************************************************************/


int T32_Term_Tar2HostBufferSize = 0x100;
int T32_Term_Host2TarBufferSize = 0x100;

void T32_Term_PutBlock(const unsigned char * data, int size)
{
    int             len;
    unsigned long   ch;
	if (!data)
		return;
    while (size > 0) {
	len = size;
	if (len > 3)
	    len = 3;
	ch = data[0] | (data[1] << 8) | (data[2]) << 16 | ((len - 1) << 24);
	while (T32_TsMon_SendStatus());
	T32_TsMon_SendWord(ch);
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
	    while (!T32_TsMon_ReceiveStatus());	/* wait for ready */
	    ch = T32_TsMon_ReceiveWord();
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

#endif /*SWI2DCC3*/

static int T32_Term_HeapInfo(unsigned long * params)
{
    int             i, j;
    unsigned char   msg[256];

    msg[2] = 0x25;

    T32_Term_PutPacket(msg + 2, 1);

    if (T32_Term_GetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;

    for (j = 0, i = 1; j < 4; j++, i += 4)
	params[j] = ((long) msg[i]) | (((long) msg[i + 1]) << 8) | (((long) msg[i + 2]) << 16) | (((long) msg[i + 3]) << 24);

    return 0;
}

void T32_Term_Break()
{
    while (1);
}

/**************************************************************************

	SWI Handler
			
**************************************************************************/

int T32_Swi_Handler(int code, unsigned long * parameter)
{
    int             i, result, len, len2, mode, handle;
    long            pos;
    char           *filename, *filename2, *cmdline;
    char           *data;
    unsigned long  *heapinfo;

    result = -1;
    switch (code) {
    case 0x01:			/* SYS_OPEN */

	filename = (char *) parameter[0];
	mode = (int) parameter[1];
	len = (int) parameter[2];

	if ((unsigned) mode > (unsigned) 11)
	    goto error;
	if (len > 255)
	    goto error;

	if (strlen(filename) != len)
	    goto error;

	if (!strcmp(":tt", filename)) {
	    switch (mode) {
	    case 0:
		result = 1;
		break;
	    case 4:
		result = 2;
		break;
	    default:
		goto error;
	    }
	} else {

	    handle = T32_Term_OpenFileANSI(filename, mode);
	    if (handle == -1)
		goto error;

	    result = handle + 4;
	}

	break;

    case 0x02:			/* SYS_CLOSE */
	handle = (int) parameter[0];

	if (handle > 4) {
	    result = T32_Term_CloseFile(handle - 4);
	    if (handle == -1)
		goto error;
	} else {
	    result = 0;
	}

	break;

    case 0x03:			/* SYS_WRITEC */
	T32_Term_Putchar(*((char *) parameter));
	result = 0;
	break;

    case 0x04:			/* SYS_WRITE0 */
	T32_Term_Puts(((char *) parameter));
	result = 0;
	break;

    case 0x05:			/* SYS_WRITE */
	handle = (int) parameter[0];
	data = (char *) parameter[1];
	len = (int) parameter[2];

	if (handle < 4) {
	    for (i = 0; i < len; i++) {
		if (data[i] == '\n')
		    T32_Term_Putchar('\r');
		T32_Term_Putchar(data[i]);
	    }
	} else {
	    result = T32_Term_WriteFile(handle - 4, data, len);
	    if (result == -1)
		goto errorwrite;
	}
	result = 0;
	break;

    case 0x06:			/* SYS_READ */
	handle = (int) parameter[0];
	data = (char *) parameter[1];
	len = (int) parameter[2];

	if (handle < 4) {
	    result = T32_Term_Getline(data, len - 1);
	    if (result == -1)
		goto errorwrite;
	    data[result++] = '\n';
	} else {
	    result = T32_Term_ReadFile(handle - 4, data, len);
	    if (result == -1)
		goto errorwrite;
	}
	result = len - result;
	break;

    case 0x07:			/* SYS_READC */
	result = T32_Term_Getchar();

    case 0x08:			/* SYS_ISERROR */
	result = (int) parameter[0];
	break;

    case 0x09:			/* SYS_ISTTY */
	handle = (int) parameter[0];
	result = (handle < 4);
	break;

    case 0x0a:			/* SYS_SEEK */
	handle = (int) parameter[0];
	pos = (long) parameter[1];
	if (handle < 4)
	    goto error;
	pos = T32_Term_SeekFile(handle - 4, pos);
	if (pos == -1)
	    goto error;
	result = 0;
	break;

    case 0x0c:			/* SYS_FLEN */
	handle = (int) parameter[0];
	if (handle < 4)
	    goto error;
	pos = T32_Term_TellSizeFile(handle - 4);
	if (pos == -1)
	    goto error;
	result = pos;
	break;

    case 0x0d:			/* SYS_TMPNAM */
	data = (char *) parameter[0];
	i = (int) parameter[1];
	len = (int) parameter[2];
	result = T32_Term_GetTempNameID(data, i);
	if (strlen(data) + 1 > len)
	    goto error;
	break;

    case 0x0e:			/* SYS_REMOVE */
	filename = (char *) parameter[0];
	len = (int) parameter[1];
	if (strlen(filename) != len)
	    goto error;
	result = T32_Term_Remove(filename);
	break;

    case 0x0f:			/* SYS_RENAME */
	filename = (char *) parameter[0];
	len = (int) parameter[1];
	filename2 = (char *) parameter[2];
	len2 = (int) parameter[3];
	if (strlen(filename) != len)
	    goto error;
	if (strlen(filename2) != len2)
	    goto error;
	result = T32_Term_Rename(filename, filename2);
	break;

    case 0x10:			/* SYS_CLOCK */
	result = T32_Term_GetClock();
	break;

    case 0x11:			/* SYS_TIME */
	result = T32_Term_GetTime();
	break;

    case 0x12:			/* SYS_SYSTEM */
	filename = (char *) parameter[0];
	len = (int) parameter[1];

	if (strlen(filename) != len)
	    goto error;

	result = T32_Term_OsExecute(filename, 2);
	break;

    case 0x13:			/* SYS_ERRNO */
	result = -1;
	break;

    case 0x15:			/* SYS_GET_CMDLINE */
	cmdline = (char *) parameter[0];
	len = (int) parameter[1];

	result = T32_Term_GetCmdline(cmdline);

	if (result > len)
	    result = -1;
	else
	    result = 0;
	break;

    case 0x16:			/* SYS_HEAPINFO */
	heapinfo = (unsigned long *) parameter[0];
	if (T32_Term_HeapInfo(heapinfo) == -1)
	    goto error;
	result = 0;
	break;

    case 0x18:			/* SYS_STOP */
	T32_Term_Break();
	break;

    case 0x30:			/* SYS_ELAPSED */
	result = -1;
	break;

    case 0x31:			/* SYS_TICKFREQ */
	result = -1;
	break;
    }

    return result;


error:
    return -1;

errorwrite:
    return len;
}


