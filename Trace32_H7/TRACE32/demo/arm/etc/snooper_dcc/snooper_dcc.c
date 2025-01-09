
#define TRUE 1
#define FALSE 0
#define SIZE 18


extern unsigned long T32_TsMon_SendStatus(void); /*returns 0 when a value can be written to DCC*/
extern void T32_TsMon_SendWord(unsigned long data); /*send one word (4 bytes)*/

static int mstatic1, mstatic2, mcount = 0;

char vtriplearray[2][3][4];

int sieve(void);


static void SnoopData(unsigned int data)
{
	while (T32_TsMon_SendStatus()); //get status of the com-channel
	T32_TsMon_SendWord(data); //if it‘s free, send data to channel
}

int main(void)
{

	while(TRUE)
	{
		mstatic1 = 12;
		mstatic2 = 34;
		mcount++;
		SnoopData(mcount);

		vtriplearray[0][0][0] = 1;
		vtriplearray[1][0][0] = 2;
		vtriplearray[0][1][0] = 3;
		vtriplearray[0][0][1] = 4;

		sieve();
	}
}

char flags[SIZE+1];

int sieve(void)				/* sieve of erathostenes */
{
	register int i, primz, k;
	int anzahl;

	anzahl = 0;

	for ( i = 0 ; i <= SIZE ; flags[ i++ ] = TRUE ) ;

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

int __attribute__ ((naked)) background(void)		/* job for background-demo - doesn't use stack*/
{
	register long count1, count2;

	count1 = count2 = 0;

	while ( TRUE )
	{
		while ( ++count1 ) ;
		count2++;
	}

	return TRUE;
}
