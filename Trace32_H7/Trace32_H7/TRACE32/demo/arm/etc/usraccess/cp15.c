/***********************************************************************
 Code to access the registers of coprocessor 15 via a target program
 using the TRACE32 User Controlled Memory Access (USR: memory class)

 This example accesses the registers of coprocessor 15
 by the following addressing scheme:
     addr.bit 16-14: <op1>
     addr.bit 12-10: <op2>
     addr.bit 9-6:   CRm
     addr.bit 5-2:   CRn
     addr.bit 1-0:   Byte within 32-bit coprocessor registers
 Which results in the following assembler commands:
     MCR/MRC  p15, <op1>, Rd, CRn, CRm, <op2>

 See readme.txt or look for command Data.USRACCESS in the manual.

 Compile this file as ARM code.

 $Date: 2014-01-09 19:57:21 +0100 (Thu, 09 Jan 2014) $
 $Rev: 5954 $
***********************************************************************/


#include <stdint.h>

typedef struct {
	uint32_t flashaddr;  /* reserved (for compatibility to other access types using a target binary) */
	uint32_t flashwidth; /* reserved (for compatibility to other access types using a target binary) */
	uint32_t width;      /* target memory access width */
	uint32_t offset;     /* reserved (for compatibility to other access types using a target binary) */
	uint32_t addr;       /* target memory adress */
	uint32_t size;       /* buffer size */
	uint32_t reserved;
	uint32_t status;     /* IN: 5=read, 6=write, OUT: 0=ok, 0x100=fail */
	uint8_t  data[4096]; /* Transfer Buffer, size can be made smaller (DATA.USRACCESS command) */
	/* 256 bytes of stack */
} UsrAccessParameter;

enum {
	T32_USRACCESS_IDLE  = 0x000,
	T32_USRACCESS_READ  = 0x005,
	T32_USRACCESS_WRITE = 0x006,
	T32_USRACCESS_ERROR = 0x100
};


extern uint32_t mrc(uint32_t data, uint32_t cmd);
extern uint32_t RdCtrlReg(void);
extern void     WrCtrlReg(uint32_t data);

uint32_t ReadCP15(uint32_t addr)
{
	/* MRC : Move to ARM Register from Coprocessor */
	uint32_t  cp_num = 15;
	uint32_t  CRn = (addr >>  0) & 0x0f;
	uint32_t  CRm = (addr >>  4) & 0x0f;
	uint32_t  op2 = (addr >>  8) & 0x03;
	uint32_t  op1 = (addr >> 12) & 0x07;
	uint32_t  Rd  = 0;
	uint32_t  cmd = 0xEE100010 | op1<<21 | CRn<<16 | Rd<<12 | cp_num<<8 | op2<<5 | CRm;
	if (addr == 1)
		return RdCtrlReg();
	return mrc(0,cmd);
}

void WriteCP15(uint32_t addr, uint32_t data)
{
	/* MCR : Move to Coprocessor from ARM Register */
	uint32_t  cp_num = 15;
	uint32_t  CRn = (addr >>  0) & 0x0f;
	uint32_t  CRm = (addr >>  4) & 0x0f;
	uint32_t  op2 = (addr >>  8) & 0x03;
	uint32_t  op1 = (addr >> 12) & 0x07;
	uint32_t  Rd  = 0;
	uint32_t  cmd = 0xEE000010 | op1<<21 | CRn<<16 | Rd<<12 | cp_num<<8 | op2<<5 | CRm;
	if (addr == 1)
		WrCtrlReg(data);
	else
		mrc(data,cmd);
}


__attribute__ ((section (".first")))
UsrAccessParameter * usraccess(UsrAccessParameter * param)
{
	int       i, nibble;
	int       size = param->size;
	uint32_t  addr = param->addr;
	uint32_t  data32 = 0;

	if (param->status == T32_USRACCESS_READ) { /* data read ? */
		for ( i=0; i<size; i++ ){
			nibble = (addr + i) % 4;

			if ( i==0 || nibble==0 ) {
				data32 = ReadCP15(addr/4);
				addr += 4;
			}
			param->data[i] = (data32 >> (8*nibble)) & 0xff; /* copy data bytes to buffer in little endian order */
		}
	} else if (param->status == T32_USRACCESS_WRITE) {  /* data write ? */
		const uint32_t mask[4] = {0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000};

		for ( i=0; i<size; i++ ) {
			nibble = (addr + i) % 4;

			if ( (i == 0  &&  nibble != 0)  ||  (size-i < 4  &&  nibble == 0) ) {
				data32 = ReadCP15(addr/4);
			}

			data32 = (data32 & ~mask[nibble]) | ((param->data[i]<<(8*nibble)) & mask[nibble]); /* copy data32 from buffer in little endian order */

			if ( nibble+1 == 4  ||  i+1 == size ) {
				WriteCP15(addr/4, data32);
				addr += 4;
			}
		}
	}

	param->status = T32_USRACCESS_IDLE;
	return param;
}

