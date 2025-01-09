
#include "stddef.h"
#include "math.h"


float vfloat;
double vdouble;

void func2c()
{
	auto double autovar;	/* double stack variable */
	register double regvar;	/* double register variable */

	autovar = regvar = vdouble;
	autovar++;

	for ( regvar = 0.0; regvar < 5.0 ; regvar += 1.0 )
		vdouble += regvar*autovar;
}


float func6( float a, float b )
{
	vfloat = 1.0;
	vfloat = -1.0;
	vfloat = 10.0;
	vfloat = (float) 1.6;

	return a*b;
}

double func7( double a, double b )
{
	vdouble = 1.0;
	vdouble = -1.0;
	vdouble = 10.0;
	vdouble = 1.6;

	return a*b;
}


double func13(double a, double c, double e)	/* arguments and locals stack-tracking */
{
	double b, d, f;

	b = a+c+e;
	f = b+a;
	d = f*b;

	if ( e > 0 )
		c += func13( b, f, e-1 );

	return c+e+d;
}


double sinewave[130];

void funcsinewave()
{
	int index;
	double x;

	index = 0;
	for ( x = 0.0 ; x < 12.0 ; x += 0.1 )
		sinewave[index++] = sin(x)/(x+0.1);
}


int main()
{
    while (1) {

	funcsinewave();

	func2c();

	func6((float)1.2,(float)3.4);

	func7(1.2,3.4);

	func13(1.0,2.0,3.0);
    }

    return 0;
}


