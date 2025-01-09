/***********************************************************************
 Code to access memory via a target program
 using the TRACE32 User Controlled Memory Access (USR: memory class)

 This example directly accesses the main memory as seen by the CPU.

 See readme.txt or look for command Data.USRACCESS in the manual.

 Compile this file as ARM code (unless you are using a Cortex-M).
 For Cortex-M compile the file as Thumb code.
 (Or use junction.sx to compile this file as Thumb code for all ARM cores.)

 $Date: 2014-01-09 19:55:17 +0100 (Thu, 09 Jan 2014) $
 $Rev: 5953 $
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

UsrAccessParameter * usraccess(UsrAccessParameter * param)
{
	int         size = param->size;
	uint8_t     *data8,  *addr8;
	int16_t     *data16, *addr16;
	uint32_t    *data32, *addr32;

	if (param->status == T32_USRACCESS_READ) { /* data read ? */
		switch (param->width) {
			case 2:
				data16 = (int16_t *) param->data;
				addr16 = (int16_t *) param->addr;
				while (size > 0) {
					*data16++ = *addr16++;
					size -= 2;
				}
				break;
			case 4:
				data32 = (uint32_t *) param->data;
				addr32 = (uint32_t *) param->addr;
				while (size > 0) {
					*data32++ = *addr32++;
					size -= 4;
				}
				break;
			default:
				data8 = (uint8_t *) param->data;
				addr8 = (uint8_t *) param->addr;
				while (size > 0) {
					*data8++ = *addr8++;
					size -= 1;
				}
				break;
		}
	} else if (param->status == T32_USRACCESS_WRITE) {  /* data write ? */
		switch (param->width) {
			case 2:
				data16 = (int16_t *) param->data;
				addr16 = (int16_t *) param->addr;
				while (size > 0) {
					*addr16++ = *data16++;
					size -= 2;
				}
				break;
			case 4:
				data32 = (uint32_t *) param->data;
				addr32 = (uint32_t *) param->addr;
				while (size > 0) {
					*addr32++ = *data32++;
					size -= 4;
				}
				break;
			default:
				data8 = (uint8_t *) param->data;
				addr8 = (uint8_t *) param->addr;
				while (size > 0) {
					*addr8++ = *data8++;
					size -= 1;
				}
				break;
		}
	}
	param->status = T32_USRACCESS_IDLE;
	return param;
}

