/**************************************************************************

	ARM family dependent DCC driver functions

**************************************************************************/

extern unsigned int DCC_SendStatus (void);
extern void DCC_SendWord (unsigned int data);
extern unsigned int DCC_ReceiveStatus (void);
extern unsigned int DCC_ReceiveWord (void);


/**************************************************************************

	memory access

**************************************************************************/

static void Monitor_ReadByte (void * buf, void * address, int len)
{
	int i;
	unsigned char *source = (unsigned char *) address;
	unsigned char *target = (unsigned char *) buf;

	for (i = 0; i < len; i++) {
		target[i] = source[i];
	}
}


static void Monitor_ReadHalf (void * buf, void * address, int len)
{
	int i;
	unsigned short *source = (unsigned short *) address;
	unsigned short *target = (unsigned short *) buf;

	for (i = 0; i < len; i++) {
		target[i] = source[i];
	}
}


static void Monitor_ReadWord (void * buf, void * address, int len)
{
	int i;
	unsigned int *source = (unsigned int *) address;
	unsigned int *target = (unsigned int *) buf;

	for (i = 0; i < len; i++) {
		target[i] = source[i];
	}
}


static void Monitor_WriteByte (void * buf, void * address, int len)
{
	int i;
	unsigned char *source = (unsigned char *) address;
	unsigned char *target = (unsigned char *) buf;

	for (i = 0; i < len; i++) {
		target[i] = source[i];
	}
}


static void Monitor_WriteHalf (void * buf, void * address, int len)
{
	int i;
	unsigned short *source = (unsigned short *) address;
	unsigned short *target = (unsigned short *) buf;

	for (i = 0; i < len; i++) {
		target[i] = source[i];
	}
}


static void Monitor_WriteWord (void * buf, void * address, int len)
{
	int i;
	unsigned int *source = (unsigned int *) address;
	unsigned int *target = (unsigned int *) buf;

	for (i = 0; i < len; i++) {
		target[i] = source[i];
	}
}


/**************************************************************************

	CP15 access

	Can be extended appropriate to your need.

**************************************************************************/

#ifdef __GNUC__
# define MRC(CORPROC,OPCODE,RD,CRN,CRM) __asm__ ("mrc " #CORPROC "," #OPCODE ",%[dst]," #CRN "," #CRM : [dst] "=r" (RD))
# define MCR(CORPROC,OPCODE,RD,CRN,CRM) __asm__ ("mcr " #CORPROC "," #OPCODE ",%[src]," #CRN "," #CRM :: [src] "r" (RD))
#else
# define MRC(CORPROC,OPCODE,RD,CRN,CRM) __asm { mrc CORPROC,OPCODE,RD,CRN,CRM }
# define MCR(CORPROC,OPCODE,RD,CRN,CRM) __asm { mcr CORPROC,OPCODE,RD,CRN,CRM }
#endif


static unsigned int Monitor_ReadCP15 (unsigned int address)
{
	unsigned int data = 0;

	switch (address) {
		case 0xf000:
			MRC( p15, 0, data, c0, c0 );
			break;
		case 0xf001:
			MRC( p15, 0, data, c1, c0 );
			break;
		case 0xf002:
			MRC( p15, 0, data, c2, c0 );
			break;
		case 0xf003:
			MRC( p15, 0, data, c3, c0 );
			break;
		case 0xf004:
			MRC( p15, 0, data, c4, c0 );
			break;
		case 0xf005:
			MRC( p15, 0, data, c5, c0 );
			break;
	}
	return data;
}


static void Monitor_WriteCP15 (unsigned int address, unsigned int data)
{
	switch (address) {
		case 0xf000:
			MCR( p15, 0, data, c0, c0 );
			break;
		case 0xf001:
			MCR( p15, 0, data, c1, c0 );
			break;
		case 0xf002:
			MCR( p15, 0, data, c2, c0 );
			break;
		case 0xf003:
			MCR( p15, 0, data, c3, c0 );
			break;
		case 0xf004:
			MCR( p15, 0, data, c4, c0 );
			break;
		case 0xf005:
			MCR( p15, 0, data, c5, c0 );
			break;
	}
}


/**************************************************************************

	Monitor

**************************************************************************/

static unsigned int Monitor_AddressHigh, Monitor_AddressLow, Monitor_Buffer[16];
static int Monitor_Index, Monitor_Count;

