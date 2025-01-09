
/*
 * The following definitions must be included 
 * in all files using the memory allocation
 * when the patches are made in the source.
 * An alternative is to use the Data.ReRoute
 * to change the calls WITHOUT recompilaton.
 */

#ifdef PATCHING_REQUIRED
#define malloc(size)		T32_malloc(size)
#define realloc(ptr,size)	T32_realloc(ptr,size)
#define free(ptr)		T32_free(ptr)
extern void * T32_malloc();
extern void * T32_realloc();
extern void T32_free();
#endif


#include "stddef.h"
#include "stdlib.h"

char * memorypointer[10];


static void memorytest()
{
    int             i;
    char           *p;

    memorypointer[0] = malloc(100);
    memorypointer[1] = malloc(200);
    memorypointer[2] = malloc(300);
    memorypointer[3] = malloc(0);

    memorypointer[2] = realloc(memorypointer[2], 100);

    for (i = 4; i < 10; i++) {
	memorypointer[i] = malloc((i + 1) * 10);
    }

    for (i = 0; i < 10; i++) {
	p = malloc(i + 1);
	free(p);
    }

    /* write before start of memory block */

    strcpy(memorypointer[0] - 1, "test");

    /* write after end of memory block */

    strcpy(memorypointer[1] + 200, "test");
    
    /* free destroyed memory blocks */

    free(memorypointer[0]);
    free(memorypointer[1]);
}


void main()
{
    T32_LoggerInit();
    
    memorytest();
}

