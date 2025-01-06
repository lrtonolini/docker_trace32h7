

/**************************************************************************

	Benchmarking routines for the Data.BENCHMARK command

	Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only

	NOTE:	Benchmarking is done by comparing the run times of
		two similar functions. Make sure that the code generated
		by the compiler is REALLY similar (even the alignment
		of code to internal buses may effect runtime).
		The modification of the formula for the return value can
		be used to trim	the functions for proper alignment.
		The optimization level should be not too high to
		prevent the side effects of unwanted optimisation.

**************************************************************************/

#define SIMPLEINSTLEN	4		/* length of basic instruction used for I-Cache testing */

#define CACHE_LINESIZE	0x20		/* maximum length of a cache line in bytes */

typedef unsigned long TESTDATA;		/* data type for bandwidth testing */
typedef unsigned long TESTDATAWIDE;
typedef unsigned char TESTDATA2;	/* data type for random and latency testing */


typedef struct
{
    TESTDATA x0;
    TESTDATA x1;
    TESTDATA x2;
    TESTDATA x3;
}
TESTSTRUCT;


/**************************************************************************

	Data structure for passing parameters to routines

**************************************************************************/

typedef struct
{
    volatile TESTDATA * memory;		/* data memory address */
    int reserved;
    int size;				/* memory size in bytes */
    int test;				/* test number */
    int count;				/* repeat count */
    int latency;			/* used in latency test function to wait till next cycle */
    int reserved2;
    int offset;				/* offset to second memory block for copy/random tests, offset for latency */
}
TESTPARAMETER;


void BenchmarkTest(TESTPARAMETER * params);

static int FetchBandwidthTest1(volatile TESTDATA * memory, int count);
static int FetchBandwidthTest2(volatile TESTDATA * memory, int count);
static int ReadBandwidthTest1(volatile TESTDATAWIDE * memory, int count);
static int ReadBandwidthTest2(volatile TESTDATAWIDE * memory, int count);
static int WriteBandwidthTest1(volatile TESTDATAWIDE * memory, int count);
static int WriteBandwidthTest2(volatile TESTDATAWIDE * memory, int count);
static int CopyBandwidthTest1(volatile TESTDATAWIDE * memory, int count, int offset);
static int CopyBandwidthTest2(volatile TESTDATAWIDE * memory, int count, int offset);
static int ReadRandomTest1(volatile TESTDATA2 * memory, int count);
static int ReadRandomTest2(volatile TESTDATA2 * memory, int count);
static int WriteRandomTest1(volatile TESTDATA2 * memory, int count);
static int WriteRandomTest2(volatile TESTDATA2 * memory, int count);
static int CopyRandomTest1(volatile TESTDATA2 * memory, int count, int offset);
static int CopyRandomTest2(volatile TESTDATA2 * memory, int count, int offset);
static int ReadLatencyTestNear1(volatile TESTDATA2 * memory, int count, int latencymax, int offset);
static int ReadLatencyTestNear2(volatile TESTDATA2 * memory, int count, int latencymax, int offset);
static int ReadLatencyTestFar1(volatile TESTDATA2 * memory, int count, int latencymax, int offset);
static int ReadLatencyTestFar2(volatile TESTDATA2 * memory, int count, int latencymax, int offset);

extern int DhryStoneTest(unsigned char * memory, int count);


/**************************************************************************

	Main test entry

**************************************************************************/

