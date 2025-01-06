// This program is used to access the registers of the MOVE coprocessor.
// The program assumes endianess=little, MOVE=coprocessor#8, word aligned 32-bit or 64-bit accesses only.
// Compiled/linked by ARM compiler: SET PATH=c:\arm251\bin; armcc -o move.elf -g -list -Otime move.c
// PEG, July 2004

struct parblock
{
    unsigned long res1;
    unsigned long res2;
    unsigned long width;
    unsigned long res3;
    unsigned long address;
    unsigned long size;
    unsigned long res4;
    unsigned long command;		// in: 5=value, 6=write, out: 0=o.k., 0x100=fail
    unsigned char data[84];
};

#define NOERROR	0
#define ERROR	0x100


void xmain (struct parblock *parameter)
{
	unsigned long status, i, value;

	// assuming word aligned accesses
	if (((*parameter).size%4 != 0) || ((*parameter).address%4 != 0) || ((*parameter).size < 4))
	{
		(*parameter).command = ERROR;
	}

	// read access
    else if ((*parameter).command == 5)
    {
		status = NOERROR;

		for (i=0; i<(*parameter).size; i+=4)
		{
			switch ((*parameter).address + i)
			{
				case 0x00:
				__asm
				{
					MRC             p8, 0, value, c0, c0
				}
				break;
				case 0x04:
				__asm
				{
					MRC             p8, 0, value, c1, c0
				}
				break;
				case 0x08:
				__asm
				{
					MRC             p8, 0, value, c2, c0
				}
				break;
				case 0x0c:
				__asm
				{
					MRC             p8, 0, value, c4, c0
				}
				break;
				case 0x10:
				__asm
				{
					MRC             p8, 0, value, c7, c0
				}
				break;
				case 0x14:
				__asm
				{
					MRC             p8, 0, value, c0, c0, 1
				}
				break;
				case 0x18:
				__asm
				{
					MRC             p8, 0, value, c1, c0, 1
				}
				break;
				case 0x1c:
				__asm
				{
					MRC             p8, 0, value, c2, c0, 1
				}
				break;
				case 0x20:
				__asm
				{
					MRC             p8, 0, value, c3, c0, 1
				}
				break;
				case 0x24:
				__asm
				{
					MRC             p8, 0, value, c4, c0, 1
				}
				break;
				case 0x28:
				__asm
				{
					MRC             p8, 0, value, c5, c0, 1
				}
				break;
				case 0x2c:
				__asm
				{
					MRC             p8, 0, value, c6, c0, 1
				}
				break;
				case 0x30:
				__asm
				{
					MRC             p8, 0, value, c7, c0, 1
				}
				break;
				case 0x34:
				__asm
				{
					MRC             p8, 0, value, c8, c0, 1
				}
				break;
				case 0x38:
				__asm
				{
					MRC             p8, 0, value, c9, c0, 1
				}
				break;
				case 0x3c:
				__asm
				{
					MRC             p8, 0, value, c10, c0, 1
				}
				break;
				case 0x40:
				__asm
				{
					MRC             p8, 0, value, c11, c0, 1
				}
				break;
				case 0x44:
				__asm
				{
					MRC             p8, 0, value, c12, c0, 1
				}
				break;
				case 0x48:
				__asm
				{
					MRC             p8, 0, value, c13, c0, 1
				}
				break;
				case 0x4c:
				__asm
				{
					MRC             p8, 0, value, c14, c0, 1
				}
				break;
				case 0x50:
				__asm
				{
					MRC             p8, 0, value, c15, c0, 1
				}
				break;
				default:
				{
					value = 0;
					status = ERROR;
				}
				break;
			}

			(*parameter).data[i] = (unsigned char)(value);
			(*parameter).data[i+1] = (unsigned char)(value >> 8);
			(*parameter).data[i+2] = (unsigned char)(value >> 16);
			(*parameter).data[i+3] = (unsigned char)(value >> 24);
		}

		(*parameter).command = status;
	}

	// write access
	else if ((*parameter).command == 6)
	{
		status = NOERROR;

		for (i=0; i<(*parameter).size; i+=4)
		{
			value = (unsigned long)((*parameter).data[i]);
			value += (unsigned long)((*parameter).data[i+1]) << 8;
			value += (unsigned long)((*parameter).data[i+2]) << 16;
			value += (unsigned long)((*parameter).data[i+3]) << 24;

			switch ((*parameter).address + i)
			{
				case 0x00:
				__asm
				{
					MCR             p8, 0, value, c0, c0
				}
				break;
				case 0x04:
				__asm
				{
					MCR             p8, 0, value, c1, c0
				}
				break;
				case 0x08:
				__asm
				{
					MCR             p8, 0, value, c2, c0
				}
				break;
				case 0x0c:
				__asm
				{
					MCR             p8, 0, value, c4, c0
				}
				break;
				case 0x10:
				__asm
				{
					MCR             p8, 0, value, c7, c0
				}
				break;
				case 0x14:
				__asm
				{
					MCR             p8, 0, value, c0, c0, 1
				}
				break;
				case 0x18:
				__asm
				{
					MCR             p8, 0, value, c1, c0, 1
				}
				break;
				case 0x1c:
				__asm
				{
					MCR             p8, 0, value, c2, c0, 1
				}
				break;
				case 0x20:
				__asm
				{
					MCR             p8, 0, value, c3, c0, 1
				}
				break;
				case 0x24:
				__asm
				{
					MCR             p8, 0, value, c4, c0, 1
				}
				break;
				case 0x28:
				__asm
				{
					MCR             p8, 0, value, c5, c0, 1
				}
				break;
				case 0x2c:
				__asm
				{
					MCR             p8, 0, value, c6, c0, 1
				}
				break;
				case 0x30:
				__asm
				{
					MCR             p8, 0, value, c7, c0, 1
				}
				break;
				case 0x34:
				__asm
				{
					MCR             p8, 0, value, c8, c0, 1
				}
				break;
				case 0x38:
				__asm
				{
					MCR             p8, 0, value, c9, c0, 1
				}
				break;
				case 0x3c:
				__asm
				{
					MCR             p8, 0, value, c10, c0, 1
				}
				break;
				case 0x40:
				__asm
				{
					MCR             p8, 0, value, c11, c0, 1
				}
				break;
				case 0x44:
				__asm
				{
					MCR             p8, 0, value, c12, c0, 1
				}
				break;
				case 0x48:
				__asm
				{
					MCR             p8, 0, value, c13, c0, 1
				}
				break;
				case 0x4c:
				__asm
				{
					MCR             p8, 0, value, c14, c0, 1
				}
				break;
				case 0x50:
				__asm
				{
					MCR             p8, 0, value, c15, c0, 1
				}
				break;
				default:
				{
					status = ERROR;
				}
				break;
			}
		}

		(*parameter).command = status;
	}

	// undefined command
	else
		(*parameter).command = ERROR;
}



