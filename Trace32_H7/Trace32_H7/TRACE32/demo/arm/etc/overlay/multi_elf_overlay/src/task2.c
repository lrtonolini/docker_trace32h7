int func13( int a, int c, int e )	/* arguments and locals stack-tracking */
{
	int b, d, f;

	b = a+c+e;
	f = b+a;
	d = f*b;

	if ( e > 0 )
		c += func13( b, f, e-1 );

	return c+e+d;
}