void BenchmarkTest(TESTPARAMETER * params)
{
    int             i, x, test, size, count, latency, offset;
    volatile TESTDATA *memory;
    volatile TESTDATA2 *memory2;
    volatile TESTDATAWIDE *memorywide;

    memory = params->memory;
    memorywide = (TESTDATAWIDE *) memory;
    memory2 = (TESTDATA2 *) memory;
    size = params->size;
    test = params->test;
    count = params->count;
    latency = params->latency;
    offset = params->offset;
    x = 0;

    switch (test) {
    case 1:
	/* D-Cache Read test */
	for (i = 0; i < count; i++) {
	    x += ReadBandwidthTest1(memorywide, size / sizeof(TESTDATAWIDE));
	}
	break;
    case 2:
	/* D-Cache Read test reference */
	for (i = 0; i < count; i++) {
	    x += ReadBandwidthTest2(memorywide, size / sizeof(TESTDATAWIDE));
	}
	break;
    case 3:
	/* D-Cache Write test */
	for (i = 0; i < count; i++) {
	    x += WriteBandwidthTest1(memorywide, size / sizeof(TESTDATAWIDE));
	}
	break;
    case 4:
	/* D-Cache Write test reference */
	for (i = 0; i < count; i++) {
	    x += WriteBandwidthTest2(memorywide, size / sizeof(TESTDATAWIDE));
	}
	break;
    case 5:
	/* D-Cache Copy test */
	for (i = 0; i < count; i++) {
	    x += CopyBandwidthTest1(memorywide, size / sizeof(TESTDATAWIDE), offset / sizeof(TESTDATAWIDE));
	}
	break;
    case 6:
	/* D-Cache Copy test reference */
	for (i = 0; i < count; i++) {
	    x += CopyBandwidthTest2(memorywide, size / sizeof(TESTDATAWIDE), offset / sizeof(TESTDATAWIDE));
	}
	break;
    case 7:
	/* D-Cache Random Read test */
	for (i = 0; i < count; i++) {
	    x += ReadRandomTest1(memory2, size / sizeof(TESTDATA2));
	}
#if CACHE_LINESIZE < 0x40
	params->size = (size / 0x400) * 32;	/* number of reads done */
#else
	params->size = (size / 0x400) * 16;	/* number of reads done */
#endif
	break;
    case 8:
	/* D-Cache Random Read test reference */
	for (i = 0; i < count; i++) {
	    x += ReadRandomTest2(memory2, size / sizeof(TESTDATA2));
	}
	break;
    case 9:
	/* D-Cache Random Write test */
	for (i = 0; i < count; i++) {
	    x += WriteRandomTest1(memory2, size / sizeof(TESTDATA2));
	}
	break;
    case 10:
	/* D-Cache Random Write test reference */
	for (i = 0; i < count; i++) {
	    x += WriteRandomTest2(memory2, size / sizeof(TESTDATA2));
	}
	break;
    case 11:
	/* D-Cache Random Copy test */
	for (i = 0; i < count; i++) {
	    x += CopyRandomTest1(memory2, size / sizeof(TESTDATA2), offset / sizeof(TESTDATA2));
	}
	break;
    case 12:
	/* D-Cache Random Copy test reference */
	for (i = 0; i < count; i++) {
	    x += CopyRandomTest2(memory2, size / sizeof(TESTDATA2), offset / sizeof(TESTDATA2));
	}
	break;
    case 13:
	/* D-Cache Latency test */
	for (i = 0; i < count; i++) {
	    x += ReadLatencyTestFar1(memory2, size / sizeof(TESTDATA2), latency, offset / sizeof(TESTDATA2));
	}
	break;
    case 14:
	/* D-Cache Latency test reference */
	for (i = 0; i < count; i++) {
	    x += ReadLatencyTestFar2(memory2, size / sizeof(TESTDATA2), latency, offset / sizeof(TESTDATA2));
	}
	break;
    case 15:
	/* D-Cache Latency test */
	for (i = 0; i < count; i++) {
	    x += ReadLatencyTestNear1(memory2, size / sizeof(TESTDATA2), latency, offset / sizeof(TESTDATA2));
	}
	break;
    case 16:
	/* D-Cache Latency test reference */
	for (i = 0; i < count; i++) {
	    x += ReadLatencyTestNear2(memory2, size / sizeof(TESTDATA2), latency, offset / sizeof(TESTDATA2));
	}
	break;

    case 0x101:
	/* I-Cache Fetch test */
	for (i = 0; i < count; i++) {
	    x += FetchBandwidthTest1(memory, size / SIMPLEINSTLEN);
	}
	params->size = size / SIMPLEINSTLEN;	/* number of instructions
						 * executed */
	break;
    case 0x102:
	/* I-Cache Fetch test reference */
	for (i = 0; i < count; i++) {
	    x += FetchBandwidthTest2(memory, size / SIMPLEINSTLEN);
	}
	break;

    case 0x200:
	/* Dhrystone benchmark */
	x = DhryStoneTest(((unsigned char *) params)+0x100, count);
	break;

    case 0x303:
	/* D-Cache Write test via libc */
	for (i = 0; i < count; i++) {
	    memset(memory, 0, size);
	}
	break;
    case 0x304:
	/* D-Cache Write test reference via libc */
	for (i = 0; i < count; i++) {
	    memset(memory, 0, size / 2);
	}
	break;
    case 0x305:
	/* D-Cache Copy test via libc */
	for (i = 0; i < count; i++) {
	    memcpy(memory, memory + offset / sizeof(TESTDATA), size);
	}
	break;
    case 0x306:
	/* D-Cache Copy test reference via libc */
	for (i = 0; i < count; i++) {
	    memcpy(memory, memory + offset / sizeof(TESTDATA), size / 2);
	}
	break;

    default:
	return;
    }

    params->reserved = x;
    params->test = 0;
}


/**************************************************************************

	I-Cache and Peak MIPS test functions

**************************************************************************/

static int FetchBandwidthTest1(volatile TESTDATA * memory, int count)
{
    register TESTDATA x0, x1, x2, x3, x4, x5, x6, x7;

    x0 = memory[0];
    x1 = memory[1];
    x2 = memory[2];
    x3 = memory[3];
    x4 = 0;
    x5 = 0;
    x6 = 0;
    x7 = 0;

    while (count > 0) {
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	count -= 64;
    }

    return x0 + x1 + x2 + x4 + x5 + x6 + x7;
}


static int FetchBandwidthTest2(volatile TESTDATA * memory, int count)
{
    register TESTDATA x0, x1, x2, x3, x4, x5, x6, x7;

    x0 = memory[0];
    x1 = memory[1];
    x2 = memory[2];
    x3 = memory[3];
    x4 = 0;
    x5 = 0;
    x6 = 0;
    x7 = 0;

    while (count > 0) {
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	x0 = x0 << 1;
	x1 = x1 >> 1;
	x2 = x2 << 1;
	x3 = x3 >> 1;
	x4 |= x0;
	x5 |= x1;
	x6 |= x2;
	x7 |= x3;
	count -= 64;
    }

    return x0 + x1 + x2 + x4 + x5 + x6 + x7;
}



