#include "defines.h"


int func42(char *txt1, char *txt2)
{
	int life, universe;
	int x;

	LoadSlotA(1);

	encode(txt1);
	encode(txt2);

	life = getLfeInfo();

	LoadSlotA(0);

	universe = checkUniv();

	x = getFinalAnswer(life, universe);
	return x;
}


int func13( int a, int c, int e )
{
	int b, d, f;

	b = a+c+e;
	f = b+a;
	d = f*b;

	if ( e > 0 )
		c += func13( b, f, e-1 );

	return c+e+d;
}


void func2(int *var)
{
	int autovar;
	register int regvar;
	static int fstatic = 44;	/* initialized static variable */
	static int fstatic2;		/* not initialized static variable */

	LoadSlotA(1);

	autovar = regvar = fstatic;
	autovar++;

	inc( &autovar );	/* to force autovar as stack-scope */
	inc( &fstatic );	/* to force fstatic as static-scope */

	for ( regvar = 0; regvar < 5 ; regvar++ )
		*var += regvar*autovar;

	fstatic += *var;
	fstatic2 = 2*fstatic;

	inc( &fstatic2 );

	LoadSlotA(0);
	return;
}
