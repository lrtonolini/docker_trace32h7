/**************************************************************************

	TRACE32 Semihosting demo for TRACE32 terminal functions and C stdio system calls

	origin file:
	  files/demo/arm/etc/semihosting_arm_syscalls/term_performance_demo.c
	
	possible defines
	  USESYSCALLS  : use stdiolib
	  USETERMCALLS : use t32term
	  USESWI       : use ARM swi instruction to do semihosting
	  BUILDARM     : use files/demo/arm/compiler/gnu-make framework
	
	other source files:
		t32term.h, t32term.c : TRACE32 terminal functions and protocol implementation
		t32term_memory.c     : TRACE32 interface for terminal functions to access BUFFERE
		syscalls.c           : translation layer from GNU C library system calls to TRACE32 terminal functions
	
	15/10/12		initial release
		
**************************************************************************/

//total size of test data pattern
#define FILESIZE (256*1024) //256KByte
//partial size for certain call to stdio/T32_Term_ routines
#define BLOCKSIZE (1024*32)

#include <stddef.h>
#include <math.h>

#ifdef USESYSCALLS
# include <stdio.h>
#endif
#ifdef USETERMCALLS
# include "t32term.h"
#endif

#if defined(BUILDARM)
#include "sieve.h"
#endif

#if !defined(USESWI)&&defined(__arm__)
#pragma import(__use_no_semihosting)
#endif

enum { FALSE = 0, TRUE = 1 };

static int mcount, vint=1;
signed short int  plot1 = -15000, plot2 = 0;

#if defined(BUILDARM)
void (*monHook)(void) __attribute__ ((section (".data"))) = 0;
extern void (*watchdogTrigger)(void);
#endif

static char pBuffer[BLOCKSIZE] = {0,};
static int blockcount = FILESIZE/sizeof(pBuffer);
static unsigned int newBUFSIZ = sizeof(pBuffer);
//setvbuf() is used to optimize the calls to T32_Term_ReadFile/T32_Term_WriteFile to reduce amount of calls to T32_Term_PutPacketEx/T32_Term_GetPacketEx
//alternativly also the macro constant BUFSIZ in stdio.h can be adapted, but this requires to recompile stdlib


/* hooks to place breakpoints for performance measurement */

#if 0
#if (defined(__arm__)&&(__ARMCC_VERSION>=5000000))||defined(__GNUC__)
# define NOP() __asm volatile ("nop")
#else
# define NOP() do { volatile int i = 0 ; (void) i; } while((0))
#endif
#endif

#define NOP() do { volatile int i = 0 ; (void) i; } while((0))

void vFatal() {
	NOP();
}

void vReadTransferStartHook() {
	NOP();
}

void vReadTransferEndHook() {
	NOP();
}

void vWriteTransferStartHook() {
	NOP();
}

void vWriteTransferEndHook() {
	NOP();
}

void vTransferHook() {
	NOP();
}

void vScenarioHook()  {
	NOP();
}