/**************************************************************************

	Block Read test functions

**************************************************************************/

#ifdef STRUCT_COPY

static int ReadBandwidthTest1(volatile TESTDATAWIDE * memory, int count)
{
    register TESTSTRUCT x;

    while (count > 0) {
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	memory += 8 * sizeof(TESTSTRUCT) / sizeof(TESTDATA);
	count -= 8 * sizeof(TESTSTRUCT) / sizeof(TESTDATA);
    }

    return (int) memory;
}


static int ReadBandwidthTest2(volatile TESTDATA * memory, int count)
{
    register TESTSTRUCT x;

    while (count > 0) {
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	x = *((volatile TESTSTRUCT *) memory);
	memory += 8 * sizeof(TESTSTRUCT) / sizeof(TESTDATA);
	count -= 8 * sizeof(TESTSTRUCT) / sizeof(TESTDATA);
    }

    return (int) memory;
}

#else

static int ReadBandwidthTest1(volatile TESTDATAWIDE * memory, int count)
{
    register TESTDATAWIDE x0 = 0;
    register TESTDATAWIDE x1 = 0;
    register TESTDATAWIDE x2 = 0;
    register TESTDATAWIDE x3 = 0;

    while (count > 0) {
	x0 = memory[0];
	x1 = memory[1];
	x2 = memory[2];
	x3 = memory[3];
	x0 = memory[4];
	x1 = memory[5];
	x2 = memory[6];
	x3 = memory[7];
	x0 = memory[8];
	x1 = memory[9];
	x2 = memory[10];
	x3 = memory[11];
	x0 = memory[12];
	x1 = memory[13];
	x2 = memory[14];
	x3 = memory[15];
	x0 = memory[16];
	x1 = memory[17];
	x2 = memory[18];
	x3 = memory[19];
	x0 = memory[20];
	x1 = memory[21];
	x2 = memory[22];
	x3 = memory[23];
	x0 = memory[24];
	x1 = memory[25];
	x2 = memory[26];
	x3 = memory[27];
	x0 = memory[28];
	x1 = memory[29];
	x2 = memory[30];
	x3 = memory[31];
	x0 = memory[32];
	x1 = memory[33];
	x2 = memory[34];
	x3 = memory[35];
	x0 = memory[36];
	x1 = memory[37];
	x2 = memory[38];
	x3 = memory[39];
	x0 = memory[40];
	x1 = memory[41];
	x2 = memory[42];
	x3 = memory[43];
	x0 = memory[44];
	x1 = memory[45];
	x2 = memory[46];
	x3 = memory[47];
	x0 = memory[48];
	x1 = memory[49];
	x2 = memory[50];
	x3 = memory[51];
	x0 = memory[52];
	x1 = memory[53];
	x2 = memory[54];
	x3 = memory[55];
	x0 = memory[56];
	x1 = memory[57];
	x2 = memory[58];
	x3 = memory[59];
	x0 = memory[60];
	x1 = memory[61];
	x2 = memory[62];
	x3 = memory[63];
	memory += 64;
	count -= 64;
    }

    return (int) (memory) + count + x0 + x1 + x2 + x3;
}


static int ReadBandwidthTest2(volatile TESTDATAWIDE * memory, int count)
{
    register TESTDATAWIDE x0 = 0;
    register TESTDATAWIDE x1 = 0;
    register TESTDATAWIDE x2 = 0;
    register TESTDATAWIDE x3 = 0;

    while (count > 0) {
	x0 = memory[0];
	x1 = memory[1];
	x2 = memory[2];
	x3 = memory[3];
	x0 = memory[4];
	x1 = memory[5];
	x2 = memory[6];
	x3 = memory[7];
	x0 = memory[8];
	x1 = memory[9];
	x2 = memory[10];
	x3 = memory[11];
	x0 = memory[12];
	x1 = memory[13];
	x2 = memory[14];
	x3 = memory[15];
	x0 = memory[16];
	x1 = memory[17];
	x2 = memory[18];
	x3 = memory[19];
	x0 = memory[20];
	x1 = memory[21];
	x2 = memory[22];
	x3 = memory[23];
	x0 = memory[24];
	x1 = memory[25];
	x2 = memory[26];
	x3 = memory[27];
	x0 = memory[28];
	x1 = memory[29];
	x2 = memory[30];
	x3 = memory[31];
	memory += 64;
	count -= 64;
    }

    return (int) (memory) + count + x0 + x1 + x2 + x3;
}

#endif


/**************************************************************************

	Block Write test functions

**************************************************************************/

