/* Example for code linked to one overlay page and to the common area.
 * This can be debugged with file-base code overlay support.
 * To have code linked to more than one overlay page in the same source
 *  file you need relocation-base code overlay support (see mix.c)
 */


/* Function func5() only available if overlay segment "page1" is active */
__attribute__ ((section (".page1")))
int func5( int a, char b, long c )         /* multiple arguments */
{
	return a+b*c;
}


/* Function memcopy() always available */
char* memcopy(char *dst, const char *src, int n)
{
	for(;n;n--) {
		*dst = *src;
		dst++;
		src++;
	}
	return dst;
}


/* Function func11() only available if overlay segment "page1" is active */
__attribute__ ((section (".page1")))
int func11( int x )
{
	switch ( x )
	{
		case 1:
			x = x+1;
			x = x*2;
			return x*x;
		case 2:
			return x+x;
		case 3:
			return x-x;
		case 4:
			x = x+1;
			x = x*2;
			return x*x;
		case 5:
			break;
		case 6:
			return x+x;
		default:
			break;
	}
	return x;
}