int main(void)
{
	do {
#ifdef BUILDARM
		if (monHook)
			monHook();
		if (watchdogTrigger)
			watchdogTrigger();
#endif
		mcount++;
#ifdef USESYSCALLS 

		{
			FILE*      pReadFile = NULL;
			FILE*      pWriteFile = NULL;
			size_t     result = 0;
			
			vScenarioHook();
			puts("Try to open a file \x1B[31mdumpin.bin\x1B[0m for reading\r");
			pReadFile = fopen("dumpin.bin", "rb");
			if (pReadFile) {
				int i;
				if (newBUFSIZ)
					if (setvbuf( pReadFile , NULL , _IOFBF , newBUFSIZ ) != 0) {
						puts("setvbuf failed\r");
						vFatal();
					}
				puts("Copy \x1B[31mdumpin.bin\x1B[0m -> \x1B[34mtarget\x1B[0m ...\r");
				vReadTransferStartHook();
				for (i = 0; i < blockcount; i++) {
					if ((result = fread(pBuffer, 1, sizeof(pBuffer), pReadFile)) != sizeof(pBuffer)) {
						puts("fread returned wrong size\r");
						vFatal();
					}
				}
				vReadTransferEndHook();
				puts("Copy \x1B[31mdumpin.bin\x1B[0m -> \x1B[34mtarget\x1B[0m ... finished\r");
			}
			else {
				vFatal();
			}
			if (pReadFile) {
				fclose(pReadFile);
			}
			
			vTransferHook();
			
			puts("Try to open a file \x1B[31mdumpout.bin\x1B[0m for writing\r");
			pWriteFile = fopen("dumpout.bin", "wb");
			if (pWriteFile) {
				int i;
				if (newBUFSIZ)
					if (setvbuf ( pWriteFile , NULL , _IOFBF , newBUFSIZ ) != 0) {
						puts("setvbuf failed\r");
						vFatal();
					}
				puts("Copy \x1B[34mtarget\x1B[0m -> \x1B[31mdumpout.bin\x1B[0m ...\r");
				vWriteTransferStartHook();
				for (i = 0; i < blockcount; i++) {
					if ((result = fwrite(pBuffer, 1, sizeof(pBuffer), pWriteFile)) != sizeof(pBuffer)) {
						puts("fwrite returned wrong size\r");
						vFatal();
					}
				}
				vWriteTransferEndHook();
				puts("Copy \x1B[34mtarget\x1B[0m -> \x1B[31mdumpout.bin\x1B[0m ... finished\r");
			}
			else {
				vFatal();
			}
			if (pWriteFile) {
				fclose(pWriteFile);
			}
			
			vTransferHook();

			puts("\r");
		}
#endif
#ifdef USETERMCALLS
		{
			int      hReadFile = 0;
			int      hWriteFile = 0;
			size_t     result = 0;
			
			vScenarioHook();
			T32_Term_Puts("Try to open a file \x1B[31mdumpin.bin\x1B[0m for reading\r\n");
			hReadFile = T32_Term_OpenFile("dumpin.bin", T32_TERM_O_BINARY|T32_TERM_O_RDONLY);
			if (hReadFile) {
				int i;
				T32_Term_Puts("Copy \x1B[31mdumpin.bin\x1B[0m -> \x1B[34mtarget\x1B[0m ...\r\n");
				vReadTransferStartHook();
				for (i = 0; i < blockcount; i++) {
					if ((result = T32_Term_ReadFile(hReadFile, pBuffer, sizeof(pBuffer))) != sizeof(pBuffer)) {
						T32_Term_Puts("T32_Term_ReadFile returned wrong size\r\n");
						vFatal();
					}
				}
				vReadTransferEndHook();
				T32_Term_Puts("Copy \x1B[31mdumpin.bin\x1B[0m -> \x1B[34mtarget\x1B[0m ... finished\r\n");
			}
			else {
				vFatal();
			}
			if (hReadFile) {
				T32_Term_CloseFile(hReadFile);
			}
			
			vTransferHook();
			
			T32_Term_Puts("Try to open a file \x1B[31mdumpout.bin\x1B[0m for writing\r\n");
			hWriteFile = T32_Term_OpenFile("dumpout.bin", T32_TERM_O_BINARY|T32_TERM_O_CREATE_TRUNC|T32_TERM_O_WRONLY);
			if (hWriteFile) {
				int i;
				T32_Term_Puts("Copy \x1B[34mtarget\x1B[0m -> \x1B[31mdumpout.bin\x1B[0m ...\r\n");
				vWriteTransferStartHook();
				for (i = 0; i < blockcount; i++) {
					if ((result = T32_Term_WriteFile(hWriteFile, pBuffer, sizeof(pBuffer)) != sizeof(pBuffer))) {
						T32_Term_Puts("T32_Term_WriteFile returned wrong size\r\n");
						vFatal();
					}
				}
				vWriteTransferEndHook();
				T32_Term_Puts("Copy \x1B[34mtarget\x1B[0m -> \x1B[31mdumpout.bin\x1B[0m ... finished\r\n");
			}
			else {
				vFatal();
			}
			if (hWriteFile) {
				T32_Term_CloseFile(hWriteFile);
			}
			
			vTransferHook();

			T32_Term_Puts("\n");
			
		}
#endif		
	} while (vint);

	return 0;
}

#ifdef BUILDARM
int background(void)			/* job for background-demo */
{
	static long int bcnt1, bcnt2;

	bcnt1 = bcnt2 = 0;

	while (1) {
		bcnt1 = 10000000;
		while (bcnt1>0)
			bcnt1--;
		bcnt2++;
	}

	return 0;
}
#endif