static int WriteBandwidthTest1(volatile TESTDATAWIDE * memory, int count)
{
    register TESTDATAWIDE x;

    x = 0;
    while (count > 0) {
	memory[0] = x;
	memory[1] = x;
	memory[2] = x;
	memory[3] = x;
	memory[4] = x;
	memory[5] = x;
	memory[6] = x;
	memory[7] = x;
	memory[8] = x;
	memory[9] = x;
	memory[10] = x;
	memory[11] = x;
	memory[12] = x;
	memory[13] = x;
	memory[14] = x;
	memory[15] = x;
	memory[16] = x;
	memory[17] = x;
	memory[18] = x;
	memory[19] = x;
	memory[20] = x;
	memory[21] = x;
	memory[22] = x;
	memory[23] = x;
	memory[24] = x;
	memory[25] = x;
	memory[26] = x;
	memory[27] = x;
	memory[28] = x;
	memory[29] = x;
	memory[30] = x;
	memory[31] = x;
	memory[32] = x;
	memory[33] = x;
	memory[34] = x;
	memory[35] = x;
	memory[36] = x;
	memory[37] = x;
	memory[38] = x;
	memory[39] = x;
	memory[40] = x;
	memory[41] = x;
	memory[42] = x;
	memory[43] = x;
	memory[44] = x;
	memory[45] = x;
	memory[46] = x;
	memory[47] = x;
	memory[48] = x;
	memory[49] = x;
	memory[50] = x;
	memory[51] = x;
	memory[52] = x;
	memory[53] = x;
	memory[54] = x;
	memory[55] = x;
	memory[56] = x;
	memory[57] = x;
	memory[58] = x;
	memory[59] = x;
	memory[60] = x;
	memory[61] = x;
	memory[62] = x;
	memory[63] = x;
	memory += 64;
	count -= 64;
    }

    return (int) memory + x;
}


static int WriteBandwidthTest2(volatile TESTDATAWIDE * memory, int count)
{
    register TESTDATAWIDE x;

    x = 0;
    while (count > 0) {
	memory[0] = x;
	memory[1] = x;
	memory[2] = x;
	memory[3] = x;
	memory[4] = x;
	memory[5] = x;
	memory[6] = x;
	memory[7] = x;
	memory[8] = x;
	memory[9] = x;
	memory[10] = x;
	memory[11] = x;
	memory[12] = x;
	memory[13] = x;
	memory[14] = x;
	memory[15] = x;
	memory[16] = x;
	memory[17] = x;
	memory[18] = x;
	memory[19] = x;
	memory[20] = x;
	memory[21] = x;
	memory[22] = x;
	memory[23] = x;
	memory[24] = x;
	memory[25] = x;
	memory[26] = x;
	memory[27] = x;
	memory[28] = x;
	memory[29] = x;
	memory[30] = x;
	memory[31] = x;
	memory += 64;
	count -= 64;
    }

    return (int) memory + x;
}



/**************************************************************************

	Block copy test functions

**************************************************************************/

static int CopyBandwidthTest1(volatile TESTDATAWIDE * memory, int count, int offset)
{
    register TESTDATAWIDE x0, x1, x2, x3, x4, x5, x6, x7;
    volatile TESTDATAWIDE *memory2;

    memory2 = memory + offset;
    while (count > 0) {
	x0 = memory[0];
	x1 = memory[1];
	x2 = memory[2];
	x3 = memory[3];
	x4 = memory[4];
	x5 = memory[5];
	x6 = memory[6];
	x7 = memory[7];
	memory2[0] = x0;
	memory2[1] = x1;
	memory2[2] = x2;
	memory2[3] = x3;
	memory2[4] = x4;
	memory2[5] = x5;
	memory2[6] = x6;
	memory2[7] = x7;
	x0 = memory[8];
	x1 = memory[9];
	x2 = memory[10];
	x3 = memory[11];
	x4 = memory[12];
	x5 = memory[13];
	x6 = memory[14];
	x7 = memory[15];
	memory2[8] = x0;
	memory2[9] = x1;
	memory2[10] = x2;
	memory2[11] = x3;
	memory2[12] = x4;
	memory2[13] = x5;
	memory2[14] = x6;
	memory2[15] = x7;
	x0 = memory[16];
	x1 = memory[17];
	x2 = memory[18];
	x3 = memory[19];
	x4 = memory[20];
	x5 = memory[21];
	x6 = memory[22];
	x7 = memory[23];
	memory2[16] = x0;
	memory2[17] = x1;
	memory2[18] = x2;
	memory2[19] = x3;
	memory2[20] = x4;
	memory2[21] = x5;
	memory2[22] = x6;
	memory2[23] = x7;
	x0 = memory[24];
	x1 = memory[25];
	x2 = memory[26];
	x3 = memory[27];
	x4 = memory[28];
	x5 = memory[29];
	x6 = memory[30];
	x7 = memory[31];
	memory2[24] = x0;
	memory2[25] = x1;
	memory2[26] = x2;
	memory2[27] = x3;
	memory2[28] = x4;
	memory2[29] = x5;
	memory2[30] = x6;
	memory2[31] = x7;
	memory += 32;
	count -= 32;
    }

    return (int) (memory) + (int) memory2 + offset + x0 + x1;
}


