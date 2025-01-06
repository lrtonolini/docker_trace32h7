#define TRUE 1
#define FALSE 0
#define SIZE 18

char flags[SIZE+1];

int sieve(void)  /* sieve of erathostenes */
{
	register int i, primz, k;
	int anzahl;

	anzahl = 0;

	for ( i = 0 ; i <= SIZE ; flags[ i++ ] = TRUE );

	for ( i = 0 ; i <= SIZE ; i++ )
	{
		if ( flags[ i ] )
		{
			primz = i + i + 3;
			k = i + primz;
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
