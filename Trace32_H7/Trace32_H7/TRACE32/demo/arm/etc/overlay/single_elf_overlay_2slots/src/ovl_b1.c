#include "defines.h"


char subst( char c )
{
	switch ( c ){
		case 'a':
			return 'e';
		case 'e':
			return 'i';
		case 'i':
			return 'u';
		case 'o':
			return 'a';
		case 'u':
			return 'o';
	}
	return c;
}


int getMagic(int x)
{
	return 103 * x;
}


typedef enum { FALSE = 0, TRUE = 1 } bool_t;

#define SIZE 18
bool_t flags[SIZE+1];

int sieve(void)
{
	register int    i, prime, k;
	int             count;

	LoadSlotA(1);

	count = 0;

	for ( i = 0 ; i <= SIZE ; flags[i++] = TRUE ) ;

	for (i = 0; i <= SIZE; i++) {
		if (flags[i]) {
			prime = i + i + 3;
			k = i + prime;
			while (k <= SIZE) {
				flags[k] = FALSE;
				k += prime;
			}
			inc(&count);
		}
	}

	LoadSlotA(0);

	return count;
}
