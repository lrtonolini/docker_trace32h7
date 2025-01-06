
const char cstr1[] = "Constant String1";

int mstatic1;
static int mstatic2, mcount;

char vchar;
short vshort;
long vlong;

char vtriplearray[2][3][4];

typedef struct struct1 {
	int count;
	struct struct1 * left;
	int field1:2;
	unsigned field2:3;
} strtype1 ;

strtype1 ast;

int ( *funcptr ) ();


#include "task2.c"
extern void func2(void);
extern void func2a(void);
extern int  func3(void);
extern int  func5(int a, char b, long c);
extern int  func11(int x);
extern int  sieve(void);


void func1( int * intptr )                 /* static function */
{
	(*intptr)++;
}

struct struct1 func4( struct struct1 str ) /* function returning struct */
{
	str.count++;
	return str;
}

static int * func9()                       /* nested local variables */
{
	static int stat1 = 0;
	register int reg1;
	auto int auto1;

	auto1 = stat1;

	for ( reg1 = 0 ; reg1 < 2 ; reg1++ )
	{
		static int stat2 = 0;
		register int reg2;
		auto int auto2;

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

long func25()                              /* Long Return */
{
	return 12345678l;
}

struct shortrecord
{
	short x;
	short y;
};

struct shortrecord vshortrecord;

struct shortrecord func28( struct shortrecord x )
{
	x.x++;
	x.y = (short) (x.x+1);
	return x;
}

void main1()
{
	int j;

	mstatic1 = 12;
	mstatic2 = 34;
	mcount++;

	vtriplearray[0][0][0] = 1;
	vtriplearray[1][0][0] = 2;
	vtriplearray[0][1][0] = 3;
	vtriplearray[0][0][1] = 4;

	func2();
	func2a();

	funcptr = (int (*)()) 0;
	funcptr = func3;

	ast.count = 12345;
	ast.left = &ast;
	ast.field1 = 1+mstatic2;
	ast.field2 = 2;

	ast = func4( ast );

	j = (*funcptr)();

start:
	j = func5( (int) j, (char) 2, (long) 3 );

	if ( j == 0 )
		goto start;

	func9();
	func11( 5 );
	func13( 1, 2, 3 );

	vlong = func25();

	vshortrecord = func28(vshortrecord);

	sieve();
}



