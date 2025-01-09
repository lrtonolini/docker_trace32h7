typedef enum{False,True} Boolean;


extern void inc(int* x);

void func222()
{
	/*
	 * The odd numbers between 3 and 16383, inclusive, are represented by
	 *   the numbers between 0 and 8191, inclusive, according to the following scheme:
	 *   if n in [0..8191], 2n+3 is the represented number.
	 * Each number 2n+3 is considered in turn.
	 * Its first odd multiple is (2n+3)*3, or 6n+9, or 2*(n+2n+3)+3.
	 * Thus the statements
	 *    Prime = 2*I+3;	       -- 2n+3
	 * which computes the prime, and
	 * 	  K = I+Prime;	       -- 2K+3 = 2*(n+2n+3)+3 = 6n+9
	 * which computes the index of the first odd multiple.
	 */

	#define SIZE 8190
	const int      size = SIZE;
	static Boolean Primes[SIZE+1];
	static int     Count = 0;
	int            I,Prime,K;

	/*
	 * find the prime numbers and save in Primes array
	 */
	Count = 0;
	for (I = 0; I <= size; inc(&I))
		Primes[I] = True;
	for (I = 0; I <= size; inc(&I))
		if (Primes[I]) {
			Prime = 2*I+3;
			K = I+Prime;
			while (K <= size) {
				Primes[K] = False;
				K += Prime;
			}
			inc(&Count);
		}
}


#define a 16807         /* multiplier */
#define m 2147483647L   /* 2**31 - 1 */
#define q 127773L       /* m div a */
#define r 2836          /* m mod a */

long nextlongrand(long seed)
{
	unsigned long lo, hi;

	lo = a * (long)(seed & 0xFFFF);
	hi = a * (long)((unsigned long)seed >> 16);
	lo += (hi & 0x7FFF) << 16;
	if (lo > m)
	{
		lo &= m;
		++lo;
	}
	lo += hi >> 15;
	if (lo > m)
	{
		lo &= m;
		++lo;
	}
	return (long)lo;
}


long longrand(void)                     /* return next random long */
{
	static long randomnum = 1;

	randomnum = nextlongrand(randomnum);
	return randomnum;
}


unsigned long int ra;

void main2 ()
{
	func222();
	ra = longrand();
}