static int CopyBandwidthTest2(volatile TESTDATAWIDE * memory, int count, int offset)
{
    register TESTDATAWIDE x0, x1, x2, x3, x4, x5, x6, x7;
    volatile TESTDATAWIDE *memory2;

    memory2 = memory + offset;
    while (count > 0) {
	x0 = memory[0];
	x1 = memory[1];
	x2 = memory[2];
	x3 = memory[3];
	x4 = memory[4];
	x5 = memory[5];
	x6 = memory[6];
	x7 = memory[7];
	memory2[0] = x0;
	memory2[1] = x1;
	memory2[2] = x2;
	memory2[3] = x3;
	memory2[4] = x4;
	memory2[5] = x5;
	memory2[6] = x6;
	memory2[7] = x7;
	x0 = memory[8];
	x1 = memory[9];
	x2 = memory[10];
	x3 = memory[11];
	x4 = memory[12];
	x5 = memory[13];
	x6 = memory[14];
	x7 = memory[15];
	memory2[8] = x0;
	memory2[9] = x1;
	memory2[10] = x2;
	memory2[11] = x3;
	memory2[12] = x4;
	memory2[13] = x5;
	memory2[14] = x6;
	memory2[15] = x7;
	memory += 32;
	count -= 32;
    }

    return (int) (memory) + (int) memory2 + offset + x0 + x1;
}



/**************************************************************************

	Random Read test functions

**************************************************************************/

static int ReadRandomTest1(volatile TESTDATA2 * memory, int count)
{
    register TESTDATA x0 = 0;
    register TESTDATA x1 = 0;
    register TESTDATA x2 = 0;
    register TESTDATA x3 = 0;
    volatile TESTDATA2 *memory2 = memory + count;

    count /= 2;

    while (count > 0) {
	memory2 -= 0x200 / sizeof(TESTDATA2);
	x0 = memory[0x000 / sizeof(TESTDATA2)];
	x1 = memory2[0x1ff / sizeof(TESTDATA2)];
	x2 = memory[0x041 / sizeof(TESTDATA2)];
	x3 = memory2[0x1be / sizeof(TESTDATA2)];
	x0 = memory[0x082 / sizeof(TESTDATA2)];
	x1 = memory2[0x17d / sizeof(TESTDATA2)];
	x2 = memory[0x0c3 / sizeof(TESTDATA2)];
	x3 = memory2[0x13c / sizeof(TESTDATA2)];
	x0 = memory[0x104 / sizeof(TESTDATA2)];
	x1 = memory2[0x0fb / sizeof(TESTDATA2)];
	x2 = memory[0x145 / sizeof(TESTDATA2)];
	x3 = memory2[0x0ba / sizeof(TESTDATA2)];
	x0 = memory[0x186 / sizeof(TESTDATA2)];
	x1 = memory2[0x079 / sizeof(TESTDATA2)];
	x2 = memory[0x1c7 / sizeof(TESTDATA2)];
	x3 = memory2[0x038 / sizeof(TESTDATA2)];
#if CACHE_LINESIZE < 0x40
	x0 = memory[0x020 / sizeof(TESTDATA2)];
	x1 = memory2[0x1df / sizeof(TESTDATA2)];
	x2 = memory[0x061 / sizeof(TESTDATA2)];
	x3 = memory2[0x19e / sizeof(TESTDATA2)];
	x0 = memory[0x0a2 / sizeof(TESTDATA2)];
	x1 = memory2[0x15d / sizeof(TESTDATA2)];
	x2 = memory[0x0e3 / sizeof(TESTDATA2)];
	x3 = memory2[0x11c / sizeof(TESTDATA2)];
	x0 = memory[0x124 / sizeof(TESTDATA2)];
	x1 = memory2[0x0db / sizeof(TESTDATA2)];
	x2 = memory[0x165 / sizeof(TESTDATA2)];
	x3 = memory2[0x09a / sizeof(TESTDATA2)];
	x0 = memory[0x1a6 / sizeof(TESTDATA2)];
	x1 = memory2[0x059 / sizeof(TESTDATA2)];
	x2 = memory[0x1e7 / sizeof(TESTDATA2)];
	x3 = memory2[0x018 / sizeof(TESTDATA2)];
#endif
	memory += 0x200 / sizeof(TESTDATA2);
	count -= 0x200 / sizeof(TESTDATA2);
    }

    return (int) memory + count + x0 + x1 + x2 + x3;
}


