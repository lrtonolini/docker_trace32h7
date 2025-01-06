
#include <string.h>


/**************************************************************************

	TRACE32 ARM SWI handler for Semihosting via DCC

	03/12/30		initial release
	09/10/22		cmdline support added
		
**************************************************************************/


extern unsigned long T32_TsMon_SendStatus(void);
extern void T32_TsMon_SendWord(unsigned long data);
extern unsigned long T32_TsMon_ReceiveStatus(void);
extern unsigned long T32_TsMon_ReceiveWord(void);


static unsigned char TerminalTypeAheadBuffer[32];
static int TerminalTypeAheadNumber = 0;


/**************************************************************************

	Receive/Send raw data blocks
			
**************************************************************************/

static void TerminalPutBlock(unsigned char * data, int size)
{
    int             len;
    unsigned long   ch;

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


static void TerminalGetBlock(unsigned char * data, int size)
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


/**************************************************************************

	Receive/Send protocol data blocks (with typeahead buffering)
			
**************************************************************************/

static void TerminalPutPacket(unsigned char * data, int len)
{
    int             i;
    unsigned char   sum;

    data[-2] = 2;
    data[-1] = len;
    sum = len;
    for (i = 0; i < len; i++)
	sum += data[i];
    data[len] = sum;
    data[len + 1] = 3;
    TerminalPutBlock(data - 2, len + 4);
}


static int TerminalGetPacket(unsigned char * data)
{
    int             i, len;
    unsigned char   sum;

    while (1) {
	TerminalGetBlock(data, 1);
	if (data[0] == 2)
	    break;
	if (TerminalTypeAheadNumber >= sizeof(TerminalTypeAheadBuffer))
	    return -1;
	TerminalTypeAheadBuffer[TerminalTypeAheadNumber++] = data[0];
    }

    TerminalGetBlock(data, 1);
    len = data[0];

    TerminalGetBlock(data, len + 2);
    if (data[len + 1] != 3)
	return -1;

    sum = len;
    for (i = 0; i < len; i++)
	sum += data[i];

    if (sum != data[i])
	return -1;

    return len;
}



/**************************************************************************

	Console input / output
			
**************************************************************************/

static int TerminalGetchar()
{
    int             i, ch;
    unsigned char   imsg[4];

    if (TerminalTypeAheadNumber) {
	ch = TerminalTypeAheadBuffer[0];
	TerminalTypeAheadNumber--;
	for (i = 0; i < TerminalTypeAheadNumber; i++)
	    TerminalTypeAheadBuffer[i] = TerminalTypeAheadBuffer[i + 1];
    } else {
	TerminalGetBlock(imsg, 1);
	ch = imsg[0];
    }

    return ch;
}


static void TerminalPutchar(int ch)
{
    unsigned char   msg[4];

    msg[0] = ch;
    TerminalPutBlock(msg, 1);
}


static int TerminalGetline(char * buffer, int len)
{
    int             i, blen;
    unsigned char   msg[256];

    if (len <= 0 || len >= 256)
	return -1;

    msg[2] = 0x27;
    msg[3] = len;
    TerminalPutPacket(msg + 2, 2);

    if ((blen = TerminalGetPacket(msg)) == -1)
	return -1;
    if (msg[0])
	return -1;

    blen--;

    for (i = 0; i < blen; i++)
	buffer[i] = msg[i + 1];

    return blen;
}


static void TerminalPuts(char * buffer)
{
    TerminalPutBlock((unsigned char *) buffer, strlen(buffer));
}



/**************************************************************************

	File I/O
			
**************************************************************************/

static int TerminalOpenFile(char * fname, int mode)
{
    unsigned char   msg[256];

    msg[2] = 0x01;
    msg[3] = mode;
    msg[4] = 0;
    strcpy((char *) msg + 5, fname);
    TerminalPutPacket(msg + 2, strlen(fname) + 3);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;
    return msg[1];
}


static int TerminalCloseFile(int handle)
{
    unsigned char   msg[256];

    msg[2] = 0x02;
    msg[3] = handle;
    TerminalPutPacket(msg + 2, 2);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;
    return 0;
}


static int TerminalReadFile(int handle, char * buffer, int len)
{
    int             i, blen, rlen;
    unsigned char   msg[256];

    rlen = 0;
    while (len > 0) {
	blen = len;
	if (blen > 240)
	    blen = 240;

	msg[2] = 0x03;
	msg[3] = handle;
	msg[4] = blen;
	TerminalPutPacket(msg + 2, 3);

	if ((blen = TerminalGetPacket(msg)) == -1)
	    return -1;
	if (msg[0])
	    return -1;

	blen--;
	if (blen == 0)
	    break;

	for (i = 0; i < blen; i++)
	    buffer[i] = msg[i + 1];

	buffer += blen;
	len -= blen;
	rlen += blen;
    }

    return rlen;
}


static int TerminalWriteFile(int handle, char * buffer, int len)
{
    int             i, blen, wlen;
    unsigned char   msg[256];

    wlen = 0;
    while (len > 0) {
	blen = len;
	if (blen > 240)
	    blen = 240;

	msg[2] = 0x04;
	msg[3] = handle;
	for (i = 0; i < blen; i++)
	    msg[i + 4] = buffer[i];
	TerminalPutPacket(msg + 2, 2 + blen);

	if (TerminalGetPacket(msg) == -1)
	    return -1;
	if (msg[0])
	    return -1;

	blen = msg[1];
	if (blen == 0)
	    break;

	buffer += blen;
	len -= blen;
	wlen += blen;
    }

    return wlen;
}


static int TerminalSeekFile(int handle, long pos)
{
    unsigned char   msg[256];

    msg[2] = 0x05;
    msg[3] = handle;
    msg[4] = (unsigned char) (pos);
    msg[5] = (unsigned char) (pos >> 8);
    msg[6] = (unsigned char) (pos >> 16);
    msg[7] = (unsigned char) (pos >> 24);

    TerminalPutPacket(msg + 2, 6);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;

    return ((long) msg[1]) | (((long) msg[2]) << 8) | (((long) msg[3]) << 16) | (((long) msg[4]) << 24);
}


static int TerminalTellSizeFile(int handle)
{
    unsigned char   msg[256];

    msg[2] = 0x07;
    msg[3] = handle;

    TerminalPutPacket(msg + 2, 2);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;

    return ((long) msg[1]) | (((long) msg[2]) << 8) | (((long) msg[3]) << 16) | (((long) msg[4]) << 24);
}


/**************************************************************************

	File/OS Functions
			
**************************************************************************/

static int TerminalGetTempNameID(char * name, int id)
{
    int             i, blen;
    unsigned char   msg[256];

    msg[2] = 0x26;
    msg[3] = id;

    TerminalPutPacket(msg + 2, 2);

    if ((blen = TerminalGetPacket(msg)) == -1)
	return -1;
    if (msg[0])
	return -1;

    blen--;

    for (i = 0; i < blen; i++)
	name[i] = msg[i + 1];
    name[i] = '\0';

    return 0;
}


static int TerminalRemove(char * fname)
{
    unsigned char   msg[256];

    msg[2] = 0x11;
    strcpy((char *) msg + 3, fname);
    TerminalPutPacket(msg + 2, strlen(fname) + 1);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;
    return 0;
}


static int TerminalRename(char * fname, char * fnamenew)
{
    int             len, lennew;
    unsigned char   msg[256];

    len = strlen(fname);
    lennew = strlen(fnamenew);

    msg[2] = 0x12;
    msg[3] = len;
    strcpy((char *) msg + 4, fname);
    strcpy((char *) msg + 4 + len, fname);
    TerminalPutPacket(msg + 2, 2 + len + lennew);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;
    return 0;
}



/**************************************************************************

	System Functions
			
**************************************************************************/

static int TerminalGetCmdline(char * cmdline)
{
    int             i, blen;
    unsigned char   msg[256];

    msg[2] = 0x28;

    TerminalPutPacket(msg + 2, 1);

    if ((blen = TerminalGetPacket(msg)) == -1)
	return -1;
    if (msg[0])
	return -1;

    blen--;

    for (i = 0; i < blen; i++)
	cmdline[i] = msg[i + 1];
    cmdline[i] = '\0';

    return blen;
}


static int TerminalHeapInfo(unsigned long * params)
{
    int             i, j;
    unsigned char   msg[256];

    msg[2] = 0x25;

    TerminalPutPacket(msg + 2, 1);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;

    for (j = 0, i = 1; j < 4; j++, i += 4)
	params[j] = ((long) msg[i]) | (((long) msg[i + 1]) << 8) | (((long) msg[i + 2]) << 16) | (((long) msg[i + 3]) << 24);

    return 0;
}


static long TerminalGetClock()
{
    unsigned char   msg[256];

    msg[2] = 0x23;

    TerminalPutPacket(msg + 2, 1);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;

    return ((long) msg[1]) | (((long) msg[2]) << 8) | (((long) msg[3]) << 16) | (((long) msg[4]) << 24);
}


static long TerminalGetTime()
{
    unsigned char   msg[256];

    msg[2] = 0x22;

    TerminalPutPacket(msg + 2, 1);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;

    return ((long) msg[1]) | (((long) msg[2]) << 8) | (((long) msg[3]) << 16) | (((long) msg[4]) << 24);
}


static int TerminalOsExecute(char * fname, int mode)
{
    unsigned char   msg[256];

    msg[2] = 0x24;
    msg[3] = mode;
    strcpy((char *) msg + 4, fname);
    TerminalPutPacket(msg + 2, strlen(fname) + 2);

    if (TerminalGetPacket(msg) == -1)
	return -1;
    if (msg[0])
	return -1;
    return 0;
}


static void TerminalBreak()
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

	    handle = TerminalOpenFile(filename, mode);
	    if (handle == -1)
		goto error;

	    result = handle + 4;
	}

	break;

    case 0x02:			/* SYS_CLOSE */
	handle = (int) parameter[0];

	if (handle > 4) {
	    result = TerminalCloseFile(handle - 4);
	    if (handle == -1)
		goto error;
	} else {
	    result = 0;
	}

	break;

    case 0x03:			/* SYS_WRITEC */
	TerminalPutchar(*((char *) parameter));
	result = 0;
	break;

    case 0x04:			/* SYS_WRITE0 */
	TerminalPuts(((char *) parameter));
	result = 0;
	break;

    case 0x05:			/* SYS_WRITE */
	handle = (int) parameter[0];
	data = (char *) parameter[1];
	len = (int) parameter[2];

	if (handle < 4) {
	    for (i = 0; i < len; i++) {
		if (data[i] == '\n')
		    TerminalPutchar('\r');
		TerminalPutchar(data[i]);
	    }
	} else {
	    result = TerminalWriteFile(handle - 4, data, len);
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
	    result = TerminalGetline(data, len - 1);
	    if (result == -1)
		goto errorwrite;
	    data[result++] = '\n';
	} else {
	    result = TerminalReadFile(handle - 4, data, len);
	    if (result == -1)
		goto errorwrite;
	}
	result = len - result;
	break;

    case 0x07:			/* SYS_READC */
	result = TerminalGetchar();

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
	pos = TerminalSeekFile(handle - 4, pos);
	if (pos == -1)
	    goto error;
	result = 0;
	break;

    case 0x0c:			/* SYS_FLEN */
	handle = (int) parameter[0];
	if (handle < 4)
	    goto error;
	pos = TerminalTellSizeFile(handle - 4);
	if (pos == -1)
	    goto error;
	result = pos;
	break;

    case 0x0d:			/* SYS_TMPNAM */
	data = (char *) parameter[0];
	i = (int) parameter[1];
	len = (int) parameter[2];
	result = TerminalGetTempNameID(data, i);
	if (strlen(data) + 1 > len)
	    goto error;
	break;

    case 0x0e:			/* SYS_REMOVE */
	filename = (char *) parameter[0];
	len = (int) parameter[1];
	if (strlen(filename) != len)
	    goto error;
	result = TerminalRemove(filename);
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
	result = TerminalRename(filename, filename2);
	break;

    case 0x10:			/* SYS_CLOCK */
	result = TerminalGetClock();
	break;

    case 0x11:			/* SYS_TIME */
	result = TerminalGetTime();
	break;

    case 0x12:			/* SYS_SYSTEM */
	filename = (char *) parameter[0];
	len = (int) parameter[1];

	if (strlen(filename) != len)
	    goto error;

	result = TerminalOsExecute(filename, 2);
	break;

    case 0x13:			/* SYS_ERRNO */
	result = -1;
	break;

    case 0x15:			/* SYS_GET_CMDLINE */
	cmdline = (char *) parameter[0];
	len = (int) parameter[1];

	result = TerminalGetCmdline(cmdline);

	if (result > len)
	    result = -1;
	else
	    result = 0;
	break;

    case 0x16:			/* SYS_HEAPINFO */
	heapinfo = (unsigned long *) parameter[0];
	if (TerminalHeapInfo(heapinfo) == -1)
	    goto error;
	result = 0;
	break;

    case 0x18:			/* SYS_STOP */
	TerminalBreak();
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


