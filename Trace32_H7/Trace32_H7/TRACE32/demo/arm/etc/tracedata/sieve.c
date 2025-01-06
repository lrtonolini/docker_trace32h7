
#define TRUE 1
#define FALSE 0
#define SIZE 18

#include "midi.h"

extern void T32_DataTrace_Write8(void*, unsigned char);
extern void T32_DataTrace_Write32(void*, unsigned long);

static int mstatic1, mstatic2, mcount;
const char cstr1[] = "Constant String1";

char vdblarray[5][6];
char vtriplearray[2][3][4];

typedef struct struct1 {
	char * word;
	int count;
	struct struct1 * left;
	struct struct1 * right;
	int field1:2;
	unsigned field2:3;
} strtype1 ;

strtype1 ast;

struct abc
{
	int x;
	int y;
};

struct abc def;

static void func1( intptr )		/* static function */
int * intptr;
{
	(*intptr)++;
}

void func2()
{
	int autovar;
	register int regvar;
	static int fstatic = 44;	/* initialized static variable */
	static int fstatic2;		/* not initialized static variable */

	autovar = regvar = fstatic;
	autovar++;

	func1( &autovar );	/* to force autovar as stack-scope */

	func1( &fstatic );	/* to force fstatic as static-scope */

	for ( regvar = 0; regvar < 5 ; regvar++ )
		mstatic1 += regvar*autovar;

	fstatic += mstatic1;

	fstatic2 = 2*fstatic;

	func1( &fstatic2 );
}

static int * func9()			/* nested local variables */
{
	static stat1 = 0;
	register reg1;
	auto auto1;

	auto1 = stat1;

	for ( reg1 = 0 ; reg1 < 2 ; reg1++ )
	{
		static stat2 = 0;
		register reg2;
		auto auto2;

		auto2 = stat2;

		for ( reg2 = 0 ; reg2 < reg1 ; reg2++ )
		{
			func1( &stat1 );
			func1( &auto1 );
			func1( &stat2 );
			func1( &auto2 );
		}
	}

	return &stat1;
}

int func13( a, c, e )			/* arguments and locals stack-tracking */
int a, c, e;
{
	int b, d, f;

	b = a+c+e;
	f = b+a;
	d = f*b;

	if ( e > 0 )
		c += func13( b, f, e-1 );

	return c+e+d;
}

int sieve();

void (*monHook)(void) __attribute__ ((section (".data"))) = 0;
extern void (*watchdogTrigger)(void);

int main()
{
	while(TRUE)
	{
		if (monHook)
			monHook();
#ifdef __GNUC__
		if (watchdogTrigger)
			watchdogTrigger();
#endif

		mstatic1 = 12;
		mstatic2 = 34;
		mcount++;
		T32_DataTrace_Write32(&mcount, mcount);

		vtriplearray[0][0][0] = 1;
		T32_DataTrace_Write8(&vtriplearray[0][0][0], 1);
		vtriplearray[1][0][0] = 2;
		T32_DataTrace_Write8(&vtriplearray[1][0][0], 2);
		vtriplearray[0][1][0] = 3;
		T32_DataTrace_Write8(&vtriplearray[0][1][0], 3);
		vtriplearray[0][0][1] = 4;
		T32_DataTrace_Write8(&vtriplearray[0][0][1], 4);

		func2();

		func9();

		func13( 1, 2, 3 );

		sieve();
	}
}

char flags[SIZE+1];
int  primz;

int sieve()				/* sieve of erathostenes */
{
	register int i, k;
	int anzahl;

	anzahl = 0;

	for ( i = 0 ; i <= SIZE ; flags[ i++ ] = TRUE ) ;

	for ( i = 0 ; i <= SIZE ; i++ )
	{
		if ( flags[ i ] )
		{
			primz = i + i + 3;
			k = i + primz;
			T32_DataTrace_Write32(&primz, primz);
			while ( k <= SIZE )
			{
				flags[ k ] = FALSE;
				k += primz;
			}
			anzahl++;
		}
	}

	return anzahl;
}