static int ReadRandomTest2(volatile TESTDATA2 * memory, int count)
{
    register TESTDATA x0 = 0;
    register TESTDATA x1 = 0;
    register TESTDATA x2 = 0;
    register TESTDATA x3 = 0;
    volatile TESTDATA2 *memory2 = memory + count;

    count /= 2;

    while (count > 0) {
	memory2 -= 0x200 / sizeof(TESTDATA2);
	x0 = memory[0x000 / sizeof(TESTDATA2)];
	x1 = memory2[0x1ff / sizeof(TESTDATA2)];
	x2 = memory[0x041 / sizeof(TESTDATA2)];
	x3 = memory2[0x1be / sizeof(TESTDATA2)];
	x0 = memory[0x082 / sizeof(TESTDATA2)];
	x1 = memory2[0x17d / sizeof(TESTDATA2)];
	x2 = memory[0x0c3 / sizeof(TESTDATA2)];
	x3 = memory2[0x13c / sizeof(TESTDATA2)];
#if CACHE_LINESIZE < 0x40
	x0 = memory[0x020 / sizeof(TESTDATA2)];
	x1 = memory2[0x1df / sizeof(TESTDATA2)];
	x2 = memory[0x061 / sizeof(TESTDATA2)];
	x3 = memory2[0x19e / sizeof(TESTDATA2)];
	x0 = memory[0x0a2 / sizeof(TESTDATA2)];
	x1 = memory2[0x15d / sizeof(TESTDATA2)];
	x2 = memory[0x0e3 / sizeof(TESTDATA2)];
	x3 = memory2[0x11c / sizeof(TESTDATA2)];
#endif
	memory += 0x200 / sizeof(TESTDATA2);
	count -= 0x200 / sizeof(TESTDATA2);
    }

    return (int) memory + count + x0 + x1 + x2 + x3;
}


/**************************************************************************

	Random Write test functions

**************************************************************************/

static int WriteRandomTest1(volatile TESTDATA2 * memory, int count)
{
    register TESTDATA x0 = 0;
    register TESTDATA x1 = 0;
    register TESTDATA x2 = 0;
    register TESTDATA x3 = 0;
    volatile TESTDATA2 *memory2 = memory + count;

    count /= 2;

    while (count > 0) {
	memory2 -= 0x200 / sizeof(TESTDATA2);
	memory[0x000 / sizeof(TESTDATA2)] = x0;
	memory2[0x1ff / sizeof(TESTDATA2)] = x1;
	memory[0x041 / sizeof(TESTDATA2)] = x2;
	memory2[0x1be / sizeof(TESTDATA2)] = x3;
	memory[0x082 / sizeof(TESTDATA2)] = x0;
	memory2[0x17d / sizeof(TESTDATA2)] = x1;
	memory[0x0c3 / sizeof(TESTDATA2)] = x2;
	memory2[0x13c / sizeof(TESTDATA2)] = x3;
	memory[0x104 / sizeof(TESTDATA2)] = x0;
	memory2[0x0fb / sizeof(TESTDATA2)] = x1;
	memory[0x145 / sizeof(TESTDATA2)] = x2;
	memory2[0x0ba / sizeof(TESTDATA2)] = x3;
	memory[0x186 / sizeof(TESTDATA2)] = x0;
	memory2[0x079 / sizeof(TESTDATA2)] = x1;
	memory[0x1c7 / sizeof(TESTDATA2)] = x2;
	memory2[0x038 / sizeof(TESTDATA2)] = x3;
#if CACHE_LINESIZE < 0x40
	memory[0x020 / sizeof(TESTDATA2)] = x0;
	memory2[0x1df / sizeof(TESTDATA2)] = x1;
	memory[0x061 / sizeof(TESTDATA2)] = x2;
	memory2[0x19e / sizeof(TESTDATA2)] = x3;
	memory[0x0a2 / sizeof(TESTDATA2)] = x0;
	memory2[0x15d / sizeof(TESTDATA2)] = x1;
	memory[0x0e3 / sizeof(TESTDATA2)] = x2;
	memory2[0x11c / sizeof(TESTDATA2)] = x3;
	memory[0x124 / sizeof(TESTDATA2)] = x0;
	memory2[0x0db / sizeof(TESTDATA2)] = x1;
	memory[0x165 / sizeof(TESTDATA2)] = x2;
	memory2[0x09a / sizeof(TESTDATA2)] = x3;
	memory[0x1a6 / sizeof(TESTDATA2)] = x0;
	memory2[0x059 / sizeof(TESTDATA2)] = x1;
	memory[0x1e7 / sizeof(TESTDATA2)] = x2;
	memory2[0x018 / sizeof(TESTDATA2)] = x3;
#endif
	memory += 0x200 / sizeof(TESTDATA2);
	count -= 0x200 / sizeof(TESTDATA2);
    }

    return (int) (memory) + x0;
}

static int WriteRandomTest2(volatile TESTDATA2 * memory, int count)
{
    register TESTDATA x0 = 0;
    register TESTDATA x1 = 0;
    register TESTDATA x2 = 0;
    register TESTDATA x3 = 0;
    volatile TESTDATA2 *memory2 = memory + count;

    count /= 2;

    while (count > 0) {
	memory2 -= 0x200 / sizeof(TESTDATA2);
	memory[0x000 / sizeof(TESTDATA2)] = x0;
	memory2[0x1ff / sizeof(TESTDATA2)] = x1;
	memory[0x041 / sizeof(TESTDATA2)] = x2;
	memory2[0x1be / sizeof(TESTDATA2)] = x3;
	memory[0x186 / sizeof(TESTDATA2)] = x0;
	memory2[0x079 / sizeof(TESTDATA2)] = x1;
	memory[0x1c7 / sizeof(TESTDATA2)] = x2;
	memory2[0x038 / sizeof(TESTDATA2)] = x3;
#if CACHE_LINESIZE < 0x40
	memory[0x020 / sizeof(TESTDATA2)] = x0;
	memory2[0x1df / sizeof(TESTDATA2)] = x1;
	memory[0x061 / sizeof(TESTDATA2)] = x2;
	memory2[0x19e / sizeof(TESTDATA2)] = x3;
	memory[0x0a2 / sizeof(TESTDATA2)] = x0;
	memory2[0x15d / sizeof(TESTDATA2)] = x1;
	memory[0x0e3 / sizeof(TESTDATA2)] = x2;
	memory2[0x11c / sizeof(TESTDATA2)] = x3;
#endif
	memory += 0x200 / sizeof(TESTDATA2);
	count -= 0x200 / sizeof(TESTDATA2);
    }

    return (int) (memory) + x0;
}



