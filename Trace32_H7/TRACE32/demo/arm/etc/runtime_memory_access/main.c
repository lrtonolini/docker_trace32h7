
/**************************************************************************

	Calling the Monitor

	The Monitor_Handler() needs to be called by the application routine.

	TSMON, UDMON:
	The call is typically included in a periodic interrupt or in the idle
	task of the kernel.

	PTMON:
	The call is typically included in the interrupt service routine which
	will be triggered by the trigger output signal coming from the debugger.

	Here, for demonstration purpose it is simply called in an endless loop.

**************************************************************************/

static int  mcount = 0;
static char flags[19];
static int  sieve(void);
extern void Monitor_Handler(void);


int main()
{
	while (1) {
		mcount++;
		Monitor_Handler();  /* <<< Calling TSMON/UDMON monitor handler <<< */
		sieve();
	}
}


static int sieve(void)  /* sieve of erathostenes */
{
	register int    i, prime, k;
	int             count;

	count = 0;

	for ( i = 0 ; i < sizeof(flags) ; flags[ i++ ] = 1 ) ;

	for (i = 0; i < sizeof(flags); i++) {
		if (flags[i]) {
			prime = i + i + 3;
			k = i + prime;
			while (k < sizeof(flags)) {
				flags[k] = 0;
				Monitor_Handler();  /* <<< Calling TSMON/UDMON monitor handler <<< */
				k += prime;
			}
			count++;
		}
	}

	return count;
}
