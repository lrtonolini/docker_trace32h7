
extern int mstatic1;
extern char vchar;
extern void func1( int * intptr );

void func2(void)
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

void func2a(void)
{
	auto char autovar;	/* char stack variable */
	register char regvar;	/* char register variable */

	autovar = regvar = (char) mstatic1;
	autovar++;

	for ( regvar = 0; regvar < (char) 5 ; regvar++ )
		vchar = (char)(vchar+regvar*autovar);
}