/**************************************************************************

	Random Copy test functions

**************************************************************************/

static int CopyRandomTest1(volatile TESTDATA2 * memory, int count, int offset)
{
    register TESTDATA x0 = 0;
    register TESTDATA x1 = 0;
    register TESTDATA x2 = 0;
    register TESTDATA x3 = 0;
    volatile TESTDATA2 *memory2 = memory + offset + count;

    while (count > 0) {
	memory2 -= 0x200 / sizeof(TESTDATA2);
	x0 = memory[0x000 / sizeof(TESTDATA2)];
	memory2[0x1ff / sizeof(TESTDATA2)] = x0;
	x1 = memory[0x041 / sizeof(TESTDATA2)];
	memory2[0x1be / sizeof(TESTDATA2)] = x1;
	x2 = memory[0x082 / sizeof(TESTDATA2)];
	memory2[0x17d / sizeof(TESTDATA2)] = x2;
	x3 = memory[0x0c3 / sizeof(TESTDATA2)];
	memory2[0x13c / sizeof(TESTDATA2)] = x3;
	x0 = memory[0x104 / sizeof(TESTDATA2)];
	memory2[0x0fb / sizeof(TESTDATA2)] = x0;
	x1 = memory[0x145 / sizeof(TESTDATA2)];
	memory2[0x0ba / sizeof(TESTDATA2)] = x1;
	x2 = memory[0x186 / sizeof(TESTDATA2)];
	memory2[0x079 / sizeof(TESTDATA2)] = x2;
	x3 = memory[0x1c7 / sizeof(TESTDATA2)];
	memory2[0x038 / sizeof(TESTDATA2)] = x3;
#if CACHE_LINESIZE < 0x40
	x0 = memory[0x020 / sizeof(TESTDATA2)];
	memory2[0x1df / sizeof(TESTDATA2)] = x0;
	x1 = memory[0x061 / sizeof(TESTDATA2)];
	memory2[0x19e / sizeof(TESTDATA2)] = x1;
	x2 = memory[0x0a2 / sizeof(TESTDATA2)];
	memory2[0x15d / sizeof(TESTDATA2)] = x2;
	x3 = memory[0x0e3 / sizeof(TESTDATA2)];
	memory2[0x11c / sizeof(TESTDATA2)] = x3;
	x0 = memory[0x124 / sizeof(TESTDATA2)];
	memory2[0x0db / sizeof(TESTDATA2)] = x0;
	x1 = memory[0x165 / sizeof(TESTDATA2)];
	memory2[0x09a / sizeof(TESTDATA2)] = x1;
	x2 = memory[0x1a6 / sizeof(TESTDATA2)];
	memory2[0x059 / sizeof(TESTDATA2)] = x2;
	x3 = memory[0x1e7 / sizeof(TESTDATA2)];
	memory2[0x018 / sizeof(TESTDATA2)] = x3;
#endif
	memory += 0x200 / sizeof(TESTDATA2);
	count -= 0x200 / sizeof(TESTDATA2);
    }

    return (int) memory + (int) memory2 + count + x0 + x1 + x2 + x3;
}

static int CopyRandomTest2(volatile TESTDATA2 * memory, int count, int offset)
{
    register TESTDATA x0 = 0;
    register TESTDATA x1 = 0;
    register TESTDATA x2 = 0;
    register TESTDATA x3 = 0;
    volatile TESTDATA2 *memory2 = memory + offset + count;

    while (count > 0) {
	memory2 -= 0x200 / sizeof(TESTDATA2);
	x0 = memory[0x000 / sizeof(TESTDATA2)];
	memory2[0x1ff / sizeof(TESTDATA2)] = x0;
	x1 = memory[0x041 / sizeof(TESTDATA2)];
	memory2[0x1be / sizeof(TESTDATA2)] = x1;
	x2 = memory[0x186 / sizeof(TESTDATA2)];
	memory2[0x079 / sizeof(TESTDATA2)] = x2;
	x3 = memory[0x1c7 / sizeof(TESTDATA2)];
	memory2[0x038 / sizeof(TESTDATA2)] = x3;
#if CACHE_LINESIZE < 0x40
	x0 = memory[0x020 / sizeof(TESTDATA2)];
	memory2[0x1df / sizeof(TESTDATA2)] = x0;
	x1 = memory[0x061 / sizeof(TESTDATA2)];
	memory2[0x19e / sizeof(TESTDATA2)] = x1;
	x2 = memory[0x0a2 / sizeof(TESTDATA2)];
	memory2[0x15d / sizeof(TESTDATA2)] = x2;
	x3 = memory[0x0e3 / sizeof(TESTDATA2)];
	memory2[0x11c / sizeof(TESTDATA2)] = x3;
#endif
	memory += 0x200 / sizeof(TESTDATA2);
	count -= 0x200 / sizeof(TESTDATA2);
    }

    return (int) memory + (int) memory2 + count + x0 + x1 + x2 + x3;
}