void Monitor_Handler(void)
{
	int index, len;
	unsigned int address, data;

	// receive data from debugger via DCC channel
	if (DCC_ReceiveStatus()) {
		data = DCC_ReceiveWord();

		switch (data >> 28) {
			case 0x01: // 8-bit read access
				len = ((data >> 24) & 0x0f) + 1;
				address = (Monitor_AddressLow & ~0xffffff) | (data & 0xffffff);
				Monitor_ReadByte (Monitor_Buffer, (void *) address, len);
				Monitor_Index = 0;
				Monitor_Count = len;
				break;

			case 0x02: // 16-bit read access
				len = ((data >> 24) & 0x0f) + 1;
				address = (Monitor_AddressLow & ~0xffffff) | (data & 0xffffff);
				Monitor_ReadHalf (Monitor_Buffer, (void *) address, len);
				Monitor_Index = 0;
				Monitor_Count = len * 2;
				break;

			case 0x03: // 32-bit read access
				len = ((data >> 24) & 0x0f) + 1;
				address = (Monitor_AddressLow & ~0xffffff) | (data & 0xffffff);
				Monitor_ReadWord (Monitor_Buffer, (void *) address, len);
				Monitor_Index = 0;
				Monitor_Count = len * 4;
				break;

			case 0x04: // 8-bit write access
				len = ((data >> 24) & 0x0f) + 1;
				((unsigned char *) Monitor_Buffer)[len - 1] = (data >> 16) & 0xff;
				address = (Monitor_AddressLow & ~0xffff) | (data & 0xffff);
				Monitor_WriteByte ((void *) address, Monitor_Buffer, len);
				break;

			case 0x05: // 16-bit write access
				len = ((data >> 24) & 0x0f) + 1;
				((unsigned char *) Monitor_Buffer)[len * 2 - 1] = (data >> 16) & 0xff;
				address = (Monitor_AddressLow & ~0xffff) | (data & 0xffff);
				Monitor_WriteHalf ((void *) address, Monitor_Buffer, len);
				break;

			case 0x06: // 32-bit write access
				len = ((data >> 24) & 0x0f) + 1;
				((unsigned char *) Monitor_Buffer)[len * 4 - 1] = (data >> 16) & 0xff;
				address = (Monitor_AddressLow & ~0xffff) | (data & 0xffff);
				Monitor_WriteWord ((void *) address, Monitor_Buffer, len);
				break;

			case 0x07: // 32-bit CP15 read access
				Monitor_Buffer[0] = Monitor_ReadCP15 (data & 0xffff);
				Monitor_Index = 0;
				Monitor_Count = 4;
				break;

			case 0x08: // 32-bit CP15 write access
				((unsigned char *) Monitor_Buffer)[4 - 1] = (data >> 16) & 0xff;
				Monitor_WriteCP15 (data & 0xffff, Monitor_Buffer[0]);
				break;

			case 0x0d: // set address
				if ((data & 0x01000000) == 0){
					/* Bits 16..39 */
					Monitor_AddressLow = (data << 16);
					Monitor_AddressHigh = (Monitor_AddressHigh & ~0xff) | ((data >> 16) & 0xff);
				} else {
					/* Bits 40..63 */
					Monitor_AddressHigh = (Monitor_AddressHigh & ~0xffffff00) | ((data << 8) & 0xffffff00);
				}
				break;

			case 0x0e: // set data to buffer
			case 0x0f:
				index = ((data >> 24) & 0x1f) * 3;
				if (index < 21) {
					((unsigned char *) Monitor_Buffer)[index] = data & 0xff;
					((unsigned char *) Monitor_Buffer)[index + 1] = (data >> 8) & 0xff;
					((unsigned char *) Monitor_Buffer)[index + 2] = (data >> 16) & 0xff;
				}
				break;
		}
	}

	// send data from debugger via DCC channel
	if (Monitor_Index < Monitor_Count && !DCC_SendStatus()) {
		data = (((unsigned char *) Monitor_Buffer)[Monitor_Index]) | (((unsigned char *) Monitor_Buffer)[Monitor_Index + 1] << 8) | (((unsigned char *) Monitor_Buffer)[Monitor_Index + 2] << 16) | 0x10000000;
		DCC_SendWord(data);
		Monitor_Index += 3;
	}
}


