#include "defines.h"


char* encode(char *str)
{
	int i;

	LoadSlotB(1);

	for (i = 0; str[i]; i++){
		char c = str[i];
		if ('A' <= c  &&  c <= 'Z'){
			c = c + ('a'-'A');
			c = subst(c);
			c = c - ('a'-'A');
		} else {
			c = subst(c);
		}
		str[i] = c;
	}

	LoadSlotB(0);

	return str;
}


static int base = 100;

int getLfeInfo()
{
	return base - 77;
}


void inc( int * intptr )
{
	(*intptr)++;
}