/**************************************************************************

	Read Latency test functions

	Latency is determined by comparing the runtime of a function
	with a memory access with the same function without memory access.
	Near/Far latency is meassured by two function sets.
	The symmetry of the two functions is very critical.

**************************************************************************/

static int ReadLatencyTestNear1(volatile TESTDATA2 * memory, int count, int latencymax, int offset)
{
    int             i;
    register TESTDATA x, x0, x1, x2, x3;
    volatile TESTDATA2 *memory2 = memory + count;

    x0 = 1;
    x1 = offset;
    x2 = count;
    x3 = latencymax;

    while (count > 0) {
	x = memory[0];
	x0 |= x;
	if (!x0) {
	    x1 = x1 >> 1;
	    x2 = x2 << 1;
	    x0 |= x1 | x2;
	}
	if (offset) {
	    for (i = 0; i < latencymax; i++) {
		x1 = x1 >> 1;
		x2 = x2 << 1;
		x0 |= x1 | x2;
	    }
	}
	memory += offset;
	count -= offset;
    }

    return (int) memory + (int) memory2 + x0 + x1 + x2 + x3 + offset;
}


static int ReadLatencyTestNear2(volatile TESTDATA2 * memory, int count, int latencymax, int offset)
{
    int             i;
    register TESTDATA x, x0, x1, x2, x3;
    volatile TESTDATA2 *memory2 = memory + count;

    x0 = 1;
    x1 = offset;
    x2 = count;
    x3 = latencymax;

    while (count > 0) {
	x = (int) memory + x0;
	x0 |= x;
	if (!x0) {
	    x1 = x1 >> 1;
	    x2 = x2 << 1;
	    x0 |= x1 | x2;
	}
	if (offset) {
	    for (i = 0; i < latencymax; i++) {
		x1 = x1 >> 1;
		x2 = x2 << 1;
		x0 |= x1 | x2;
	    }
	}
	memory += offset;
	count -= offset;
    }

    return (int) memory + (int) memory2 + x0 + x1 + x2 + x3 + offset;
}



static int ReadLatencyTestFar1(volatile TESTDATA2 * memory, int count, int latencymax, int offset)
{
    int             i;
    register TESTDATA x, x0, x1, x2, x3;
    volatile TESTDATA2 *memory2 = memory + count - offset;

    x = 0;
    x0 = 1;
    x1 = offset;
    x2 = count;
    x3 = latencymax;

    count /= 2;

    while (count > 0) {
	x = memory[0];
	x0 |= x;
	if (!x0) {
	    x1 = x1 >> 1;
	    x2 = x2 << 1;
	    x0 |= x1 | x2;
	}
	if (offset) {
	    for (i = 0; i < latencymax; i++) {
		x1 = x1 >> 1;
		x2 = x2 << 1;
		x0 |= x1 | x2;
	    }
	}
	x = memory2[0];
	x0 |= x;
	if (!x0) {
	    x1 = x1 >> 1;
	    x2 = x2 << 1;
	    x0 |= x1 | x2;
	}
	if (offset) {
	    for (i = 0; i < latencymax; i++) {
		x1 = x1 >> 1;
		x2 = x2 << 1;
		x0 |= x1 | x2;
	    }
	}
	memory += offset;
	memory2 -= offset;
	count -= offset;
    }

    return (int) memory + (int) memory2 + x + x0 + x1 + x2;
}


static int ReadLatencyTestFar2(volatile TESTDATA2 * memory, int count, int latencymax, int offset)
{
    int             i;
    register TESTDATA x, x0, x1, x2, x3;
    volatile TESTDATA2 *memory2 = memory + count - offset;

    x = 0;
    x0 = 1;
    x1 = offset;
    x2 = count;
    x3 = latencymax;

    count /= 2;

    while (count > 0) {
	x = (int) memory + x0;
	x0 |= x;
	if (!x0) {
	    x1 = x1 >> 1;
	    x2 = x2 << 1;
	    x0 |= x1 | x2;
	}
	if (offset) {
	    for (i = 0; i < latencymax; i++) {
		x1 = x1 >> 1;
		x2 = x2 << 1;
		x0 |= x1 | x2;
	    }
	}
	x = (int) memory2 + x0;
	x0 |= x;
	if (!x0) {
	    x1 = x1 >> 1;
	    x2 = x2 << 1;
	    x0 |= x1 | x2;
	}
	if (offset) {
	    for (i = 0; i < latencymax; i++) {
		x1 = x1 >> 1;
		x2 = x2 << 1;
		x0 |= x1 | x2;
	    }
	}
	memory += offset;
	memory2 -= offset;
	count -= offset;
    }

    return (int) memory + (int) memory2 + x + x0 + x1 + x2;
}
